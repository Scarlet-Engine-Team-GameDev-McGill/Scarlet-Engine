#pragma once
#include "RigidBodyComponent.h"
#include "Core.h"
#include "Transform.h" 
#include "Engine.h"
#include "System.h"

namespace ScarletEngine::Achilles
{
	class RigidBodySystem : public System<RigidBodyComponent, Transform>
	{
	public:
		void UpdateEntity(EID EntityID, Transform* Trans, RigidBodyComponent* Rb, float DeltaTime) const;
		virtual void FixedUpdate() const override;
		virtual void Update() const override;
	};
}
