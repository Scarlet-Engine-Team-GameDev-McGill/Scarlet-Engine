#pragma once
#include "RigidBodyComponent.h"
#include "Core.h"
#include "Transform.h" 
#include "System.h"
#include "SpringComponent.h"

using namespace Achilles;

namespace ScarletEngine
{
	namespace Achilles
	{
		class SpringSystem : public System<RigidBodyComponent, Transform, SpringComponent>
		{
		public:
			SpringSystem(Registry* InReg, const String& InName);
			virtual void UpdateEntity(EID EntityID, double DeltaTime) const override;
			virtual void FixedUpdate(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const override;
			virtual void Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const override;
		};
	}
};