#pragma once

#include "Core.h"

namespace ScarletEngine
{    
    class World;

    class Entity
    {
    public:
        Entity() : Name(), ID(), OwningWorld(nullptr) {}
        Entity(const String& InName, EID InID, World* InOwningWorld);

        const String& GetName() const { return Name; }
        void SetName(const String& InName) { Name = String(InName); }

        EID GetEntityID() const { return ID; }

        World* GetWorld() const { return OwningWorld; }

        // #todo_core: add a way to retrieve components directly from the entity

        void ChangeWorld(World* NewOwningWorld);

        void Serialize(Archive& Arc) const;

        void Deserialize(Archive& Arc);
    protected:
        String Name;
        EID ID;
        World* OwningWorld;
    };

    using EntityPtr = SharedPtr<Entity>;
}
