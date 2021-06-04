#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
    class ITickable;

    // -----------------------------------------------------------------------------------------------------------------

    class TickableList
    {
    public:
        /** Enqueue the addition of a new tickable object for the next frame */
        void QueueAddTickable(ITickable* TickableObject);

        /** Remove a tickable object. This must be immediate as it will only be called on destruction of an ITickable */
        void RemoveTickable(ITickable* TickableObject);

        /** Moves queued tickables into the list of objects to tick on the next frame */
        void AddQueuedTickables();

        /** Cleaned up the tickable list */
        void Cleanup();

        /** Resets the list of known tickables. */
        void Reset();

        void Update(double DeltaTime);
        void FixedUpdate(double DeltaTime);

        static TickableList& Get() { static TickableList Instance; return Instance; }
    private:        
        void AddTickable(ITickable* TickableToAdd);
    private:
        /** List of tickables to update on a fixed-timestep */
        Array<ITickable*> VariableUpdateTickables;
        
        /** List of tickables to update on a variable-timestep */
        Array<ITickable*> FixedUpdateTickables;
        
        /** Queue of tickables to add at the beginning of the next frame */
        Array<ITickable*> TickableQueue;
    };
}