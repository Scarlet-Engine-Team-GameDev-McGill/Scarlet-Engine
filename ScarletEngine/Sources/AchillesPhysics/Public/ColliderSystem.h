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
		void UpdateEntity(EID EntityID, double DeltaTime) const;
		virtual void FixedUpdate() const override;
		virtual void Update() const override;
		glm::vec3 GetIntersection(BoxColliderComponent* BoxA, BoxColliderComponent* BoxB) const;
	};

	class SphereVsSphereColliderSystem : public System<SphereColliderComponent, RigidBodyComponent, Transform>
	{
	public:
		void UpdateEntity(EID EntityID, double DeltaTime) const;
		virtual void FixedUpdate() const override;
		virtual void Update() const override;
		std::pair<glm::vec3, float> GetIntersection(SphereColliderComponent* SphereA, SphereColliderComponent* SphereB) const;
	};

	class PlaneVsSphereColliderSystem : public System<SphereColliderComponent, PlaneColliderComponent, RigidBodyComponent, Transform>
	{
	public:
		void UpdateEntity(EID EntityID, double DeltaTime) const;
		virtual void FixedUpdate() const override;
		virtual void Update() const override;
		std::pair<glm::vec3, float> GetIntersection(PlaneColliderComponent* Plane, SphereColliderComponent* Sphere) const;
	};
};