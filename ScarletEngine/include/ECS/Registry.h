#pragma once
#include <tuple>
#include <map>
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>

#include "TypeInfo.h"
#include "System.h"

#define INVALID_EID UINT64_MAX

namespace ScarletEngine
{
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
				const size_t Index = Components.size();
				Map[EntityID] = Index;
				return &Components.emplace_back(T());
			}

			T* Get(EID EntityID)
			{
				if (Has(EntityID))
				{
					return &Components[Map.at(EntityID)];
				}
				return nullptr;
			}

			virtual bool Has(EID EntityID) const override
			{
				return Map.find(EntityID) != Map.end();
			}

			virtual bool Remove(EID EntityID) override
			{
				const size_t IndexToRemove = Map.at(EntityID);
				const T* BackElement = &Components.back();
				const EID BackOwner = FindOwner(BackElement);

				if (BackOwner == INVALID_EID)
				{
					return false;
				}

				Components[IndexToRemove] = *BackElement;
				Map[BackOwner] = IndexToRemove;
				Components.pop_back();
				Map.erase(EntityID);

				return true;
			}

			T* Attach(EID EntityID, const T& Component)
			{
				if (!Has(EntityID))
				{
					const size_t Index = Components.size();
					Map[EntityID] = Index;
					return &Components.emplace_back(Component);
				}

				const size_t Index = Map.at(EntityID);
				Components[Index] = Component;
				return &Components.at(Index);
			}

			virtual void Sort() override
			{
				std::vector<size_t> Ids(Map.size());

				size_t NextIndex = 0;
				for (auto& Pair : Map)
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
				for (const auto& Pair : Map)
				{
					if (&Components[Pair.second] == Component) return Pair.first;
				}
				return INVALID_EID;
			}
		private:
			/** Maps EID to index in component map */
			std::map<EID, size_t> Map;
			/** Stores components */
			std::vector<T> Components;
		};
	public:
		/* Entity Interface */

		template <typename ...Ts>
		std::tuple<EID, std::add_pointer_t<Ts>...> CreateEntity()
		{
			EID EntityID = NextAvailableEID++;
			Entities.push_back(EntityID);
			return std::make_tuple(EntityID, AddComponent<Ts>(EntityID)...);
		}

		const std::vector<EID>& GetEntities() const { return Entities; }
	public:
		/* Component Interface */

		template <typename T>
		T* AddComponent(EID EntityID)
		{
			auto Container = GetOrCreateComponentContainer<T>();
			check(Container);
			check(!Container->Has(EntityID));

			return Container->Add(EntityID);
		}

		template <typename T>
		bool RemoveComponent(EID EntityID) const
		{
			if (auto Container = GetComponentContainer<T>())
			{
				return Container->Remove(EntityID);
			}
			return false;
		}

		template <typename T>
		T* AttachComponent(EID EntityID, const T& Component) const
		{
			if (auto Container = GetComponentContainer<T>())
			{
				return Container->Attach(EntityID, Component);
			}
			return nullptr;
		}

		template <typename T>
		T* GetComponent(EID EntityID) const
		{
			if (auto Container = GetComponentContainer<T>())
			{
				return Container->Get(EntityID);
			}

			return nullptr;
		}

		template <typename T>
		bool HasComponent(EID EntityID) const
		{
			if (auto Container = GetComponentContainer<T>())
			{
				return Container->Has(EntityID);
			}
			return false;
		}

		void DestroyEntity(EID EntityID);

		void SortAll()
		{
			for (auto& Container : ComponentContainers)
			{
				Container.second->Sort();
			}
		}
	private:
		template <typename T>
		ComponentContainer<T>* GetComponentContainer() const
		{
			if (ComponentContainers.find(ComponentTypeID<T>::Value()) != ComponentContainers.end())
			{
				return static_cast<ComponentContainer<T>*>(ComponentContainers.at(ComponentTypeID<T>::Value()).get());
			}
			return nullptr;
		}

		template <typename T>
		ComponentContainer<T>* GetOrCreateComponentContainer()
		{
			ComponentContainer<T>* Container = GetComponentContainer<T>();
			if (!Container)
			{
				Container = new ComponentContainer<T>();
				ComponentContainers[ComponentTypeID<T>::Value()] = std::unique_ptr<IComponentContainer>(Container);
			}
			return Container;
		}
	private:
		EID NextAvailableEID = 0;

		std::vector<EID> Entities;
		std::unordered_map<CTID, std::unique_ptr<IComponentContainer>> ComponentContainers;
	};
}