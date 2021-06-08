#pragma once

#include "CoreMinimal.h"
#include "World.h"

namespace ScarletEngine
{
	class ITickable;
	class ISystem;
	class ApplicationWindow;

	// -----------------------------------------------------------------------------------------------------------------

	class Engine
	{
	public:
		Engine();
		virtual ~Engine() {}

		virtual void Initialize();
		void Run();

		/** Signals that the engine should quit on the next update */
		void SignalQuit() { bIsRunning = false; }

		void Update(double DeltaTime);
		void FixedUpdate(double DeltaTime);

		bool IsInitialized() const { return bIsInitialized; }

		const SharedPtr<World>& GetActiveWorld() const { return ActiveWorld; }
		ApplicationWindow* GetApplicationWindow() const { return AppWindow; }
	protected:
		virtual void Terminate();

		/** Called before objects are ticked each frame */
		void PreUpdate();

		/** Called after objects are ticked each frame */
		void PostUpdate();
	protected:
		// Prevent copy/move constructors
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;

		SharedPtr<World> ActiveWorld = nullptr;

		/** Main engine application window */
		ApplicationWindow* AppWindow = nullptr;

		/** True when the engine is initialized */
		uint32_t bIsInitialized : 1 = false;
		/** True when the engine is in the running state */
		uint32_t bIsRunning : 1 = false;
		/** True after the engine has finished running */
		uint32_t bIsTerminated : 1 = false;
		/** True when the engine is in the middle of ticking objects */
		uint32_t bTickingObjects : 1 = false;
		/** Should the engine immediately start all gameplay systems */
		uint32_t bStartGameplaySystemsOnLoad : 1 = true;
	};

	extern Engine* GEngine;
}