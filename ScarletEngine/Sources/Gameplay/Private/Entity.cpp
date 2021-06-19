#include "Entity.h"

namespace ScarletEngine
{
    Entity::Entity(const String& InName, EID InID, World* InOwningWorld)
        : Name(InName)
        , ID(InID)
        , OwningWorld(InOwningWorld)
    {
        Name.reserve(64);
    }
}
