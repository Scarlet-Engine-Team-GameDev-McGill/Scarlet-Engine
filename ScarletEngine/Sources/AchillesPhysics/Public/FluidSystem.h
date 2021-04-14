#pragma once
#include "RigidBodyComponent.h"
#include "Transform.h" 
#include "ColliderComponent.h"
#include "FluidComponent.h"
#include "System.h"
#include "Core.h"

namespace ScarletEngine::Achilles
{
	class FluidSystem : public System<RigidBodyComponent, Transform, SphereColliderComponent, FluidComponent>
	{
	public:
		virtual void FixedUpdate() const override;
		float Poly6Kernel(glm::vec3 Radius, float H) const;
		glm::vec3 GradSpikyKernel(glm::vec3 Radius, float H) const;
		float EuclidianDistance2(glm::vec3 v) const;
	};
}