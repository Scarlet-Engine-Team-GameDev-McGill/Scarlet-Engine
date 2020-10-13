#include "World.h"

#include "Renderer/Renderer.h"
#include "Renderer/StaticMeshComponent.h"

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
			.Each([ProxyPtr](const EID, const Transform& Trans, const StaticMeshComponent& SMC)
				{
					ProxyPtr->AddSMC(Trans, SMC);
				});
	}

	void World::Tick(double DeltaTime)
	{
		ZoneScoped
		LastDeltaTime = DeltaTime;
		RenderSceneProxy.Reset();
		RunSystems();
	}

}