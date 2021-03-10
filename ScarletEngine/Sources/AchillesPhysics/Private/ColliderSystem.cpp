#pragma once

#include "ColliderSystem.h"
#include "RigidBodyComponent.h"
#include "Core.h"

namespace ScarletEngine
{
	namespace Achilles
	{
		void Bounce(EID& EntityA, EID& EntityB, glm::vec3 IntersectionDepth, Registry* Reg)
		{
			if (glm::length(IntersectionDepth) <= 0.001f && Reg->HasComponent<RigidBodyComponent>(EntityB) && Reg->HasComponent<RigidBodyComponent>(EntityA))
			{
				RigidBodyComponent* RbA = Reg->GetComponent<RigidBodyComponent>(EntityA);
				RigidBodyComponent* RbB = Reg->GetComponent<RigidBodyComponent>(EntityB);

				RbA->Velocity = glm::reflect(RbA->Velocity, glm::reflect(glm::normalize(IntersectionDepth), glm::normalize(RbA->Velocity)));
				RbB->Velocity = glm::reflect(RbB->Velocity, glm::normalize(IntersectionDepth));
			}
		}

#pragma region AABBvsAABB
		AABBvsAABBColliderSystem::AABBvsAABBColliderSystem(Registry* InReg, const String& InName)
			: System(InReg, InName)
		{
		}

		void AABBvsAABBColliderSystem::UpdateEntity(const EID Entity, double Dt) const
		{
			RigidBodyComponent* Rb = Reg->GetComponent<RigidBodyComponent>(Entity);
			BoxColliderComponent* Box = Reg->GetComponent<BoxColliderComponent>(Entity);

			Box->Max += Rb->Velocity * (float)Dt;
			Box->Min += Rb->Velocity * (float)Dt;
		}

		void AABBvsAABBColliderSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
			// Move collider
			for (int i = 0; i < Entities.size(); i++)
			{
				EID Entity = Entities[i]->ID;
				if (Reg->HasComponent<RigidBodyComponent>(Entity) && Reg->HasComponent<BoxColliderComponent>(Entity))
				{
					UpdateEntity(Entity, DeltaTime);
				}
			}

			// Get Intersection
			for (int i = 0; i < Entities.size() - 1; i++)
			{
				EID EntityA = Entities[i]->ID;
				for (int j = i + 1; j < Entities.size(); j++)
				{
					EID EntityB = Entities[j]->ID;

					if (Reg->HasComponent<BoxColliderComponent>(EntityA) && Reg->HasComponent<BoxColliderComponent>(EntityB))
					{
						BoxColliderComponent* BoxA = Reg->GetComponent<BoxColliderComponent>(EntityA);
						BoxColliderComponent* BoxB = Reg->GetComponent<BoxColliderComponent>(EntityB);

						// if dynamic : bounce
						Bounce(EntityA, EntityB, GetIntersection(BoxA, BoxB), Reg);
					}
				}
			}
		}

		glm::vec3 AABBvsAABBColliderSystem::GetIntersection(BoxColliderComponent* BoxA, BoxColliderComponent* BoxB) const
		{
			return glm::max(BoxB->Max - BoxA->Min, BoxB->Min - BoxA->Max);
		}

#pragma endregion

#pragma region SphereVSphere

		SphereVsSphereColliderSystem::SphereVsSphereColliderSystem(Registry* InReg, const String& InName)
			: System(InReg, InName)
		{
		}

		void SphereVsSphereColliderSystem::UpdateEntity(const EID Entity, double Dt) const
		{
			RigidBodyComponent* Rb = Reg->GetComponent<RigidBodyComponent>(Entity);
			SphereColliderComponent* Sphere = Reg->GetComponent<SphereColliderComponent>(Entity);

			Sphere->Pos += Rb->Velocity * (float)Dt;
		}

		void SphereVsSphereColliderSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
			// Move collider
			for (int i = 0; i < Entities.size(); i++)
			{
				EID Entity = Entities[i]->ID;
				if (Reg->HasComponent<RigidBodyComponent>(Entity) && Reg->HasComponent<SphereColliderComponent>(Entity))
				{
					UpdateEntity(Entity, DeltaTime);
				}
			}

			// Get Intersection
			for (int i = 0; i < Entities.size() - 1; i++)
			{
				EID EntityA = Entities[i]->ID;

				for (int j = i + 1; j < Entities.size(); j++)
				{
					EID EntityB = Entities[j]->ID;

					if (Reg->HasComponent<SphereColliderComponent>(EntityA) && Reg->HasComponent<SphereColliderComponent>(EntityB))
					{
						SphereColliderComponent* SphereA = Reg->GetComponent<SphereColliderComponent>(EntityA);
						SphereColliderComponent* SphereB = Reg->GetComponent<SphereColliderComponent>(EntityB);

						glm::vec3 IntersectionDepth = GetIntersection(SphereA, SphereB);

						// if dynamic : bounce
						Bounce(EntityA, EntityB, GetIntersection(SphereA, SphereB), Reg);
					}
				}
			}
		}

		glm::vec3 SphereVsSphereColliderSystem::GetIntersection(SphereColliderComponent* SphereA, SphereColliderComponent* SphereB) const
		{
			glm::vec3 centDist = SphereB->Pos - SphereA->Pos;
			float radDist = SphereB->Radius + SphereB->Radius;

			return glm::normalize(centDist) * ( glm::length(centDist) - radDist );	// intersects if negative
		}

