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

		Rb->Force = glm::vec3(0.f, -9.81f, 0.f);

		Rb->Velocity += Rb->Force / Rb->Mass * DtS;
		Trans->Position += Rb->Velocity * DtS;
	}

	void Ac_RigidBodySystem::AddForce(glm::vec3 Force, Ac_RigidBodyComponent* Rb)
	{
		Rb->Force += Force;
	}

	void Ac_RigidBodySystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
	{
		for (const SharedPtr<Entity>& Ent : Entities)
		{
			EID EntityID = Ent->ID;

			if ((Reg->HasComponent<Ac_RigidBodyComponent>(EntityID)) && (Reg->HasComponent<Transform>(EntityID)))
			{
				UpdateEntity(EntityID, DeltaTime);
			}
		}
	}
}

/*
Ac_RigidBodyComponent RbA = std::get<Ac_RigidBodyComponent>(allComp[i]);
Ac_RigidBodyComponent RbB = std::get<Ac_RigidBodyComponent>(allComp[j]);

glm::vec3 posA = std::get<Transform>(allComp[i]).Position;
glm::vec3 posB = std::get<Transform>(allComp[j]).Position;

float d = glm::distance(posA, posB);
glm::vec3 u = posA / glm::length(posA);

float mA = RbA.Mass;
float mB = RbB.Mass;

glm::vec3 G = u * 6.14f * pow(10.f, 11.f) * mA * mB / d;

AddForce(G, RbA);
AddForce(-G, RbB);
*/