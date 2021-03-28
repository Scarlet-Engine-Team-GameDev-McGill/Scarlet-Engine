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

		void RigidBodySystem::UpdateEntity(const EID Entity, double Dt) const
		{
			RigidBodyComponent* Rb = Reg->GetComponent<RigidBodyComponent>(Entity);
			Transform* Trans = Reg->GetComponent<Transform>(Entity);

			// translational

			if (Rb->UsesGravity)
			{
				Rb->Force += Rb->GravityEarth * Rb->Mass;
			}

			Rb->Velocity += Rb->Force / Rb->Mass * (float)Dt;
			Trans->Position += Rb->Velocity * (float)Dt;

			Rb->Force = glm::vec3(0.f, 0.f, 0.f);

			// rotational
			/*
			
			Rb->AngularVelocity += glm::inverse(Rb->Inertia) * (Rb->Moment - glm::cross(Rb->AngularVelocity, (Rb->Inertia * Rb->AngularVelocity)) * (float)Dt);

			glm::quat QOrientation = glm::quat(glm::radians(Trans->Rotation)); // pitch yaw roll
			glm::quat DQ = QOrientation * Rb->AngularVelocity * 0.5f * (float)Dt;
			QOrientation += DQ;

			Trans->Rotation = glm::degrees(glm::eulerAngles(QOrientation)); // returns XYZ (PYR) */
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