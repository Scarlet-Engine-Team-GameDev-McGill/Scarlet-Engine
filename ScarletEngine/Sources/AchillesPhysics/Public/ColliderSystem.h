#pragma once

#include "ColliderComponent.h" 
#include "RigidBodyComponent.h" 
#include "Core.h"
#include "Engine.h"
#include "System.h"

namespace ScarletEngine::Achilles
{
	class AABBvsAABBColliderSystem : public System<BoxColliderComponent, RigidBodyComponent, Transform>
	{
	public:
		virtual void FixedUpdate() const override;
		virtual void Update() const override;
	};

	class SphereVsSphereColliderSystem : public System<SphereColliderComponent, RigidBodyComponent, Transform>
	{
	public:
		virtual void FixedUpdate() const override;
		virtual void Update() const override;
		std::pair<glm::vec3, float> GetIntersection(SphereColliderComponent* SphereA, SphereColliderComponent* SphereB) const;
		void SolveIntersection(RigidBodyComponent* Rb, Transform* Trans, SphereColliderComponent* Sphere, SphereColliderComponent* OtherSphere, glm::vec3 Fi, glm::vec3 newPos) const;
	};

	class PlaneVsSphereColliderSystem : public System<SphereColliderComponent, PlaneColliderComponent, RigidBodyComponent, Transform>
	{
	public:
		virtual void FixedUpdate() const override;
		virtual void Update() const override;
		std::pair<glm::vec3, float> GetIntersection(PlaneColliderComponent* Plane, SphereColliderComponent* Sphere) const;
	};
}
