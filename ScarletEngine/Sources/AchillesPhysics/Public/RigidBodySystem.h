#pragma once
#include "RigidBodyComponent.h"
#include "Core.h"
#include "Transform.h" 
#include "System.h"

namespace ScarletEngine::Achilles
{
	class RigidBodySystem : public System<RigidBodyComponent, Transform>
	{
	public:
		void UpdateEntity(Transform* Trans, RigidBodyComponent* Rb, const float Dt) const;
		virtual void FixedUpdate() const override;
	};
}
