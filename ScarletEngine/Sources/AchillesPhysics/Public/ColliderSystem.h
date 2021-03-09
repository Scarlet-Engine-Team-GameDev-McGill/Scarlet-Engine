#pragma once

#include "ColliderComponent.h" 
#include "RigidBodyComponent.h" 
#include "Core.h"
#include "System.h"

using namespace Achilles;

namespace ScarletEngine
{
	namespace Achilles
	{
		class AABBvsAABBColliderSystem : public System<BoxColliderComponent, RigidBodyComponent, Transform>
		{
		public:
			AABBvsAABBColliderSystem(Registry* InReg, const String& InName);
			virtual void UpdateEntity(EID EntityID, double DeltaTime) const override;
			virtual void Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const override;
			glm::vec3 GetIntersection(BoxColliderComponent* BoxA, BoxColliderComponent* BoxB) const;
		};

		class SphereVsSphereColliderSystem : public System<SphereColliderComponent, RigidBodyComponent, Transform>
		{
		public:
			SphereVsSphereColliderSystem(Registry* InReg, const String& InName);
			virtual void UpdateEntity(EID EntityID, double DeltaTime) const override;
			virtual void Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const override;
			glm::vec3 GetIntersection(SphereColliderComponent* SphereA, SphereColliderComponent* SphereB) const;
		};

		class PlaneVsSphereColliderSystem : public System<SphereColliderComponent, PlaneColliderComponent, RigidBodyComponent, Transform>
		{
		public:
			PlaneVsSphereColliderSystem(Registry* InReg, const String& InName);
			virtual void UpdateEntity(EID EntityID, double DeltaTime) const override;
			virtual void Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const override;
			glm::vec3 GetIntersection(PlaneColliderComponent* Plane, SphereColliderComponent* Sphere) const;
		};
	};
};