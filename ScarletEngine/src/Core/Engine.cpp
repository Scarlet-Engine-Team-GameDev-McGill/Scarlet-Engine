#include "Core/Engine.h"

#include "Core/CoreUtils.h"
#include "Core/ITickable.h"
#include "Renderer/Renderer.h"

#include <chrono>
#include <common/TracySystem.hpp>
#include "UI/UIStyleRegistry.h"
#include "UI/UISystem.h"
#include "UI/CoreUIStyle.h"

#define FIXED_UPDATE_MS 20.0

namespace ScarletEngine
{
	UniquePtr<Engine> GEngine = nullptr;

	Engine::Engine()
		: VariableUpdateTickables()
		, FixedUpdateTickables()
		, TickableQueue()
		, bIsInitialized(false)
		, bIsRunning(false)
		, bIsTerminated(false)
		, bTickingObjects(false)
	{
		tracy::SetThreadName("Engine Thread");
	}

	void Engine::Initialize()
	{
		ZoneScoped
		Logger::Get().SetLogFile("Log.txt");

		Renderer::Get().Initialize();

		UIStyleRegistry::Get().RegisterStyle("Core", MakeShared<CoreUIStyle>());

		bIsInitialized = true;
	}

	void Engine::Run()
	{
		ZoneScoped
		check(bIsInitialized);

		std::chrono::high_resolution_clock Clock;

		using ms = std::chrono::duration<double, std::milli>;
		auto LastTime = Clock.now();
		double Lag = 0.0;

		bIsRunning = true;
		while (bIsRunning)
		{
			FrameMark
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
	}

	void Engine::PreUpdate()
	{
		ZoneScoped
		AddQueuedTickables();
		
		UISystem::Get().PreUpdate();
	}

	void Engine::PostUpdate()
	{
		ZoneScoped

		UISystem::Get().PostUpdate();
		Renderer::Get().EndFrame();
	}

	void Engine::Terminate()
	{
		ZoneScoped
		UISystem::Get().Shutdown();

		bIsInitialized = false;
		VariableUpdateTickables.clear();
		FixedUpdateTickables.clear();

		bIsTerminated = true;
	}

	void Engine::Update(double DeltaTime)
	{
		ZoneScoped
		for (const auto Tickable : VariableUpdateTickables)
		{
			Tickable->Tick(DeltaTime);
		}
	}

	void Engine::FixedUpdate(double DeltaTime)
	{
		ZoneScoped
		for (const auto Tickable : FixedUpdateTickables)
		{
			Tickable->Tick(DeltaTime);
		}
	}

	void Engine::QueueAddTickable(ITickable* TickableObject)
	{
		ZoneScoped
		//std::lock_guard<std::mutex> Lock(TickableQueueMutex);

		TickableQueue.push_back(TickableObject);
	}

	void Engine::RemoveTickable(ITickable* TickableObject)
	{
		ZoneScoped
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
		ZoneScoped
		//std::lock_guard<std::mutex> Lock(TickableQueueMutex);
		// Add any new tickables which may have been enqueued during the last frame
		for (const auto TickableToAdd : TickableQueue)
		{
			AddTickable(TickableToAdd);
		}
	}

	void Engine::AddTickable(ITickable* TickableObject)
	{
		ZoneScoped
		check(bIsInitialized);
		check(!bTickingObjects);

		TickableObject->Initialize();

		if (TickableObject->WantsFixedTimestep())
		{
			FixedUpdateTickables.push_back(TickableObject);
		}
		else
		{
			VariableUpdateTickables.push_back(TickableObject);
		}

		auto It = std::remove(TickableQueue.begin(), TickableQueue.end(), TickableObject);
		if (It != TickableQueue.end())
		{
			TickableQueue.erase(It, TickableQueue.end());
		}
	}
}