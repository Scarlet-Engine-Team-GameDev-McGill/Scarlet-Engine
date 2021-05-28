#pragma once

#include "Core.h"
#include "ITickable.h"
#include "SceneProxy.h"
#include "SystemScheduler.h"
#include "Entity.h"


namespace ScarletEngine
{
	using OnEntityAddedToWorldEvent = Event<const EntityPtr&>;

	class World final : public ITickable
	{
	public:
		World();
		
		/* ITickable interface */
		virtual void Tick(double DeltaTime) override;
		virtual void FixedTick(double DeltaTime) override;
		virtual bool WantsFixedTimestep() const override { return true; }

		/** Returns the render scene proxy for this world */
		// #todo_rendering: should be managed by the renderer directly
		SceneProxy* GetRenderSceneProxy() const { return Reg.GetSingleton<SceneProxy>(); }

		/** Create a new entity in the world with specified component types */
		template <typename... ComponentTypes>
		std::tuple<EID, std::add_pointer_t<ComponentTypes>...> CreateEntity(const char* Name)
		{
			const auto EntityProxy = Reg.CreateEntity<ComponentTypes...>();
			EntityPtr& Ent = Entities.emplace_back(ScarNew(Entity, Name, std::get<EID>(EntityProxy), this));
			
			OnEntityAddedToWorld.Broadcast(Ent);
			return EntityProxy;
		}

		const Array<EntityPtr>& GetEntities() const { return Entities; }

		template <typename ComponentType>
		ComponentType* GetComponent(const Entity& Ent) const
		{
			return Reg.GetComponent<ComponentType>(Ent.ID);
		}

		template <typename ComponentType>
		ComponentType* AddComponent(const EID Ent)
		{
			return Reg.AddComponent<ComponentType>(Ent);
		}

		OnEntityAddedToWorldEvent& GetOnEntityAddedToWorldEvent() { return OnEntityAddedToWorld; }
	private:
		Registry Reg;
		Array<EntityPtr> Entities;

		OnEntityAddedToWorldEvent OnEntityAddedToWorld;
	};
}
