#pragma once
#include "Components/RigidBodyComponent.h"
#include "Core.h"
#include "Components/TransformComponent.h" 
#include "System.h"

namespace ScarletEngine::Achilles
{
	class RigidBodySystem : public System<RigidBodyComponent, TransformComponent>
	{
	public:
		void UpdateEntity(TransformComponent* Trans, RigidBodyComponent* Rb, const float Dt) const;
		virtual void FixedUpdate() const override;
	};
}
