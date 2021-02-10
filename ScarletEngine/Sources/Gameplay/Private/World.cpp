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
			.Each([ProxyPtr](const EID, const Transform& Trans, const StaticMeshComponent& SMC)
				{
					ProxyPtr->DrawSMC(Trans, SMC);
				});

		AddSystem<Transform, const Ac_RigidBodyComponent>("RigidBody")
			.Each([ProxyPtr](const EID, Transform& Trans, const Ac_RigidBodyComponent& Rb)
				{
					Trans.Rotation += glm::vec3(1f, 0.f, 0.f);
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