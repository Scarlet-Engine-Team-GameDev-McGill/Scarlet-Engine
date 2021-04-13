#include "SpringSystem.h"

namespace ScarletEngine::Achilles
{
	void SpringSystem::UpdateEntity(const SpringComponent* Spring, RigidBodyComponent* Rb, const TransformComponent* Trans) const
	{
		ZoneScoped
		const auto [EntA, TransAnchor] = GetEntityChecked<const TransformComponent>(Spring->Anchor);

		const glm::vec3 Distance = Trans->Position - TransAnchor->Position;
		const glm::vec3 F = -glm::normalize(Distance) * Spring->Stiffness * (glm::length(Distance) - Spring->RestLength);
		Rb->Force += F - Spring->Damping * Rb->Velocity;

		if (auto Proxy = GetEntity<RigidBodyComponent>(Spring->Anchor))
		{
			auto [Ent, RbAnchor] = Proxy.value();
			RbAnchor->Force += -F - Spring->Damping * RbAnchor->Velocity;
		}
	}

	void SpringSystem::FixedUpdate() const
	{
		ZoneScoped
		for (const auto& [EntityID, Springs, Rb, Trans] : GetEntities<const SpringCollection, RigidBodyComponent, const TransformComponent>())
		{
			for (size_t i = 0; i < Springs->size(); i++)
			{
				UpdateEntity(&Springs->at(i), Rb, Trans);
			}
		}

		for (const auto& [EntityID, Spring, Rb, Trans] : GetEntities<const SpringComponent, RigidBodyComponent, const TransformComponent>())
		{
			UpdateEntity(Spring, Rb, Trans);
		}
	}
}
