#pragma once

#include "Ac_ColliderComponent.h" 
#include "Ac_RigidBodyComponent.h" 
#include "Core.h"
#include "System.h"

namespace ScarletEngine
{
	class Ac_AABBvsAABBColliderSystem : public System<Ac_BoxColliderComponent, Ac_RigidBodyComponent, Transform>
	{
	public:
		Ac_AABBvsAABBColliderSystem(Registry* InReg, const String& InName);
		virtual void UpdateEntity(EID EntityID, double DeltaTime) const override;
		virtual void Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const override;
		float GetIntersection(Ac_BoxColliderComponent* BoxA, Ac_BoxColliderComponent* BoxB) const;
	};

	class Ac_SphereVsSphereColliderSystem : public System<Ac_SphereColliderComponent, Ac_RigidBodyComponent, Transform>
	{
	public:
		Ac_SphereVsSphereColliderSystem(Registry* InReg, const String& InName);
		virtual void UpdateEntity(EID EntityID, double DeltaTime) const override;
		virtual void Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const override;
		float GetIntersection(Ac_SphereColliderComponent* SphereA, Ac_SphereColliderComponent* SphereB) const;
	};

	class Ac_PlaneVsSphereColliderSystem : public System<Ac_SphereColliderComponent, Ac_PlaneColliderComponent, Ac_RigidBodyComponent, Transform>
	{
	public:
		Ac_PlaneVsSphereColliderSystem(Registry* InReg, const String& InName);
		virtual void UpdateEntity(EID EntityID, double DeltaTime) const override;
		virtual void Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const override;
		float GetIntersection(Ac_PlaneColliderComponent* Plane, Ac_SphereColliderComponent* Sphere) const;
	};
};