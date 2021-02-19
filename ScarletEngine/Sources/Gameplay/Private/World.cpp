#include "World.h"
#include "StaticMeshComponent.h"
#include "RigidBodySystem.h"

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
					ProxyPtr->DrawSMC(Trans, SMC);
				});

		AddSystem(GlobalAllocator<Achilles::RigidBodySystem>::New(&Reg, "RigidBody System"));
	}

	void World::Tick(double DeltaTime)
	{
		ZoneScoped
		LastDeltaTime = DeltaTime;
		RenderSceneProxy.Reset();
		RunSystems();
	}

}