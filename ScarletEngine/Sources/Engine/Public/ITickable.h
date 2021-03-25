#pragma once

namespace ScarletEngine
{
	/** 
	 * Base class for all tickable objects.
	 * When a tickable object is created, it is automatically tracked by the engine
	 * and ticked on every frame.
	 */
	class ITickable
	{
	public:
		/** Adds the object to the engine tickable list */
		ITickable();

		/** Removes the object from the engine tickable list */
		virtual ~ITickable();

		/** Called each frame by the Engine with the delta time since the last Tick. */
		virtual void Tick(double /* DeltaTime */) {}

		/** Called at a fixed-interval with a fixed timestep. */
		virtual void FixedTick(double /* DeltaTime */) {}

		virtual bool WantsVariableTimestep() const { return true; }

		/** Override if object wants a fixed timestep rather than variable */
		virtual bool WantsFixedTimestep() const { return false; }
	};
}