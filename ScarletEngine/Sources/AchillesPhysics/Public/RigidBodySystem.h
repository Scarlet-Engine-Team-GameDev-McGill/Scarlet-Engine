#pragma once
#include "Components/RigidBodyComponent.h"
#include "Core.h"
#include "Components/TransformComponent.h" 
#include "System.h"

namespace ScarletEngine::Achilles
{
	class RigidBodySystem final : public System<RigidBodyComponent, TransformComponent>
	{
	public:
		virtual bool IsGameplayOnly() const override { return true;}

		void UpdateEntity(TransformComponent* Trans, RigidBodyComponent* Rb, const float Dt) const;
		virtual void FixedUpdate() const override;
	};
}
