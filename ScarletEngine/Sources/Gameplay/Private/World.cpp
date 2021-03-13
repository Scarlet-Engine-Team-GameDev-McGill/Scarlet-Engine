#include "World.h"
#include "StaticMeshComponent.h"

namespace ScarletEngine
{
	World::World()
		: LastDeltaTime()
	{
		ZoneScoped
	}

	void World::Initialize()
	{
		ZoneScoped

		SceneProxy* ProxyPtr = &RenderSceneProxy;
		AddSystem<const Transform, const StaticMeshComponent>("Draw Static Meshes")
			.Each([ProxyPtr](double, const EID, const Transform& Trans, const StaticMeshComponent& SMC)
				{
					ProxyPtr->DrawSMC(Trans, SMC);
				});
	}

	void World::Tick(double DeltaTime)
	{
		ZoneScoped
		LastDeltaTime = DeltaTime;
		RenderSceneProxy.Reset();
		RunSystems(DeltaTime);
	}

	void World::FixedTick(double DeltaTime)
	{
		RunFixedSystem(DeltaTime);
	}

}