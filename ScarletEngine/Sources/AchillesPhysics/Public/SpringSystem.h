#pragma once
#include "RigidBodyComponent.h"
#include "Core.h"
#include "Transform.h" 
#include "System.h"
#include "SpringComponent.h"

namespace ScarletEngine::Achilles
{
	class SpringSystem : public System<RigidBodyComponent, const Transform, const SpringComponent, const SpringCollection>
	{
	public:
		virtual void FixedUpdate() const override;
	private:
		void UpdateEntity(const SpringComponent* Spring, RigidBodyComponent* Rb, const Transform* Trans) const;
		float Poly6Kernel(float radius, float h);
		float SpikyKernel(float radius, float h);
	};
}
