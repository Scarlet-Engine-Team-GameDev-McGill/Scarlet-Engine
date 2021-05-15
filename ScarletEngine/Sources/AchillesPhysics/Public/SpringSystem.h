#pragma once
#include "Components/RigidBodyComponent.h"
#include "Core.h"
#include "Components/TransformComponent.h" 
#include "System.h"
#include "Components/SpringComponent.h"

namespace ScarletEngine::Achilles
{
	class SpringSystem : public System<RigidBodyComponent, const TransformComponent, const SpringComponent, const SpringCollection>
	{
	public:
		virtual void FixedUpdate() const override;
	private:
		void UpdateEntity(const SpringComponent* Spring, RigidBodyComponent* Rb, const TransformComponent* Trans) const;
	};
}
