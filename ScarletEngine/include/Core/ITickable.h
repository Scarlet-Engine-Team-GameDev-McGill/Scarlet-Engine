#pragma once

namespace ScarletEngine
{
	/** Base class for all tickable objects */
	class ITickable
	{
	public:
		/** Adds the object to the engine tickable list */
		ITickable();

		/** Removes the object from the engine tickable list */
		~ITickable();

		virtual void Initialize() {}

		virtual void Tick(double DeltaTime) = 0;

		virtual void Terminate() {}

		/** Override if object wants a fixed timestep rather than variable */
		virtual bool WantsFixedTimestep() const { return false; }
	};
}