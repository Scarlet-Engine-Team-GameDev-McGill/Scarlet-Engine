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

    void Entity::ChangeWorld(World* NewOwningWorld)
    {
        check(NewOwningWorld);
        OwningWorld = NewOwningWorld;
    }

    void Entity::Serialize(BinaryArchive& Arc)
    {
        Arc << Name;
        Arc << ID;
    }

    void Entity::Deserialize(BinaryArchive& Arc)
    {
        Arc >> Name;
        Arc >> ID;
    }
}
