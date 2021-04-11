#pragma once
#include "RigidBodyComponent.h"
#include "Core.h"
#include "Transform.h" 
#include "System.h"
#include "SpringComponent.h"
#include "Engine.h"

using namespace Achilles;

namespace ScarletEngine
{
	namespace Achilles
	{
		class SpringSystem : public System<RigidBodyComponent, Transform, SpringComponent>
		{
		public:
			virtual void FixedUpdate() const override;
			virtual void Update() const override;
			void UpdateEntity(const EID Entity, double Dt, SpringComponent* Spring) const;
		};
	}
};