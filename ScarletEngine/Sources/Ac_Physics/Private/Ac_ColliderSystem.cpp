#pragma once

#include "Ac_ColliderSystem.h"
#include "Ac_RigidBodyComponent.h"
#include "Core.h"

namespace ScarletEngine
{
#pragma region AABBvsAABB


	Ac_AABBvsAABBColliderSystem::Ac_AABBvsAABBColliderSystem(Registry* InReg, const String& InName)
		: System(InReg, InName)
	{
	}

	void Ac_AABBvsAABBColliderSystem::UpdateEntity(const EID Entity, double Dt) const
	{
	}

	void Ac_AABBvsAABBColliderSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
	{
		for (int i = 0; i < Entities.size()-1; i++)
		{
			for (int j = i + 1; j < Entities.size(); j++)
			{
				EID EntityA = Entities[i]->ID;
				EID EntityB = Entities[j]->ID;

				if (Reg->HasComponent<Ac_BoxColliderComponent>(EntityA) && Reg->HasComponent<Ac_BoxColliderComponent>(EntityB))
				{
					Ac_BoxColliderComponent* BoxA = Reg->GetComponent<Ac_BoxColliderComponent>(EntityA);
					Ac_BoxColliderComponent* BoxB = Reg->GetComponent<Ac_BoxColliderComponent>(EntityB);

					GetIntersection(BoxA, BoxB);
				}
			}
		}
	}

	float Ac_AABBvsAABBColliderSystem::GetIntersection(Ac_BoxColliderComponent* BoxA, Ac_BoxColliderComponent* BoxB) const
	{
		glm::vec3 dist = glm::max(BoxB->Max - BoxA->Min, BoxB->Min - BoxA->Max);
		return glm::max(glm::max(dist.x, dist.y), dist.z);	// intersects if negative
	}

#pragma endregion

#pragma region SphereVSphere

	Ac_SphereVsSphereColliderSystem::Ac_SphereVsSphereColliderSystem(Registry* InReg, const String& InName)
		: System(InReg, InName)
	{
	}

	void Ac_SphereVsSphereColliderSystem::UpdateEntity(const EID Entity, double Dt) const
	{
	}

	void Ac_SphereVsSphereColliderSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
	{
		for (int i = 0; i < Entities.size() - 1; i++)
		{
			for (int j = i + 1; j < Entities.size(); j++)
			{
				EID EntityA = Entities[i]->ID;
				EID EntityB = Entities[j]->ID;

				if (Reg->HasComponent<Ac_SphereColliderComponent>(EntityA) && Reg->HasComponent<Ac_SphereColliderComponent>(EntityB))
				{
					Ac_SphereColliderComponent* SphereA = Reg->GetComponent<Ac_SphereColliderComponent>(EntityA);
					Ac_SphereColliderComponent* SphereB = Reg->GetComponent<Ac_SphereColliderComponent>(EntityB);

					GetIntersection(SphereA, SphereB);
				}
			}
		}
	}

	float Ac_SphereVsSphereColliderSystem::GetIntersection(Ac_SphereColliderComponent* SphereA, Ac_SphereColliderComponent* SphereB) const
	{
		float centDist = glm::length(SphereB->Pos - SphereA->Pos);
		float radDist = SphereB->Radius + SphereB->Radius;

		return centDist - radDist;	// intersects if negative
	}

#pragma endregion

#pragma region PlaneVsSphere

	Ac_PlaneVsSphereColliderSystem::Ac_PlaneVsSphereColliderSystem(Registry* InReg, const String& InName)
		: System(InReg, InName)
	{
	}

	void Ac_PlaneVsSphereColliderSystem::UpdateEntity(const EID Entity, double Dt) const
	{
	}

	void Ac_PlaneVsSphereColliderSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
	{
		for (int i = 0; i < Entities.size() - 1; i++)
		{
			for (int j = i + 1; j < Entities.size(); j++)
			{
				EID EntityA = Entities[i]->ID;
				EID EntityB = Entities[j]->ID;

				if (Reg->HasComponent<Ac_PlaneColliderComponent>(EntityA) && Reg->HasComponent<Ac_SphereColliderComponent>(EntityB))
				{
					Ac_PlaneColliderComponent* Plane = Reg->GetComponent<Ac_PlaneColliderComponent>(EntityA);
					Ac_SphereColliderComponent* Sphere = Reg->GetComponent<Ac_SphereColliderComponent>(EntityB);

					GetIntersection(Plane, Sphere);
				}
				else if (Reg->HasComponent<Ac_PlaneColliderComponent>(EntityB) && Reg->HasComponent<Ac_SphereColliderComponent>(EntityA))
				{
					Ac_PlaneColliderComponent* Plane = Reg->GetComponent<Ac_PlaneColliderComponent>(EntityB);
					Ac_SphereColliderComponent* Sphere = Reg->GetComponent<Ac_SphereColliderComponent>(EntityA);

					GetIntersection(Plane, Sphere);
				}
			}
		}
	}

	float Ac_PlaneVsSphereColliderSystem::GetIntersection(Ac_PlaneColliderComponent* Plane, Ac_SphereColliderComponent* Sphere) const
	{
		return abs(glm::dot(Plane->Normal, Sphere->Pos) + Plane->Distance) - Sphere->Radius; // Intersects if negative
	}

#pragma endregion

}
