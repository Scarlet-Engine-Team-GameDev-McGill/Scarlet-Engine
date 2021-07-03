#include "ComponentTypeRegistry.h"

namespace ScarletEngine
{
    void ComponentTypeRegistry::CreateComponentFromTypeID(CTID ComponentTypeID, EID EntityID, Registry* Reg)
    {
        if (const auto It = ComponentFactories.find(ComponentTypeID); It != ComponentFactories.end())
        {
            It->second(EntityID, Reg);
        }
    }
}
