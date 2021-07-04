#pragma once

#include "Core.h"

namespace ScarletEngine
{
    class Registry;

    class ComponentTypeRegistry
    {
    public:
        const Array<std::pair<CTID, String>>& GetRegisteredComponents() const { return ComponentTypeNames; }
        void CreateComponentFromTypeID(CTID ComponentTypeID, EID EntityID, Registry* Reg);

        template <typename ComponentType>
        void RegisterComponentType(const Function<void(EID, Registry*)>& Factory)
        {
            ComponentFactories.emplace(ComponentType::StaticTypeID, Factory);
            ComponentTypeNames.emplace_back(std::make_pair(ComponentType::StaticTypeID, ComponentType::StaticTypeName));
        }

        static ComponentTypeRegistry& Get() { static ComponentTypeRegistry Instance; return Instance; } 
    private:
        UnorderedMap<CTID, Function<void(EID, Registry*)>> ComponentFactories;
        Array<std::pair<CTID, String>> ComponentTypeNames;
    };
}
