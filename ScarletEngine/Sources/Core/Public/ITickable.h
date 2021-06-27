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
        friend class TickableList;
    public:
        /** Adds the object to the engine tickable list */
        ITickable();

        /** Removes the object from the engine tickable list */
        virtual ~ITickable();

        /** Called each frame by the Engine with the delta time since the last Tick. */
        virtual void Tick(double /* DeltaTime */) {}

        /** Called at a fixed-interval with a fixed timestep. */
        virtual void FixedTick(double /* DeltaTime */) {}

    protected:
        /** If object wants to tick on the variable timestep update. */
        bool bWantsVariableTimestep = true;

        /** If object wants to tick on the fixed timestep update. */
        bool bWantsFixedTimestep = false;
    };
}
