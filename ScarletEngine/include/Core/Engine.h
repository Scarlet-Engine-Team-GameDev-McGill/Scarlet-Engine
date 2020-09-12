#pragma once

#include <vector>
#include <mutex>

namespace ScarletEngine
{
	class ITickable;

	class Engine
	{
	public:
		void Initialize();
		void Run();

		void SignalQuit() { bIsRunning = false; }

		void Update(double DeltaTime);
		void FixedUpdate(double DeltaTime);

		bool IsInitialized() const { return bIsInitialized; }

		/** Enqueue the addition of a new tickable object for the next frame */
		void QueueAddTickable(ITickable* TickableObject);
		/** Remove a tickable object. This must be immediate as it will only be called on destruction of an ITickable */
		void RemoveTickable(ITickable* TickableObject);

		/** Return a reference to the global Engine object */
		static Engine& Get() { static Engine Instance; return Instance; }
	private:
		void Terminate();

		/** Called before objects are ticked each frame */
		void PreUpdate();
		/** Called after objects are ticked each frame */
		void PostUpdate();
		
		/** Moves queued tickables into the list of objects to tick on the next frame */
		void AddQueuedTickables();
		void AddTickable(ITickable* TickableToAdd);
	private:
		// Prevent instantiations of this class and disable copy/move constructors
		Engine();
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		
		/** List of tickables to update on a fixed-timestep */
		std::vector<ITickable*> VariableUpdateTickables;
		/** List of tickables to update on a variable-timestep */
		std::vector<ITickable*> FixedUpdateTickables;
		/** Queue of tickables to add at the beginning of the next frame */
		std::vector<ITickable*> TickableQueue;
		/** Used to lock the TickableQueue */
		std::mutex TickableQueueMutex;

		/** True when the engine is initialized */
		uint32_t bIsInitialized : 1;
		/** True when the engine is in the running state */
		uint32_t bIsRunning : 1;
		/** True after the engine has finished running */
		uint32_t bIsTerminated : 1;
		/** True when the engine is in the middle of ticking objects */
		uint32_t bTickingObjects : 1;
	};
}