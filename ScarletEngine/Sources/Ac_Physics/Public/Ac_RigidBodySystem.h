#pragma once
#include "Ac_RigidBodyComponent.h"
#include "Core.h"
#include "Transform.h" 
#include "System.h"

namespace ScarletEngine 
{
	class Ac_RigidBodySystem : public System<Ac_RigidBodyComponent, Transform>
	{

	public:
		Ac_RigidBodySystem(Registry* InReg, const String& InName);
		static void AddForce(const glm::vec3 aForce, Ac_RigidBodyComponent* aComponent);
		static void ResetForce(Ac_RigidBodyComponent* Rb);
		void ComputeGravities(const Array<SharedPtr<Entity>>& Entities) const;
		virtual void UpdateEntity(EID EntityID, double DeltaTime) const override;
		virtual void Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const override;
	};
};