#include "Core/Engine.h"

#include "Core/CoreUtils.h"

#include <chrono>

#define FIXED_UPDATE_MS 20.0

namespace ScarletEngine
{
	void Engine::Initialize()
	{
		Logger::Get().SetLogFile("Log.txt");

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

			// If it has been more than FIXED_UPDATE_MS since our last fixed timestep update,
			// we should run a fixed update before running a regular update
			while (Lag >= FIXED_UPDATE_MS)
			{
				Lag -= FIXED_UPDATE_MS;
				FixedUpdate(FIXED_UPDATE_MS);
			}

			Update(DeltaTime);
		}

		Terminate();
	}

	void Engine::Terminate()
	{

	}

	void Engine::Update(double) const
	{

	}

	void Engine::FixedUpdate(double) const
	{

	}
}