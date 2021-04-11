#include "SpringSystem.h"

namespace ScarletEngine::Achilles
{
	void SpringSystem::UpdateEntity(const SpringComponent* Spring, RigidBodyComponent* Rb, const Transform* Trans) const
	{
		ZoneScoped
		const Transform* TransAnchor = Reg->GetComponent<Transform>(Spring->Anchor);

		const glm::vec3 Distance = Trans->Position - TransAnchor->Position;
		const glm::vec3 F = -glm::normalize(Distance) * Spring->Stiffness * (glm::length(Distance) - Spring->RestLength);
		Rb->Force += F - Spring->Damping * Rb->Velocity;

		if (RigidBodyComponent* RbAnchor = Reg->GetComponent<RigidBodyComponent>(Spring->Anchor))
		{
			RbAnchor->Force += -F - Spring->Damping * RbAnchor->Velocity;
		}
	}

	void SpringSystem::FixedUpdate() const
	{
		ZoneScoped
		for (const auto& [EntityID, Springs, Rb, Trans] : GetEntities<const SpringCollection, RigidBodyComponent, const Transform>())
		{
			for (size_t i = 0; i < Springs->size(); i++)
			{
				UpdateEntity(&Springs->at(i), Rb, Trans);
			}
		}

		for (const auto& [EntityID, Spring, Rb, Trans] : GetEntities<const SpringComponent, RigidBodyComponent, const Transform>())
		{
			UpdateEntity(Spring, Rb, Trans);
		}
	}
}
