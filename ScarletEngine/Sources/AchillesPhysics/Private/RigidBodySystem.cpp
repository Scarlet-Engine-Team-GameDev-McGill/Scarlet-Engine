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

			ComputeGravities(Entities);
		}

		void RigidBodySystem::ComputeGravities(const Array<SharedPtr<Entity>>& Entities) const
		{
			// Compute the forces on the entities
			int size = Entities.size();
			for (int i = 0; i < size - 1; i++)
			{
				EID EntityA = Entities[i]->ID;

				if ((Reg->HasComponent<RigidBodyComponent>(EntityA)) && (Reg->HasComponent<Transform>(EntityA)))
				{
					for (int j = i + 1; j < size; j++)
					{
						EID EntityB = Entities[j]->ID;
						if ((Reg->HasComponent<RigidBodyComponent>(EntityB)) && (Reg->HasComponent<Transform>(EntityB)))
						{
							RigidBodyComponent* RbA = Reg->GetComponent<RigidBodyComponent>(EntityA);
							RigidBodyComponent* RbB = Reg->GetComponent<RigidBodyComponent>(EntityB);

							if (RbA->UsesKeplerGravity && RbB->UsesKeplerGravity)
							{

								glm::vec3 posA = Reg->GetComponent<Transform>(EntityA)->Position;
								glm::vec3 posB = Reg->GetComponent<Transform>(EntityB)->Position;

								float d = abs(glm::distance(posA, posB));

								if (d != 0)
								{
									glm::vec3 u = (posB - posA) / d;

									float mA = RbA->Mass;
									float mB = RbB->Mass;

									glm::vec3 G = 6.14f * pow(10.f, -11.f) * mA * mB * u / pow(d, 2.f);

									RbA->Force += G;
									RbB->Force -= G;
								}
							}
						}
					}
				}
			}
		}
	}
}