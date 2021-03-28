#pragma once

#include "CoreMinimal.h"
#include "TypeInfo.h"
#include "Entity.h"

namespace ScarletEngine
{
	/** Manages entity-component relationships */
	class Registry
	{
		struct IComponentContainer
		{
			virtual ~IComponentContainer() {}

			virtual bool Has(EID EntityID) const = 0;
			virtual bool Remove(EID EntityID) = 0;
			virtual void Sort() = 0;
		};

		template <typename T>
		struct ComponentContainer : public IComponentContainer
		{
		public:
			T* Add(EID EntityID)
			{
				ZoneScoped
				const size_t Index = Components.size();
				EntityMap[EntityID] = Index;
				return &Components.emplace_back(T());
			}

			T* Get(EID EntityID)
			{
				ZoneScoped
				if (Has(EntityID))
				{
					return &Components[EntityMap.at(EntityID)];
				}
				return nullptr;
			}

			virtual bool Has(EID EntityID) const override
			{
				ZoneScoped
				return EntityMap.find(EntityID) != EntityMap.end();
			}

			virtual bool Remove(EID EntityID) override
			{
				ZoneScoped
				const size_t IndexToRemove = EntityMap.at(EntityID);
				const T* BackElement = &Components.back();
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

			T* Attach(EID EntityID, const T& Component)
			{
				ZoneScoped
				if (!Has(EntityID))
				{
					const size_t Index = Components.size();
					EntityMap[EntityID] = Index;
					return &Components.emplace_back(Component);
				}

				const size_t Index = EntityMap.at(EntityID);
				Components[Index] = Component;
				return &Components.at(Index);
			}

			virtual void Sort() override
			{
				ZoneScoped
				Array<size_t> Ids(EntityMap.size());

				size_t NextIndex = 0;
				for (auto& Pair : EntityMap)
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
			/** Warning: Potentially slow! */
			EID FindOwner(const T* Component) const
			{
				ZoneScoped
				for (const auto& Pair : EntityMap)
				{
					if (&Components[Pair.second] == Component) return Pair.first;
				}
				return INVALID_EID;
			}
		private:
			/** Maps EID to index in component map */
			Map<EID, size_t> EntityMap;
			/** Stores components */
			Array<T> Components;
		};
	public:
		/* Entity Interface */

		template <typename ...Ts>
		std::tuple<SharedPtr<Entity>, std::add_pointer_t<Ts>...> CreateEntity(const char* Name)
		{
			ZoneScoped
			check(Name != nullptr);
			
			Entities.push_back(MakeShared<Entity>(Name));
			SharedPtr<Entity>& Ent = Entities.back();
			Ent->ID = NextAvailableEID++;
			
			return std::make_tuple(Ent, AddComponent<Ts>(Ent->ID)...);
		}
	public:
		/* Component Interface */

		template <typename T>
		T* AddComponent(EID EntityID)
		{
			ZoneScoped
			auto Container = GetOrCreateComponentContainer<T>();
			check(Container);
			check(!Container->Has(EntityID));

			return Container->Add(EntityID);
		}

		template <typename T>
		bool RemoveComponent(EID EntityID) const
		{
			ZoneScoped
			if (auto Container = GetComponentContainer<T>())
			{
				return Container->Remove(EntityID);
			}
			return false;
		}

		template <typename T>
		T* AttachComponent(EID EntityID, const T& Component)
		{
			ZoneScoped
			if (auto Container = GetOrCreateComponentContainer<T>())
			{
				return Container->Attach(EntityID, Component);
			}
			return nullptr;
		}

		template <typename T>
		T* GetComponent(EID EntityID) const
		{
			ZoneScoped
			if (auto Container = GetComponentContainer<T>())
			{
				return Container->Get(EntityID);
			}

			return nullptr;
		}

		template <typename T>
		bool HasComponent(EID EntityID) const
		{
			ZoneScoped
			if (auto Container = GetComponentContainer<T>())
			{
				return Container->Has(EntityID);
			}
			return false;
		}

		void DestroyEntity(EID EntityID);

		void SortAll()
		{
			ZoneScoped
			for (auto& Container : ComponentContainers)
			{
				Container.second->Sort();
			}
		}

		template <typename T>
		T* GetSingleton() const
		{
			static T Instance;
			return &Instance;
		}

		const Array<SharedPtr<Entity>>& GetEntities() const { return Entities; }
	private:
		template <typename T>
		ComponentContainer<T>* GetComponentContainer() const
		{
			ZoneScoped
			if (ComponentContainers.find(ComponentTypeID<T>::Value()) != ComponentContainers.end())
			{
				return static_cast<ComponentContainer<T>*>(ComponentContainers.at(ComponentTypeID<T>::Value()).get());
			}
			return nullptr;
		}

		template <typename T>
		ComponentContainer<T>* GetOrCreateComponentContainer()
		{
			ZoneScoped
			ComponentContainer<T>* Container = GetComponentContainer<T>();
			if (!Container)
			{
				Container = ScarNew(ComponentContainer<T>);
				ComponentContainers[ComponentTypeID<T>::Value()] = UniquePtr<IComponentContainer>(Container);
			}
			return Container;
		}
	private:
		EID NextAvailableEID = 1;
		UnorderedMap<CTID, UniquePtr<IComponentContainer>> ComponentContainers;
		Array<SharedPtr<Entity>> Entities;
	};
}