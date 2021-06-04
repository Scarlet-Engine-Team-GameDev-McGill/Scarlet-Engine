#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
	class ITickable;
	class ApplicationWindow;

	// -----------------------------------------------------------------------------------------------------------------

	class Engine
	{
	public:
		Engine();

		void Initialize();
		void Terminate();
		void Run();

		/** Signals that the engine should quit on the next update */
		void SignalQuit() { bIsRunning = false; }

		void Update(double DeltaTime);
		void FixedUpdate(double DeltaTime);

		bool IsInitialized() const { return bIsInitialized; }

		ApplicationWindow* GetApplicationWindow() const { return AppWindow; }
	private:
		/** Called before objects are ticked each frame */
		void PreUpdate();
		/** Called after objects are ticked each frame */
		void PostUpdate();
	private:
		// Prevent copy/move constructors
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;

		/** Main engine application window */
		ApplicationWindow* AppWindow = nullptr;

		/** True when the engine is initialized */
		uint32_t bIsInitialized : 1;
		/** True when the engine is in the running state */
		uint32_t bIsRunning : 1;
		/** True after the engine has finished running */
		uint32_t bIsTerminated : 1;
		/** True when the engine is in the middle of ticking objects */
		uint32_t bTickingObjects : 1;
	};

	extern UniquePtr<Engine> GEngine;
}