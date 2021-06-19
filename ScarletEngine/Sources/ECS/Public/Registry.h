#pragma once

#include "CoreMinimal.h"
#include "ComponentContainer.h"

namespace ScarletEngine
{
    template <typename ...Components>
    using ProxyType = std::tuple<EID, std::add_pointer_t<Components>...>;

    /** Manages entity-component relationships. */
    class Registry
    {
    public:
        /* Entity Interface */

        /**
         * Create a new entity.
         * Optionally creates a set of components along with the entity which are automatically attached to it.
         *
         * @returns a std::tuple with the entity id followed by a pointer to each of its components.
         */
        template <typename ...ComponentTypes>
        ProxyType<ComponentTypes...> CreateEntity()
        {
            const EID EntityID = NextAvailableEID++;
            Entities.push_back(EntityID);

            (MarkComponentContainerDirty<ComponentTypes>(), ...);

            return std::make_tuple(EntityID, AddComponent<ComponentTypes>(EntityID)...);
        }
    public:
        /* Component Interface */

        /** Add a component of the templated type to an entity */
        template <typename ComponentType>
        ComponentType* AddComponent(EID EntityID)
        {
            const auto Container = GetOrCreateComponentContainer<ComponentType>();
            check(Container);

            MarkComponentContainerDirty<ComponentType>();

            return Container->Add(EntityID);
        }

        /**
         * Removes a component of the templated type from an entity if it exists.
         * @returns true if operation was successful
         */
        template <typename ComponentType>
        bool RemoveComponent(EID EntityID) const
        {
            if (const auto Container = GetComponentContainer<ComponentType>())
            {
                MarkComponentContainerDirty<ComponentType>();
                return Container->Remove(EntityID);
            }
            return false;
        }

        /**
         * Attach an existing component to an entity.
         * @note: The old component will be invalid and changes to it will not be reflected on the entity.
         * @returns the new pointer for that component.
         */
        template <typename ComponentType>
        ComponentType* AttachComponent(EID EntityID, const ComponentType& Component)
        {
            const auto Container = GetOrCreateComponentContainer<ComponentType>();
            check(Container);

            if (!Container->Has(EntityID))
            {
                MarkComponentContainerDirty<ComponentType>();
            }

            return Container->Attach(EntityID, Component);
        }

        /** Get a pointer to the component of the templated type associated with the given entity */
        template <typename ComponentType>
        ComponentType* GetComponent(EID EntityID) const
        {
            if (const auto Container = GetComponentContainer<ComponentType>())
            {
                return Container->Get(EntityID);
            }

            return nullptr;
        }

        /** @returns true if the entity has a component of the templated type */
        template <typename ComponentType>
        bool HasComponent(EID EntityID) const
        {
            if (const auto Container = GetComponentContainer<ComponentType>())
            {
                return Container->Has(EntityID);
            }
            return false;
        }

        /** Destroy an entity along with all its components */
        void DestroyEntity(EID EntityID);

        /** Sort all the component containers */
        void SortAll()
        {
            ZoneScoped
            for (const auto& [_, Container] : ComponentContainers)
            {
                Container->Sort();
            }
        }

        /** @returns a pointer to the singleton instance of the templated component type */
        template <typename ComponentType>
        ComponentType* GetSingleton() const
        {
            // HACK: Sort of a hacky way to implement singleton components but very performant since it doesn't use any hashmaps
            static ComponentType Instance;
            return &Instance;
        }

        /**
         * Clears the registry, deleting all entities and components.
         * @note does not clear the values of any singleton components as these are static.
         */
        void Clear()
        {
            ComponentContainers.clear();
            Entities.clear();
            NextAvailableEID = 1;
        }

