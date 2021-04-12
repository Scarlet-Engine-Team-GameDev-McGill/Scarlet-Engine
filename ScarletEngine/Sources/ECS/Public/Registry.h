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
			
			(MarkComponentContainerDirty<ComponentTypes>(), ...);
			
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
			
			MarkComponentContainerDirty<ComponentType>();

			return Container->Add(EntityID);
		}

		template <typename ComponentType>
		bool RemoveComponent(EID EntityID) const
		{
			ZoneScoped
			if (const auto Container = GetComponentContainer<ComponentType>())
			{
				MarkComponentContainerDirty<ComponentType>();
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

			if (!Container->Has(EntityID))
			{
				MarkComponentContainerDirty<ComponentType>();
			}
			
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
		const Array<ProxyType<ComponentTypes...>>& GetProxies() const
		{
			ZoneScoped
			static_assert(sizeof...(ComponentTypes) > 0, "Missing template argument list");

			static Array<ProxyType<ComponentTypes...>> EntityProxies;

			const bool bAllClean = (IsComponentContainerClean<std::remove_cv_t<ComponentTypes>>() && ...);

			if (bAllClean) return EntityProxies;
			
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
				(DirtyComponentContainers.erase(ComponentTypeID<ComponentTypes>::Value()), ...);
			}

			return EntityProxies;
		}

		template <typename ...ComponentTypes>
		std::optional<ProxyType<ComponentTypes...>> GetProxy(EID EntityID) const
		{
			ZoneScoped
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

		template <typename ComponentType>
		void MarkComponentContainerDirty() const
		{
			DirtyComponentContainers.emplace(ComponentTypeID<std::remove_cv_t<ComponentType>>::Value());
		}

		template <typename ComponentType>
		bool IsComponentContainerClean() const
		{
			return !DirtyComponentContainers.contains(ComponentTypeID<std::remove_cv_t<ComponentType>>::Value());
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