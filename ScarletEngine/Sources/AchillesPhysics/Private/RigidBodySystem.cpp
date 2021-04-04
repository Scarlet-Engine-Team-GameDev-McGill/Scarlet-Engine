#include "RigidBodySystem.h"

namespace ScarletEngine
{
	namespace Achilles
	{
		RigidBodySystem::RigidBodySystem(Registry* InReg, const String& InName)
			: System(InReg, InName)
		{
		}

		void RigidBodySystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
		}

		void RigidBodySystem::UpdateEntity(const EID Entity, double DeltaTime) const
		{
			RigidBodyComponent* Rb = Reg->GetComponent<RigidBodyComponent>(Entity);
			Transform* Trans = Reg->GetComponent<Transform>(Entity);
			float Dt = (float)DeltaTime;

			// translational

			if (Rb->UsesGravity)
			{
				Rb->Force += Rb->GravityEarth * Rb->Mass;
			}

			Rb->Velocity += Rb->Force / Rb->Mass * Dt;
			Trans->Position += Rb->Velocity * Dt;

			Rb->Force = glm::vec3(0.f, 0.f, 0.f);

			// rotational
			
			Rb->AngularVelocity += glm::inverse(Rb->Inertia) * (Rb->Moment - glm::cross(Rb->AngularVelocity, (Rb->Inertia * Rb->AngularVelocity))) * Dt;

			glm::quat QOrientation = glm::quat(glm::radians(Trans->Rotation)); // pitch yaw roll
			glm::quat DQ = QOrientation * Rb->AngularVelocity * 0.5f * Dt;
			QOrientation += DQ;

			Trans->Rotation = glm::degrees(glm::eulerAngles(glm::normalize(QOrientation))); // returns XYZ (PYR) 

			Rb->Moment = glm::vec3(0.f, 0.f, 0.f);
		}	

		void RigidBodySystem::FixedUpdate(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
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