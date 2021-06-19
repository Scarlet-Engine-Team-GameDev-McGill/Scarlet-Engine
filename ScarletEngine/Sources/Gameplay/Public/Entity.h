#pragma once

#include "Core.h"

namespace ScarletEngine
{    
    class World;

    class Entity
    {
    public:
        Entity(const String& InName, EID InID, World* InOwningWorld);

        const String& GetName() const { return Name; }
        void SetName(const String& InName) { Name = String(InName); }

        EID GetEntityID() const { return ID; }

        World* GetWorld() const { return OwningWorld; }

        // #todo_core: add a way to retrieve components directly from the entity
    protected:
        String Name;
        const EID ID;
        World* const OwningWorld;
    };

    using EntityPtr = SharedPtr<Entity>;
}
