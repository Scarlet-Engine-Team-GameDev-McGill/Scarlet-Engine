#include "SpringSystem.h"

namespace ScarletEngine
{
	namespace Achilles
	{
		SpringSystem::SpringSystem(Registry* InReg, const String& InName)
			: System(InReg, InName)
		{
		}

		void SpringSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
		}

		void SpringSystem::UpdateEntity(const EID Entity, double Dt) const
		{
			SpringComponent* Spring = Reg->GetComponent<SpringComponent>(Entity);
			RigidBodyComponent* Rb = Reg->GetComponent<RigidBodyComponent>(Entity);
			Transform* Trans = Reg->GetComponent<Transform>(Entity);
			Transform* TransAnchor = Reg->GetComponent<Transform>(Spring->Anchor);

			glm::vec3 distance = Trans->Position - TransAnchor->Position;
			glm::vec3 F = - glm::normalize(distance) * Spring->Stiffness * (glm::length(distance) - Spring->RestLength);
			Rb->Force += F - Spring->Damping * Rb->Velocity;

			if (Reg->HasComponent<RigidBodyComponent>(Spring->Anchor))
			{
				RigidBodyComponent* RbAnchor = Reg->GetComponent<RigidBodyComponent>(Spring->Anchor);
				RbAnchor->Force += - F - Spring->Damping * RbAnchor->Velocity;
			}

		}

		void SpringSystem::FixedUpdate(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
			for (const SharedPtr<Entity>& Ent : Entities)
			{
				EID EntityID = Ent->ID;

				if (Reg->HasComponent<SpringComponent>(EntityID) && Reg->HasComponent<RigidBodyComponent>(EntityID) && Reg->HasComponent<Transform>(EntityID))
				{
					UpdateEntity(EntityID, DeltaTime);
				}
			}
		}
	}
}
