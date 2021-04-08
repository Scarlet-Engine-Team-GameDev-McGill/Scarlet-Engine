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

		SceneProxy* GetRenderSceneProxy() const { return Reg.GetSingleton<SceneProxy>(); }

		OnEntityAddedToWorldEvent& GetOnEntityAddedToWorldEvent() { return OnEntityAddedToWorld; }
	public:
		template <typename... ComponentTypes>
		std::tuple<EID, std::add_pointer_t<ComponentTypes>...> CreateEntity(const char* Name)
		{
			ZoneScoped
			const auto EntityProxy = Reg.CreateEntity<ComponentTypes...>();
			SharedPtr<Entity>& Ent = Entities.emplace_back(ScarNew(Entity, Name, std::get<EID>(EntityProxy), this));
			
			OnEntityAddedToWorld.Broadcast(Ent);
			return EntityProxy;
		}

		const Array<SharedPtr<Entity>>& GetEntities() const
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
		double LastDeltaTime;
		Registry Reg;
		Array<SharedPtr<Entity>> Entities;

		OnEntityAddedToWorldEvent OnEntityAddedToWorld;
	};
}
