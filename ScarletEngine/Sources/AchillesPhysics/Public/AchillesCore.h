#pragma once
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "SpringComponent.h"
#include "SpringSystem.h"
#include "RigidBodySystem.h"
#include "FluidComponent.h"
#include "FluidSystem.h"
#include "ColliderSystem.h"

namespace ScarletEngine::Achilles
{
	void RegisterSystems()
	{
		SystemScheduler::Get().RegisterSystem<SpringSystem>();
		SystemScheduler::Get().RegisterSystem<RigidBodySystem>();
		SystemScheduler::Get().RegisterSystem<AABBvsAABBColliderSystem>();
		SystemScheduler::Get().RegisterSystem<SphereVsSphereColliderSystem>();
		SystemScheduler::Get().RegisterSystem<PlaneVsSphereColliderSystem>();
		SystemScheduler::Get().RegisterSystem<FluidSystem>();
	}
}

