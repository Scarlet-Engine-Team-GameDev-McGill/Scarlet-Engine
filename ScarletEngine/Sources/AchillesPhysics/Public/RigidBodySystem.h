#pragma once
#include "Components/RigidBodyComponent.h"
#include "Core.h"
#include "Components/TransformComponent.h" 
#include "System.h"

namespace ScarletEngine::Achilles
{
	// #todo_physics: one system for all rigidbodies is probably too few. Could be split into Gravity, Dynamics, etc.
	class RigidBodySystem final : public System<RigidBodyComponent, TransformComponent>
	{
	public:
		virtual String GetName() const override { return "RigidBodySystem"; }
		virtual bool IsGameplayOnly() const override { return true;}

		void UpdateEntity(TransformComponent* Trans, RigidBodyComponent* Rb, const float Dt) const;
		virtual void FixedUpdate() const override;
	};
}
