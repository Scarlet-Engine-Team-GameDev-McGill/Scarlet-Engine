#pragma once
#include "RigidBodyComponent.h"
#include "Core.h"
#include "Transform.h" 
#include "System.h"

using namespace Achilles;

namespace ScarletEngine 
{
	namespace Achilles
	{
		class RigidBodySystem : public System<RigidBodyComponent, Transform>
		{
		public:
			RigidBodySystem(Registry* InReg, const String& InName);
			static void AddForce(const glm::vec3 aForce, RigidBodyComponent* aComponent);
			virtual void UpdateEntity(EID EntityID, double DeltaTime) const override;
			virtual void Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const override;
		};
	}
};