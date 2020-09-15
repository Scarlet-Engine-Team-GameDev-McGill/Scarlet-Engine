#include "Core/Engine.h"

#include "Core/CoreUtils.h"
#include "Core/ITickable.h"
#include "Renderer/Renderer.h"

#include <chrono>
#include <algorithm>

#define FIXED_UPDATE_MS 20.0

namespace ScarletEngine
{
	Engine::Engine()
		: VariableUpdateTickables()
		, FixedUpdateTickables()
		, TickableQueue()
		, bIsInitialized(false)
		, bIsRunning(false)
		, bIsTerminated(false)
		, bTickingObjects(false)
	{

	}

	void Engine::Initialize()
	{
		Logger::Get().SetLogFile("Log.txt");

		Renderer::Get().Initialize();
		bIsInitialized = true;
	}

	void Engine::Run()
	{
		check(bIsInitialized);

		std::chrono::high_resolution_clock Clock;

		using ms = std::chrono::duration<double, std::milli>;
		auto LastTime = Clock.now();
		double Lag = 0.0;

		bIsRunning = true;
		while (bIsRunning)
		{
			const double DeltaTime = std::chrono::duration_cast<ms>(Clock.now() - LastTime).count();
			LastTime = Clock.now();
			Lag += DeltaTime;

			PreUpdate();
			// In this scope we are ticking objects
			{
				bTickingObjects = true;
				// If it has been more than FIXED_UPDATE_MS since our last fixed timestep update,
				// we should run a fixed update before running a regular update
				while (Lag >= FIXED_UPDATE_MS)
				{
					Lag -= FIXED_UPDATE_MS;
					FixedUpdate(FIXED_UPDATE_MS);
				}

				Update(DeltaTime);
				bTickingObjects = false;
			}

			// Process anything that should happen before the next update
			PostUpdate();
		}

		Terminate();
	}

	void Engine::PreUpdate()
	{
		AddQueuedTickables();
	}

	void Engine::PostUpdate()
	{
		Renderer::Get().DrawFrame();
	}

	void Engine::Terminate()
	{
		bIsInitialized = false;
		VariableUpdateTickables.clear();
		FixedUpdateTickables.clear();

		bIsTerminated = true;
	}

	void Engine::Update(double DeltaTime)
	{
		for (const auto Tickable : VariableUpdateTickables)
		{
			Tickable->Tick(DeltaTime);
		}
	}

	void Engine::FixedUpdate(double DeltaTime)
	{
		for (const auto Tickable : FixedUpdateTickables)
		{
			Tickable->Tick(DeltaTime);
		}
	}

	void Engine::QueueAddTickable(ITickable* TickableObject)
	{
		check(bIsInitialized);
		std::lock_guard<std::mutex> Lock(TickableQueueMutex);

		TickableQueue.push_back(TickableObject);
	}

	void Engine::RemoveTickable(ITickable* TickableObject)
	{
		// Tickables cannot be removed while objects are being ticked as this would invalidate the iterator
		check(!bTickingObjects);

		if (TickableObject->WantsFixedTimestep())
		{
			auto It = std::remove(FixedUpdateTickables.begin(), FixedUpdateTickables.end(), TickableObject);
			if (It != FixedUpdateTickables.end())
			{
				FixedUpdateTickables.erase(It, FixedUpdateTickables.end());
			}
		}
		else
		{
			auto It = std::remove(VariableUpdateTickables.begin(), VariableUpdateTickables.end(), TickableObject);
			if (It != VariableUpdateTickables.end())
			{
				VariableUpdateTickables.erase(It, VariableUpdateTickables.end());
			}
		}
	}

	void Engine::AddQueuedTickables()
	{
		std::lock_guard<std::mutex> Lock(TickableQueueMutex);
		// Add any new tickables which may have been enqueued during the last frame
		for (const auto TickableToAdd : TickableQueue)
		{
			AddTickable(TickableToAdd);
		}
		TickableQueue.clear();
	}

	void Engine::AddTickable(ITickable* TickableObject)
	{
		check(bIsInitialized);
		check(!bTickingObjects);

		if (TickableObject->WantsFixedTimestep())
		{
			FixedUpdateTickables.push_back(TickableObject);
		}
		else
		{
			VariableUpdateTickables.push_back(TickableObject);
		}
	}
}