#pragma once

#include "Core.h"
#include "ITickable.h"
#include "ECS.h"
#include "SceneProxy.h"
#include "SystemScheduler.h"

namespace ScarletEngine
{
	using OnEntityAddedToWorldEvent = Event<const SharedPtr<Entity>&>;

	class World final : public ITickable
	{
	public:
		World();

		void Initialize();

		virtual void Tick(double DeltaTime) override;
		virtual void FixedTick(double DeltaTime) override;

		virtual bool WantsFixedTimestep() const override { return true; }

		inline double GetDeltaTime() const { return LastDeltaTime; }

		SceneProxy* GetRenderSceneProxy() { return &RenderSceneProxy; }

		OnEntityAddedToWorldEvent& GetOnEntityAddedToWorldEvent() { return OnEntityAddedToWorld; }
	public:
		template <typename... ComponentTypes>
		std::tuple<SharedPtr<Entity>, std::add_pointer_t<ComponentTypes>...> CreateEntity(const char* Name)
		{
			ZoneScoped
			auto EntityProxy = Reg.CreateEntity<ComponentTypes...>(Name);
			SharedPtr<Entity>& Ent = std::get<SharedPtr<Entity>>(EntityProxy);
			Ent->OwningWorld = this;
			OnEntityAddedToWorld.Broadcast(Ent);
			return EntityProxy;
		}

		/** Register a new system with the world */
		template <typename... SystemSig>
		void RegisterSystem(const String& Name)
		{
			ZoneScoped
			//return *static_cast<System<SystemSig...>*>(Systems.emplace_back(ScarNew(System<SystemSig...>, &Reg, Name)).get());
			
		}

		auto GetEntities()
		{
			ZoneScoped
			return Reg.GetEntities();
		}

		template <typename ComponentType>
		auto GetComponent(const Entity& Ent)
		{
			ZoneScoped
			return Reg.GetComponent<ComponentType>(Ent.ID);
		}
	private:
		double LastDeltaTime;
		Registry Reg;

		OnEntityAddedToWorldEvent OnEntityAddedToWorld;

		SceneProxy RenderSceneProxy;
	};
}
