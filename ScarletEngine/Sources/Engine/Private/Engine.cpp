#include "Engine.h"

#include "CoreUtils.h"
#include "ITickable.h"

#include <chrono>
#include <common/TracySystem.hpp>
#include "ModuleManager.h"
#include "Window.h"

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
	}

	void Engine::Initialize()
	{
		ZoneScoped
		Logger::Get().SetLogFile("Log.txt");

		AppWindow = ScarNew(ApplicationWindow, 800, 600, "Scarlet Engine");
		check(AppWindow);

		AppWindow->OnWindowClose.BindMember(this, &Engine::SignalQuit);

		ModuleManager::GetInstance().Startup();

		bIsInitialized = true;
	}

	void Engine::Run()
	{
		ZoneScoped
		tracy::SetThreadName("Engine Thread");

		check(bIsInitialized);

		std::chrono::high_resolution_clock Clock;

		using seconds = std::chrono::duration<double, std::ratio<1>>;
		auto LastTime = Clock.now();
		double Lag = 0.0;

		bIsRunning = true;
		while (bIsRunning)
		{
			FrameMark

			const double DeltaTime = std::chrono::duration_cast<seconds>(Clock.now() - LastTime).count();
			LastTime = Clock.now();
			Lag += DeltaTime;

			PreUpdate();
			// In this scope we are ticking objects
			{
				bTickingObjects = true;
				// If it has been more than FIXED_UPDATE_S since our last fixed timestep update,
				// we should run a fixed update before running a regular update
				while (Lag >= FIXED_UPDATE_S)
				{
					Lag -= FIXED_UPDATE_S;
					FixedUpdate(FIXED_UPDATE_S);
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
		AppWindow->PollEvents();
		
		AddQueuedTickables();
		ModuleManager::GetInstance().PreUpdate();
	}

	void Engine::PostUpdate()
	{
		ZoneScoped
		ModuleManager::GetInstance().PostUpdate();

		AppWindow->SwapBuffer();
	}

	void Engine::Terminate()
	{
		ZoneScoped
		SCAR_LOG(LogInfo, "Terminating engine");
		ModuleManager::GetInstance().Shutdown();

		ScarDelete(AppWindow);

		bIsInitialized = false;
		VariableUpdateTickables.clear();
		FixedUpdateTickables.clear();

		bIsTerminated = true;
	}

	void Engine::Update(double DeltaTime)
	{
		ZoneScoped
		ModuleManager::GetInstance().Update();
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
			Tickable->FixedTick(DeltaTime);
		}
	}

	void Engine::QueueAddTickable(ITickable* TickableObject)
	{
		TickableQueue.push_back(TickableObject);
	}

	void Engine::RemoveTickable(ITickable* TickableObject)
	{
		// Tickables cannot be removed while objects are being ticked as this would invalidate the iterator
		check(!bTickingObjects);

		if (TickableObject->WantsFixedTimestep())
		{
			const auto It = std::remove(FixedUpdateTickables.begin(), FixedUpdateTickables.end(), TickableObject);
			if (It != FixedUpdateTickables.end())
			{
				FixedUpdateTickables.erase(It, FixedUpdateTickables.end());
			}
		}
		if (TickableObject->WantsVariableTimestep())
		{
			const auto It = std::remove(VariableUpdateTickables.begin(), VariableUpdateTickables.end(), TickableObject);
			if (It != VariableUpdateTickables.end())
			{
				VariableUpdateTickables.erase(It, VariableUpdateTickables.end());
			}
		}
	}

	void Engine::AddQueuedTickables()
	{
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

		if (TickableObject->WantsVariableTimestep())
		{
			VariableUpdateTickables.push_back(TickableObject);
		}
	}
}