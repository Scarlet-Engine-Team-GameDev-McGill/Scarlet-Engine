#pragma once
#include "Components/RigidBodyComponent.h"
#include "Core.h"
#include "Components/TransformComponent.h" 
#include "System.h"
#include "Components/SpringComponent.h"

namespace ScarletEngine::Achilles
{
	// #todo_physics: split this into a SpringSystem and a SpringCollection system
	class SpringSystem final : public System<RigidBodyComponent, SpringCollection, const TransformComponent, const SpringComponent>
	{
	public:
		virtual String GetName() const override { return "SpringSystem"; }
		virtual bool IsGameplayOnly() const override { return true;}

		virtual void FixedUpdate() const override;
	private:
		void UpdateEntity(const SpringComponent* Spring, RigidBodyComponent* Rb, const TransformComponent* Trans) const;
	};
}
