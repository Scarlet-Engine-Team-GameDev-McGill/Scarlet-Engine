#include "Ac_RigidBodySystem.h"

namespace ScarletEngine
{
	Ac_RigidBodySystem::Ac_RigidBodySystem(Registry* InReg, const String& InName)
	: System(InReg, InName)
	{
	}

    void Ac_RigidBodySystem::UpdateEntity(const EID Entity, double Dt) const
	{
		float DtS = Dt * pow(10, -3);

		Ac_RigidBodyComponent* Rb = Reg->GetComponent<Ac_RigidBodyComponent>(Entity);
		Transform* Trans = Reg->GetComponent<Transform>(Entity);

		Rb->Velocity += Rb->Force / Rb->Mass * DtS;
		Trans->Position += Rb->Velocity * DtS;

		ResetForce(Rb);
	}

	void Ac_RigidBodySystem::AddForce(glm::vec3 Force, Ac_RigidBodyComponent* Rb)
	{
		Rb->Force += Force;
	}

	void Ac_RigidBodySystem::ResetForce(Ac_RigidBodyComponent* Rb)
	{
		Rb->Force = glm::vec3(0.f, 0.f, 0.f);
	}

	void Ac_RigidBodySystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
	{
		ComputeGravities(Entities);

		// Update each Entity

		for (const SharedPtr<Entity>& Ent : Entities)
		{
			EID EntityID = Ent->ID;

			if ((Reg->HasComponent<Ac_RigidBodyComponent>(EntityID)) && (Reg->HasComponent<Transform>(EntityID)))
			{
				UpdateEntity(EntityID, DeltaTime);
			}
		}
	}

	void Ac_RigidBodySystem::ComputeGravities(const Array<SharedPtr<Entity>>& Entities) const
	{
		// Compute the forces on the entities
		int size = Entities.size();
		for (int i = 0; i < size - 1; i++)
		{
			EID EntityA = Entities[i]->ID;

			if ((Reg->HasComponent<Ac_RigidBodyComponent>(EntityA)) && (Reg->HasComponent<Transform>(EntityA)))
			{
				for (int j = i + 1; j < size; j++)
				{
					EID EntityB = Entities[j]->ID;
					if ((Reg->HasComponent<Ac_RigidBodyComponent>(EntityB)) && (Reg->HasComponent<Transform>(EntityB)))
					{
						Ac_RigidBodyComponent* RbA = Reg->GetComponent<Ac_RigidBodyComponent>(EntityA);
						Ac_RigidBodyComponent* RbB = Reg->GetComponent<Ac_RigidBodyComponent>(EntityB);

						glm::vec3 posA = Reg->GetComponent<Transform>(EntityA)->Position;
						glm::vec3 posB = Reg->GetComponent<Transform>(EntityB)->Position;

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