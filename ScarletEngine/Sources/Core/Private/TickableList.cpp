#include "TickableList.h"

#include "ITickable.h"

namespace ScarletEngine
{
    void TickableList::QueueAddTickable(ITickable* TickableObject)
    {
        TickableQueue.push_back(TickableObject);
    }

    void TickableList::RemoveTickable(ITickable* TickableObject)
    {
        // replace the entry with a nullptr to prevent invalidating the iterator

        if (TickableObject->WantsFixedTimestep())
        {
            const auto It = std::remove(FixedUpdateTickables.begin(), FixedUpdateTickables.end(), TickableObject);
            if (It != FixedUpdateTickables.end())
            {
                FixedUpdateTickables.emplace(It, nullptr);
            }
        }
        if (TickableObject->WantsVariableTimestep())
        {
            const auto It = std::remove(VariableUpdateTickables.begin(), VariableUpdateTickables.end(), TickableObject);
            if (It != VariableUpdateTickables.end())
            {
                VariableUpdateTickables.emplace(It, nullptr);
            }
        }
    }

    void TickableList::Reset()
    {
        VariableUpdateTickables.clear();
        FixedUpdateTickables.clear();
    }

    void TickableList::Update(double DeltaTime)
    {
        for (ITickable* Tickable : VariableUpdateTickables)
        {
            if (Tickable != nullptr)
            {
                Tickable->Tick(DeltaTime);
            }
        }
    }

    void TickableList::FixedUpdate(double DeltaTime)
    {
        for (ITickable* Tickable : FixedUpdateTickables)
        {
            if (Tickable != nullptr)
            {
                Tickable->FixedTick(DeltaTime);
            }
        }
    }

    void TickableList::AddQueuedTickables()
    {
        // Add any new tickables which may have been enqueued during the last frame
        for (const auto TickableToAdd : TickableQueue)
        {
            AddTickable(TickableToAdd);
        }

        TickableQueue.clear();
    }

    void TickableList::Cleanup()
    {
        VariableUpdateTickables.erase(
            std::remove(VariableUpdateTickables.begin(), VariableUpdateTickables.end(), nullptr),
            VariableUpdateTickables.end());

        FixedUpdateTickables.erase(
            std::remove(FixedUpdateTickables.begin(), FixedUpdateTickables.end(), nullptr),
            FixedUpdateTickables.end());
    }

    void TickableList::AddTickable(ITickable* TickableObject)
    {
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