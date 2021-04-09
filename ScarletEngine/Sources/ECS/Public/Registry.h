#pragma once

#include "CoreMinimal.h"
#include "TypeInfo.h"
#include "ComponentContainer.h"

namespace ScarletEngine
{
	template <typename ...Components>
	using ProxyType = std::tuple<EID, std::add_pointer_t<Components>...>;
	
	/** Manages entity-component relationships at a low level. */
	class Registry
	{
	public:
		/* Entity Interface */

		template <typename ...ComponentTypes>
		ProxyType<ComponentTypes...> CreateEntity()
		{
			ZoneScoped
			const EID EntityID = NextAvailableEID++;
			Entities.push_back(EntityID);
			
			return std::make_tuple(EntityID, AddComponent<ComponentTypes>(EntityID)...);
		}
	public:
		/* Component Interface */

		template <typename ComponentType>
		ComponentType* AddComponent(EID EntityID)
		{
			ZoneScoped
			const auto Container = GetOrCreateComponentContainer<ComponentType>();
			check(Container);

			return Container->Add(EntityID);
		}

		template <typename ComponentType>
		bool RemoveComponent(EID EntityID) const
		{
			ZoneScoped
			if (const auto Container = GetComponentContainer<ComponentType>())
			{
				return Container->Remove(EntityID);
			}
			return false;
		}

		template <typename ComponentType>
		ComponentType* AttachComponent(EID EntityID, const ComponentType& Component)
		{
			ZoneScoped
			const auto Container = GetOrCreateComponentContainer<ComponentType>();
			check(Container);
			
			return Container->Attach(EntityID, Component);
		}

		template <typename ComponentType>
		ComponentType* GetComponent(EID EntityID) const
		{
			ZoneScoped
			if (const auto Container = GetComponentContainer<ComponentType>())
			{
				return Container->Get(EntityID);
			}

			return nullptr;
		}

		template <typename ComponentType>
		bool HasComponent(EID EntityID) const
		{
			ZoneScoped
			if (const auto Container = GetComponentContainer<ComponentType>())
			{
				return Container->Has(EntityID);
			}
			return false;
		}

		void DestroyEntity(EID EntityID);

		void SortAll()
		{
			ZoneScoped
			for (const auto& [_, Container] : ComponentContainers)
			{
				Container->Sort();
			}
		}

		template <typename ComponentType>
		ComponentType* GetSingleton() const
		{
			// HACK: Sort of a hacky way to implement singleton components but very performant since it doesn't use any hashmaps
			static ComponentType Instance;
			return &Instance;
		}

		void Clear()
		{
			// Note: this does NOT clear any values for singletons.
			ComponentContainers.clear();
			Entities.clear();
			NextAvailableEID = 1;
		}

		template <typename ...ComponentTypes>
        Array<ProxyType<ComponentTypes...>> GetProxies()
		{
			ZoneScoped
            static_assert(sizeof...(ComponentTypes) > 0, "Missing template argument list");

			// Cache pointers to all component containers in a tuple to access later
			const auto Containers = std::make_tuple(GetOrCreateComponentContainer<std::remove_cv_t<ComponentTypes>>()...);
			
			// Create an array of std::tuples of references to components
			// could probably cache some of this work
			Array<ProxyType<ComponentTypes...>> EntityProxies;
			const size_t Count = std::min({ std::get<ComponentContainer<std::remove_cv_t<ComponentTypes>>*>(Containers)->Count()... });
			EntityProxies.reserve(Count);
			
			if (Count > 0)
			{
				for (const EID Entity : Entities)
				{
					const auto Proxy = std::make_tuple(Entity, std::get<ComponentContainer<std::remove_cv_t<ComponentTypes>>*>(Containers)->Get(Entity)...);
					if ((std::get<std::remove_cv_t<ComponentTypes>*>(Proxy) && ...))
					{
						EntityProxies.emplace_back(Proxy);
					}
				}
			}

			return EntityProxies;
		}

		const Array<EID>& GetEntities() const { return Entities; }
	private:
		template <typename ComponentType>
		ComponentContainer<ComponentType>* GetComponentContainer() const
		{
			ZoneScoped
			if (const auto It = ComponentContainers.find(ComponentTypeID<ComponentType>::Value()); It != ComponentContainers.end())
			{
				return static_cast<ComponentContainer<ComponentType>*>(It->second.get());
			}
			return nullptr;
		}

		template <typename ComponentType>
		ComponentContainer<ComponentType>* GetOrCreateComponentContainer()
		{
			ZoneScoped
			UniquePtr<IComponentContainer>& Container = ComponentContainers[ComponentTypeID<ComponentType>::Value()];
			if (!Container)
			{
				Container = UniquePtr<IComponentContainer>(ScarNew(ComponentContainer<ComponentType>));
			}
			return static_cast<ComponentContainer<ComponentType>*>(Container.get());
		}
	private:
		EID NextAvailableEID = 1;
		UnorderedMap<CTID, UniquePtr<IComponentContainer>> ComponentContainers;
		Array<EID> Entities;
	};
}