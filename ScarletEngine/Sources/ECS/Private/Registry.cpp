#include "Registry.h"

namespace ScarletEngine
{
    void Registry::DestroyEntity(EID EntityID)
    {
        for (auto& Pair : ComponentContainers)
        {
            Pair.second->Remove(EntityID);
        }
    }

    void Registry::MarkComponentContainerDirty_Impl(CTID DirtyContainerID) const
    {
        DirtyComponentContainers.emplace(DirtyContainerID);
    }
}
