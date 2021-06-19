#pragma once

#include "Core.h"
#include "Components/IComponent.h"

namespace ScarletEngine
{
    class IComponentContainer
    {
    public:
        IComponentContainer(CTID InComponentTypeID) : ComponentTypeID(InComponentTypeID) {}
        virtual ~IComponentContainer() {}

        virtual bool Has(EID EntityID) const = 0;
        virtual bool Remove(EID EntityID) = 0;
        virtual void Sort() = 0;

        CTID GetComponentTypeID() const { return ComponentTypeID; }
    protected:
        const CTID ComponentTypeID;
    };

    template <typename ComponentType>
    class ComponentContainer final : public IComponentContainer
    {
        static_assert(std::is_base_of_v<IComponent, ComponentType>, "Invalid component type! Must derive from IComponent");
    public:
        ComponentContainer(CTID InComponentTypeID) : IComponentContainer(InComponentTypeID) {}

        /** Add a component to a specified entity. Returns a pointer to the newly created component */
        inline ComponentType* Add(EID EntityID)
        {
            const size_t Index = Components.size();
            EntityMap[EntityID] = Index;
            return &Components.emplace_back();
        }

        /** Get a pointer to a component for a specified entity, if it exists. Returns nullptr otherwise */
        NODISCARD inline ComponentType* Get(EID EntityID)
        {
            if (auto Index = Find(EntityID))
            {
                return &Components[Index.value()];
            }
            return nullptr;
        }

        /** Check if an entity has a component of this type */
        inline virtual bool Has(EID EntityID) const override
        {
            return EntityMap.find(EntityID) != EntityMap.end();
        }

        /** Returns the number of components stored in this container */
        inline size_t Count() const { return Components.size(); }

        /** Attempts to remove a component from an entity. Returns true if successful, false otherwise. */
        inline virtual bool Remove(EID EntityID) override
        {
            if (!Has(EntityID))
            {
                return false;
            }

            const size_t IndexToRemove = EntityMap.at(EntityID);
            const ComponentType* BackElement = &Components.back();
            const EID BackOwner = FindOwner(BackElement);

            if (BackOwner == INVALID_EID)
            {
                return false;
            }

            Components[IndexToRemove] = *BackElement;
            EntityMap[BackOwner] = IndexToRemove;
            Components.pop_back();
            EntityMap.erase(EntityID);

            return true;
        }

        /**
         * Attach an existing instance of a component to the specified entity.
         * @note: The old component will be invalid and changes to it will not be reflected on the entity.
         * @returns the new pointer for that component.
         */
        inline ComponentType* Attach(EID EntityID, const ComponentType& Component)
        {
            if (auto Index = Find(EntityID))
            {
                const size_t IndexVal = Index.value();
                Components[IndexVal] = Component;
                return &Components.at(IndexVal);
            }

            const size_t Index = Components.size();
            EntityMap[EntityID] = Index;
            return &Components.emplace_back(Component);
        }

        /** Sort the component container such that it is in increasing entity id order. */
        inline virtual void Sort() override
        {
            ZoneScopedN("Sort Component Container")
            Array<size_t> Ids(EntityMap.size());

            for (size_t NextIndex = 0; auto& Pair : EntityMap)
            {
                Ids[Pair.second] = NextIndex;
                Pair.second = NextIndex;
                NextIndex++;
            }

            for (size_t i = 0; i < Ids.size(); ++i)
            {
                while (Ids[i] != i)
                {
                    const size_t IndexToSwap = Ids[i];
                    std::swap(Components[i], Components[IndexToSwap]);
                    std::swap(Ids[i], Ids[IndexToSwap]);
                }
            }
        }
    private:
        inline std::optional<size_t> Find(EID EntityID) const
        {
            const auto It = EntityMap.find(EntityID);
            return It != EntityMap.end() ? It->second : std::optional<size_t>{};
        }

        /* Slow! Requires searching through the entity map by value */
        EID FindOwner(const ComponentType* Component) const
        {
            if (Component < Components.data() || Component > Components.data() + Components.size())
            {
                return INVALID_EID;
            }

            const size_t Index = Component - Components.data();
            EID Result = INVALID_EID;
            const auto It = std::find_if(EntityMap.begin(), EntityMap.end(), [Index](const auto& Pair)
            {
                return Pair.second == Index;
            });

            if (It != EntityMap.end())
            {
                Result = It->first;
                check(&Components[EntityMap.at(Result)] == Component);
            }

            return Result;
        }
    private:
        /** Maps EID to index in component map */
        Map<EID, size_t> EntityMap;
        /** Stores components */
        Array<ComponentType> Components;
    };
}
