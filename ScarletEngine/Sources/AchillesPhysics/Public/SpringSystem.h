#pragma once
#include "RigidBodyComponent.h"
#include "Core.h"
#include "Transform.h" 
#include "System.h"
#include "SpringComponent.h"
#include "Engine.h"

namespace ScarletEngine::Achilles
{
	class SpringSystem : public System<RigidBodyComponent, Transform, SpringComponent, SpringCollection>
	{
	public:
		virtual void FixedUpdate() const override;
		virtual void Update() const override;
		void UpdateEntity(SpringComponent* Spring, RigidBodyComponent* Rb, Transform* Trans) const;
	};
}
