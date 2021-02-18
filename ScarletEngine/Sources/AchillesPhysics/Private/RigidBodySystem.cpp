#include "RigidBodySystem.h"

namespace ScarletEngine
{
	namespace Achilles
	{
		RigidBodySystem::RigidBodySystem(Registry* InReg, const String& InName)
			: System(InReg, InName)
		{
		}

		void RigidBodySystem::UpdateEntity(const EID Entity, double Dt) const
		{
			RigidBodyComponent* Rb = Reg->GetComponent<RigidBodyComponent>(Entity);
			Transform* Trans = Reg->GetComponent<Transform>(Entity);

			Rb->Force = glm::vec3(0.f, -9.81f, 0.f);

			Rb->Velocity += Rb->Force / Rb->Mass * (float)Dt;
			Trans->Position += Rb->Velocity * (float)Dt;
		}

		void RigidBodySystem::AddForce(glm::vec3 Force, RigidBodyComponent* Rb)
		{
			Rb->Force += Force;
		}

		void RigidBodySystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
			for (const SharedPtr<Entity>& Ent : Entities)
			{
				EID EntityID = Ent->ID;

				if ((Reg->HasComponent<RigidBodyComponent>(EntityID)) && (Reg->HasComponent<Transform>(EntityID)))
				{
					UpdateEntity(EntityID, DeltaTime);
				}
			}
		}
	}
}