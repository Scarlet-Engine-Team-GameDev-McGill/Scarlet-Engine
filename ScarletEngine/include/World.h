#pragma once

#include "Core/Core.h"
#include "ECS/ECS.h"

#include <memory>
#include <tuple>
#include <vector>

namespace ScarletEngine
{
	using OnEntityAddedToWorldEvent = Event<const std::shared_ptr<Entity>&>;

	class World : public ITickable
	{
	public:
		World();

		virtual void Initialize() override;

		virtual void Tick(double DeltaTime) override;

		inline double GetDeltaTime() const { return LastDeltaTime; }

		OnEntityAddedToWorldEvent& GetOnEntityAddedToWorldEvent() { return OnEntityAddedToWorld; }
	public:
		template <typename... ComponentTypes>
		std::tuple<std::shared_ptr<Entity>, std::add_pointer_t<ComponentTypes>...> CreateEntity(const char* Name = "")
		{
			auto& Ent = Entities.emplace_back(new Entity(Name));
			Ent->OwningWorld = this;
			
			auto Ret = Reg.CreateEntity<ComponentTypes...>(*Ent);
			OnEntityAddedToWorld.Broadcast(Ent);
			return std::tuple_cat(std::make_tuple(Ent), Ret);
		}

		/** Register a new system with the world */
		template <typename... SystemSig>
		void AddSystem(const typename System<SystemSig...>::ForEachFunctionType ForEach)
		{
			Systems.emplace_back(new System<SystemSig...>(&Reg, ForEach));
		}

		auto GetEntities()
		{
			return Entities;
		}

		template <typename ComponentType>
		auto GetComponent(const Entity& Ent)
		{
			return Reg.GetComponent<ComponentType>(Ent.ID);
		}
	private:
		void RunSystems()
		{
			for (const auto& Sys : Systems)
			{
				for (const std::shared_ptr<Entity>& Ent : Entities)
				{
					Sys->Run(Ent->ID);
				}
			}
		}
	private:
		double LastDeltaTime;
		Registry Reg;

		std::vector<std::shared_ptr<Entity>> Entities;
		std::vector<std::unique_ptr<ISystem>> Systems;

		OnEntityAddedToWorldEvent OnEntityAddedToWorld;
	};
}
