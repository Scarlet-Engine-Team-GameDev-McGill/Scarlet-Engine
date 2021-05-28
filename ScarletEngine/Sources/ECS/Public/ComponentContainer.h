#pragma once
#include "Core.h"
#include "TypeInfo.h"

namespace ScarletEngine
{
    struct IComponentContainer
	{
		virtual ~IComponentContainer() {}

		virtual bool Has(EID EntityID) const = 0;
		virtual bool Remove(EID EntityID) = 0;
		virtual void Sort() = 0;
	};

	template <typename ComponentType>
	struct ComponentContainer : public IComponentContainer
	{
	public:
		inline ComponentType* Add(EID EntityID)
		{
			const size_t Index = Components.size();
			EntityMap[EntityID] = Index;
			return &Components.emplace_back();
		}

		inline ComponentType* Get(EID EntityID)
		{
			if (auto Index = Find(EntityID))
			{
				return &Components[Index.value()];
			}
			return nullptr;
		}

		inline virtual bool Has(EID EntityID) const override
		{
			return EntityMap.find(EntityID) != EntityMap.end();
		}

		inline size_t Count() const { return Components.size(); }
		
		inline virtual bool Remove(EID EntityID) override
		{
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

		inline virtual void Sort() override
		{
			ZoneScopedN("Sort Component Container")
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
		inline std::optional<size_t> Find(EID EntityID) const
		{
			const auto It = EntityMap.find(EntityID);
			return It != EntityMap.end() ? It->second : std::optional<size_t>{};
		}
		
		/** Warning: Potentially slow! */
		EID FindOwner(const ComponentType* Component) const
		{
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
		Array<ComponentType> Components;
	};
}
