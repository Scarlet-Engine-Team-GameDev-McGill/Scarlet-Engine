#pragma once

#include "CoreMinimal.h"
#include "TypeInfo.h"
#include "ComponentContainer.h"

namespace ScarletEngine
{
	/** Manages entity-component relationships at a low level. */
	class Registry
	{
	public:
		/* Entity Interface */

		template <typename ...Ts>
		std::tuple<EID, std::add_pointer_t<Ts>...> CreateEntity()
		{
			ZoneScoped
			EID EntityID = NextAvailableEID++;
			Entities.push_back(EntityID);
			
			return std::make_tuple(EntityID, AddComponent<Ts>(EntityID)...);
		}
	public:
		/* Component Interface */

		template <typename T>
		T* AddComponent(EID EntityID)
		{
			ZoneScoped
			const auto Container = GetOrCreateComponentContainer<T>();
			check(Container);

			return Container->Add(EntityID);
		}

		template <typename T>
		bool RemoveComponent(EID EntityID) const
		{
			ZoneScoped
			if (const auto Container = GetComponentContainer<T>())
			{
				return Container->Remove(EntityID);
			}
			return false;
		}
		
		template <typename T>
		T* AttachComponent(EID EntityID, const T& Component)
		{
			ZoneScoped
			const auto Container = GetOrCreateComponentContainer<T>();
			check(Container);
			
			return Container->Attach(EntityID, Component);
		}

		template <typename T>
		T* GetComponent(EID EntityID) const
		{
			ZoneScoped
			if (const auto Container = GetComponentContainer<T>())
			{
				return Container->Get(EntityID);
			}

			return nullptr;
		}

		template <typename T>
		bool HasComponent(EID EntityID) const
		{
			ZoneScoped
			if (const auto Container = GetComponentContainer<T>())
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

		template <typename T>
		T* GetSingleton() const
		{
			// HACK: Sort of a hacky way to implement singleton components but very performant since it doesn't use any hashmaps
			static T Instance;
			return &Instance;
		}

		void Clear()
		{
			// Note: this does NOT clear any values for singletons.
			ComponentContainers.clear();
			Entities.clear();
			NextAvailableEID = 1;
		}

		const Array<EID>& GetEntities() const { return Entities; }
	private:
		template <typename T>
		ComponentContainer<T>* GetComponentContainer() const
		{
			ZoneScoped
			if (const auto It = ComponentContainers.find(ComponentTypeID<T>::Value()); It != ComponentContainers.end())
			{
				return static_cast<ComponentContainer<T>*>(It->second.get());
			}
			return nullptr;
		}

		template <typename T>
		ComponentContainer<T>* GetOrCreateComponentContainer()
		{
			ZoneScoped
			UniquePtr<IComponentContainer>& Container = ComponentContainers[ComponentTypeID<T>::Value()];
			if (!Container)
			{
				Container = UniquePtr<IComponentContainer>(ScarNew(ComponentContainer<T>));
			}
			return static_cast<ComponentContainer<T>*>(Container.get());
		}
	private:
		EID NextAvailableEID = 1;
		UnorderedMap<CTID, UniquePtr<IComponentContainer>> ComponentContainers;
		Array<EID> Entities;
	};
}