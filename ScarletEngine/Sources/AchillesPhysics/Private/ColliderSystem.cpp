#pragma once

#include "ColliderSystem.h"
#include "RigidBodyComponent.h"
#include "Core.h"

namespace ScarletEngine
{
	namespace Achilles
	{
#pragma region AABBvsAABB
		AABBvsAABBColliderSystem::AABBvsAABBColliderSystem(Registry* InReg, const String& InName)
			: System(InReg, InName)
		{
		}

		void AABBvsAABBColliderSystem::UpdateEntity(const EID Entity, double Dt) const
		{
		}

		void AABBvsAABBColliderSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
			for (int i = 0; i < Entities.size() - 1; i++)
			{
				for (int j = i + 1; j < Entities.size(); j++)
				{
					EID EntityA = Entities[i]->ID;
					EID EntityB = Entities[j]->ID;

					if (Reg->HasComponent<BoxColliderComponent>(EntityA) && Reg->HasComponent<BoxColliderComponent>(EntityB))
					{
						BoxColliderComponent* BoxA = Reg->GetComponent<BoxColliderComponent>(EntityA);
						BoxColliderComponent* BoxB = Reg->GetComponent<BoxColliderComponent>(EntityB);

						GetIntersection(BoxA, BoxB);
					}
				}
			}
		}

		float AABBvsAABBColliderSystem::GetIntersection(BoxColliderComponent* BoxA, BoxColliderComponent* BoxB) const
		{
			glm::vec3 dist = glm::max(BoxB->Max - BoxA->Min, BoxB->Min - BoxA->Max);
			return glm::max(glm::max(dist.x, dist.y), dist.z);	// intersects if negative
		}

#pragma endregion

#pragma region SphereVSphere

		SphereVsSphereColliderSystem::SphereVsSphereColliderSystem(Registry* InReg, const String& InName)
			: System(InReg, InName)
		{
		}

		void SphereVsSphereColliderSystem::UpdateEntity(const EID Entity, double Dt) const
		{
		}

		void SphereVsSphereColliderSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
			for (int i = 0; i < Entities.size() - 1; i++)
			{
				for (int j = i + 1; j < Entities.size(); j++)
				{
					EID EntityA = Entities[i]->ID;
					EID EntityB = Entities[j]->ID;

					if (Reg->HasComponent<SphereColliderComponent>(EntityA) && Reg->HasComponent<SphereColliderComponent>(EntityB))
					{
						SphereColliderComponent* SphereA = Reg->GetComponent<SphereColliderComponent>(EntityA);
						SphereColliderComponent* SphereB = Reg->GetComponent<SphereColliderComponent>(EntityB);

						GetIntersection(SphereA, SphereB);
					}
				}
			}
		}

		float SphereVsSphereColliderSystem::GetIntersection(SphereColliderComponent* SphereA, SphereColliderComponent* SphereB) const
		{
			float centDist = glm::length(SphereB->Pos - SphereA->Pos);
			float radDist = SphereB->Radius + SphereB->Radius;

			return centDist - radDist;	// intersects if negative
		}

#pragma endregion

#pragma region PlaneVsSphere

		PlaneVsSphereColliderSystem::PlaneVsSphereColliderSystem(Registry* InReg, const String& InName)
			: System(InReg, InName)
		{
		}

		void PlaneVsSphereColliderSystem::UpdateEntity(const EID Entity, double Dt) const
		{
		}

		void PlaneVsSphereColliderSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
			for (int i = 0; i < Entities.size() - 1; i++)
			{
				for (int j = i + 1; j < Entities.size(); j++)
				{
					EID EntityA = Entities[i]->ID;
					EID EntityB = Entities[j]->ID;

					if (Reg->HasComponent<PlaneColliderComponent>(EntityA) && Reg->HasComponent<SphereColliderComponent>(EntityB))
					{
						PlaneColliderComponent* Plane = Reg->GetComponent<PlaneColliderComponent>(EntityA);
						SphereColliderComponent* Sphere = Reg->GetComponent<SphereColliderComponent>(EntityB);

						GetIntersection(Plane, Sphere);
					}
					else if (Reg->HasComponent<PlaneColliderComponent>(EntityB) && Reg->HasComponent<SphereColliderComponent>(EntityA))
					{
						PlaneColliderComponent* Plane = Reg->GetComponent<PlaneColliderComponent>(EntityB);
						SphereColliderComponent* Sphere = Reg->GetComponent<SphereColliderComponent>(EntityA);

						GetIntersection(Plane, Sphere);
					}
				}
			}
		}

		float PlaneVsSphereColliderSystem::GetIntersection(PlaneColliderComponent* Plane, SphereColliderComponent* Sphere) const
		{
			return abs(glm::dot(Plane->Normal, Sphere->Pos) + Plane->Distance) - Sphere->Radius; // Intersects if negative
		}

#pragma endregion
	};
};
