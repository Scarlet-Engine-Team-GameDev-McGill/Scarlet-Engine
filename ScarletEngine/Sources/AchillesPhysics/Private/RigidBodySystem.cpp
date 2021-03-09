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

			Rb->Velocity += Rb->Force / Rb->Mass * (float)Dt * 70.f;
			Trans->Position += Rb->Velocity * (float)Dt;

			ResetForce(Rb);
		}

		void RigidBodySystem::AddForce(glm::vec3 Force, RigidBodyComponent* Rb)
		{
			Rb->Force += Force;
		}

		void RigidBodySystem::ResetForce(RigidBodyComponent* Rb)
		{
			Rb->Force = glm::vec3(0.f, 0.f, 0.f);
		}

		void RigidBodySystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
			ComputeGravities(Entities);

			// Update each Entity

			for (const SharedPtr<Entity>& Ent : Entities)
			{
				EID EntityID = Ent->ID;

				if ((Reg->HasComponent<RigidBodyComponent>(EntityID)) && (Reg->HasComponent<Transform>(EntityID)))
				{
					UpdateEntity(EntityID, DeltaTime);
				}
			}
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

					RigidBodyComponent* RbA = Reg->GetComponent<RigidBodyComponent>(EntityA);
					glm::vec3 posA = Reg->GetComponent<Transform>(EntityA)->Position;

					if (RbA->UsesGravity)
					{
						for (int j = i + 1; j < size; j++)
						{
							EID EntityB = Entities[j]->ID;

							if ((Reg->HasComponent<RigidBodyComponent>(EntityB)) && (Reg->HasComponent<Transform>(EntityB)))
							{
								RigidBodyComponent* RbB = Reg->GetComponent<RigidBodyComponent>(EntityB);
								glm::vec3 posB = Reg->GetComponent<Transform>(EntityB)->Position;

								if (RbB->UsesGravity)
								{
									float d = abs(glm::distance(posA, posB));

									if (d != 0)
									{
										glm::vec3 u = (posB - posA) / d;

										float mA = RbA->Mass;
										float mB = RbB->Mass;

										glm::vec3 G = 6.14f * pow(10.f, -11.f) * mA * mB * u / pow(d, 2.f);

										AddForce(G, RbA);
										AddForce(-G, RbB);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}