#pragma once

#include "Core/Core.h"
#include "ECS/ECS.h"

namespace ScarletEngine
{
	using OnEntityAddedToWorldEvent = Event<const SharedPtr<Entity>&>;

	class World final : public ITickable
	{
	public:
		World();
		~World() {}

		virtual void Initialize() override;

		virtual void Tick(double DeltaTime) override;

		inline double GetDeltaTime() const { return LastDeltaTime; }

		OnEntityAddedToWorldEvent& GetOnEntityAddedToWorldEvent() { return OnEntityAddedToWorld; }
	public:
		template <typename... ComponentTypes>
		std::tuple<SharedPtr<Entity>, std::add_pointer_t<ComponentTypes>...> CreateEntity(const char* Name = "")
		{
			ZoneScoped
			Entities.push_back(MakeShared<Entity>(Name));
			SharedPtr<Entity>& Ent = Entities.back();
			Ent->OwningWorld = this;
			
			auto Ret = Reg.CreateEntity<ComponentTypes...>(*Ent);
			OnEntityAddedToWorld.Broadcast(Ent);
			return std::tuple_cat(std::make_tuple(Ent), Ret);
		}

		/** Register a new system with the world */
		template <typename... SystemSig>
		System<SystemSig...>& AddSystem(const String& Name)
		{
			ZoneScoped
			return *static_cast<System<SystemSig...>*>(Systems.emplace_back(GlobalAllocator<System<SystemSig...>>::New(&Reg, Name)).get());
		}

		auto GetEntities()
		{
			ZoneScoped
			return Entities;
		}

		template <typename ComponentType>
		auto GetComponent(const Entity& Ent)
		{
			ZoneScoped
			return Reg.GetComponent<ComponentType>(Ent.ID);
		}
	private:
		void RunSystems()
		{
			ZoneScoped
			for (const auto& Sys : Systems)
			{
				for (const SharedPtr<Entity>& Ent : Entities)
				{
					Sys->Run(Ent->ID);
				}
			}
		}
	private:
		double LastDeltaTime;
		Registry Reg;

		Array<SharedPtr<Entity>> Entities;
		Array<UniquePtr<ISystem>> Systems;

		OnEntityAddedToWorldEvent OnEntityAddedToWorld;
	};
}