#pragma endregion

#pragma region PlaneVsSphere

		PlaneVsSphereColliderSystem::PlaneVsSphereColliderSystem(Registry* InReg, const String& InName)
			: System(InReg, InName)
		{
		}

		void PlaneVsSphereColliderSystem::UpdateEntity(const EID Entity, double Dt) const
		{
			RigidBodyComponent* Rb = Reg->GetComponent<RigidBodyComponent>(Entity);
			PlaneColliderComponent* Plane = Reg->GetComponent<PlaneColliderComponent>(Entity);

			if (glm::length(Rb->Velocity) > 0)
			{
				//SCAR_LOG(LogWarning, "Plane should be static");
			}
		}

		void PlaneVsSphereColliderSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
			// Move collider
			for (int i = 0; i < Entities.size(); i++)
			{
				EID Entity = Entities[i]->ID;
				if (Reg->HasComponent<RigidBodyComponent>(Entity) && Reg->HasComponent<PlaneColliderComponent>(Entity))
				{
					UpdateEntity(Entity, DeltaTime);
				}
			}

			// Get Intersection
			for (int i = 0; i < Entities.size() - 1; i++)
			{
				EID EntityA = Entities[i]->ID;
				if (Reg->HasComponent<RigidBodyComponent>(EntityA) && Reg->HasComponent<SphereColliderComponent>(EntityA))
				{
					UpdateEntity(EntityA, DeltaTime);
				}

				for (int j = i + 1; j < Entities.size(); j++)
				{
					EID EntityB = Entities[j]->ID;

					SphereColliderComponent* Sphere = nullptr;
					PlaneColliderComponent* Plane = nullptr;
					RigidBodyComponent* RbSphere = nullptr;
					Transform* TransSphere = nullptr;

					if (Reg->HasComponent<PlaneColliderComponent>(EntityA) && Reg->HasComponent<SphereColliderComponent>(EntityB))
					{
						Plane = Reg->GetComponent<PlaneColliderComponent>(EntityA);
						Sphere = Reg->GetComponent<SphereColliderComponent>(EntityB);
						RbSphere = Reg->GetComponent<RigidBodyComponent>(EntityB);
						TransSphere = Reg->GetComponent<Transform>(EntityB);
					}
					else if (Reg->HasComponent<PlaneColliderComponent>(EntityB) && Reg->HasComponent<SphereColliderComponent>(EntityA))
					{
						Plane = Reg->GetComponent<PlaneColliderComponent>(EntityB);
						Sphere = Reg->GetComponent<SphereColliderComponent>(EntityA);
						RbSphere = Reg->GetComponent<RigidBodyComponent>(EntityA);
						TransSphere = Reg->GetComponent<Transform>(EntityA);
					}
					else
					{
						break;
					}

					IntersectionData* IntersectionDepth = GetIntersection(Plane, Sphere);

					if (IntersectionDepth->Distance <= 0.f && RbSphere)
					{
						glm::vec3 NewVel = glm::length(RbSphere->Velocity) == 0 ? glm::vec3(0.f, 0.f, 0.f) : 
																				  glm::reflect(RbSphere->Velocity, glm::reflect(glm::normalize(IntersectionDepth->Normal), glm::normalize(RbSphere->Velocity)));
						glm::vec3 NewPos = Plane->Normal * -IntersectionDepth->Distance;

						RbSphere->Velocity =  (1-Plane->FrictionCoefficient) * NewVel;
						TransSphere->Position += NewPos;
						Sphere->Pos += NewPos;

						if (RbSphere->UsesGravity)
						{
							RbSphere->Force -= RbSphere->GravityEarth * RbSphere->Mass;
						}
					}

					delete IntersectionDepth;
				}
			}
		}

		IntersectionData* PlaneVsSphereColliderSystem::GetIntersection(PlaneColliderComponent* Plane, SphereColliderComponent* Sphere) const
		{
			float Dist = (abs(glm::dot(Plane->Normal, Sphere->Pos) + Plane->Distance) - Sphere->Radius);
			IntersectionData* Data = new IntersectionData();
			Data->Distance = Dist;
			Data->Normal = Plane->Normal * Dist;
			return Data; // Intersects if negative
		}

#pragma endregion
	};
};