        /** @returns an array of entity proxies for the templated component types */
        template <typename ...ComponentTypes>
        const Array<ProxyType<ComponentTypes...>>& GetProxies() const
        {
            static_assert(sizeof...(ComponentTypes) > 0, "Missing template argument list");

            static Array<ProxyType<ComponentTypes...>> EntityProxies;

            const bool bAllClean = (IsComponentContainerClean<std::remove_cv_t<ComponentTypes>>() && ...);

            if (bAllClean)
            {
                return EntityProxies;
            }

            // Cache pointers to all component containers in a tuple to access later
            const auto Containers = std::make_tuple(GetComponentContainer<std::remove_cv_t<ComponentTypes>>()...);
            const bool bAllContainersExist = ((std::get<ComponentContainer<std::remove_cv_t<ComponentTypes>>*>(Containers) != nullptr) && ...);

            if (bAllContainersExist)
            {
                const size_t Count = std::min({ std::get<ComponentContainer<std::remove_cv_t<ComponentTypes>>*>(Containers)->Count()... });
                if (Count > 0)
                {
                    EntityProxies.clear();
                    EntityProxies.reserve(Count);
                    for (const EID Entity : Entities)
                    {
                        const auto Proxy = std::make_tuple(Entity, std::get<ComponentContainer<std::remove_cv_t<ComponentTypes>>*>(Containers)->Get(Entity)...);
                        if (((std::get<std::remove_cv_t<ComponentTypes>*>(Proxy) != nullptr) && ...))
                        {
                            EntityProxies.emplace_back(Proxy);
                        }
                    }
                }
                (DirtyComponentContainers.erase(ComponentTypes::StaticTypeID), ...);
            }

            return EntityProxies;
        }

        /** @returns an entity proxies for the templated component types if possible */
        template <typename ...ComponentTypes>
        std::optional<ProxyType<ComponentTypes...>> GetProxy(EID EntityID) const
        {
            static_assert(sizeof...(ComponentTypes) > 0, "Missing template argument list");

            const auto Containers = std::make_tuple(GetComponentContainer<std::remove_cv_t<ComponentTypes>>()...);
            const bool bAllContainersExist = (std::get<ComponentContainer<std::remove_cv_t<ComponentTypes>>*>(Containers) && ...);

            if (bAllContainersExist)
            {
                const auto Proxy = std::make_tuple(EntityID, std::get<ComponentContainer<std::remove_cv_t<ComponentTypes>>*>(Containers)->Get(EntityID)...);
                // if any one of the components is nullptr, zero out the proxy
                if (((std::get<std::remove_cv_t<ComponentTypes>*>(Proxy) != nullptr) && ...))
                {
                    return Proxy;
                }
            }
            return std::optional<ProxyType<ComponentTypes...>>{};
        }

        /** Mark a component container as dirty such that any proxies that depend on it will be reconstructed next time they are requested. */
        template <typename ComponentType>
        void MarkComponentContainerDirty() const
        {
            DirtyComponentContainers.emplace(ComponentType::StaticTypeID);
        }

        template <typename ComponentType>
        bool IsComponentContainerClean() const
        {
            return !DirtyComponentContainers.contains(ComponentType::StaticTypeID);
        }

        const Array<EID>& GetEntities() const { return Entities; }
    private:
        template <typename ComponentType>
        ComponentContainer<ComponentType>* GetComponentContainer() const
        {
            if (const auto It = ComponentContainers.find(ComponentType::StaticTypeID); It != ComponentContainers.end())
            {
                return static_cast<ComponentContainer<ComponentType>*>(It->second.get());
            }
            return nullptr;
        }

        template <typename ComponentType>
        ComponentContainer<ComponentType>* GetOrCreateComponentContainer()
        {
            UniquePtr<IComponentContainer>& Container = ComponentContainers[ComponentType::StaticTypeID];
            if (!Container)
            {
                Container = UniquePtr<IComponentContainer>(ScarNew(ComponentContainer<ComponentType>, ComponentType::StaticTypeID));
                MarkComponentContainerDirty<ComponentType>();
            }
            return static_cast<ComponentContainer<ComponentType>*>(Container.get());
        }

    private:
        EID NextAvailableEID = 1;
        UnorderedMap<CTID, UniquePtr<IComponentContainer>> ComponentContainers;
        mutable UnorderedSet<CTID> DirtyComponentContainers;
        Array<EID> Entities;
    };
}
