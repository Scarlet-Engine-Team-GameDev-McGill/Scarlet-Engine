#pragma once
#include "RigidBodyComponent.h"
#include "Core.h"
#include "Transform.h" 
#include "System.h"
#include "SpringComponent.h"
#include "Engine.h"

namespace ScarletEngine::Achilles
{
	class SpringSystem : public System<RigidBodyComponent, const Transform, const SpringComponent, const SpringCollection>
	{
	public:
		virtual void FixedUpdate() const override;
		void UpdateEntity(const SpringComponent* Spring, RigidBodyComponent* Rb, const Transform* Trans) const;
	};
}
