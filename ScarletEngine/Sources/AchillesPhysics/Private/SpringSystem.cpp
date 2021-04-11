#include "SpringSystem.h"

namespace ScarletEngine::Achilles
{
	void SpringSystem::Update() const
	{
	}

	void SpringSystem::UpdateEntity(SpringComponent* Spring, RigidBodyComponent* Rb, Transform* Trans) const
	{
		Transform* TransAnchor = Reg->GetComponent<Transform>(Spring->Anchor);

		glm::vec3 distance = Trans->Position - TransAnchor->Position;
		glm::vec3 F = -glm::normalize(distance) * Spring->Stiffness * (glm::length(distance) - Spring->RestLength);
		Rb->Force += F - Spring->Damping * Rb->Velocity;

		if (Reg->HasComponent<RigidBodyComponent>(Spring->Anchor))
		{
			RigidBodyComponent* RbAnchor = Reg->GetComponent<RigidBodyComponent>(Spring->Anchor);
			RbAnchor->Force += -F - Spring->Damping * RbAnchor->Velocity;
		}
	}

	void SpringSystem::FixedUpdate() const
	{
		for (const auto& [EntityID, Springs, Rb, Trans] : GetEntities<SpringCollection, RigidBodyComponent, Transform>())
		{
			for (int i = 0; i < Springs->size(); i++)
			{
				UpdateEntity(&Springs->at(i), Rb, Trans);
			}
		}

		for (const auto& [EntityID, Spring, Rb, Trans] : GetEntities<SpringComponent, RigidBodyComponent, Transform>())
		{
			UpdateEntity(Spring, Rb, Trans);
		}
	}
}
