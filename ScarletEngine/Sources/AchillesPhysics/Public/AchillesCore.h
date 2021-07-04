#pragma once
#include "Components/RigidBodyComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/SpringComponent.h"
#include "SpringSystem.h"
#include "RigidBodySystem.h"
#include "ColliderSystem.h"

namespace ScarletEngine::Achilles
{
	inline void RegisterSystems()
	{
		SystemScheduler::Get().RegisterSystem<SpringSystem>();
		SystemScheduler::Get().RegisterSystem<RigidBodySystem>();
		SystemScheduler::Get().RegisterSystem<AABBvsAABBCollisionSystem>();
		SystemScheduler::Get().RegisterSystem<SphereVsSphereCollisionSystem>();
		SystemScheduler::Get().RegisterSystem<PlaneVsSphereCollisionSystem>();
	}
}

