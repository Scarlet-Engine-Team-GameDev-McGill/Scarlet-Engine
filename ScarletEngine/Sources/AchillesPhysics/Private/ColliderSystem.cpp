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

		void AABBvsAABBColliderSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
		}

		void AABBvsAABBColliderSystem::UpdateEntity(const EID Entity, double Dt) const
		{
			RigidBodyComponent* Rb = Reg->GetComponent<RigidBodyComponent>(Entity);
			BoxColliderComponent* Box = Reg->GetComponent<BoxColliderComponent>(Entity);

			Box->Max += Rb->Velocity * (float)Dt;
			Box->Min += Rb->Velocity * (float)Dt;
		}

		void AABBvsAABBColliderSystem::FixedUpdate(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
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

						// TODO if dynamic : bounce
					}
				}
			}
		}

		// TODO : adapt with IntersectData
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

		void SphereVsSphereColliderSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
		}

		void SphereVsSphereColliderSystem::UpdateEntity(const EID Entity, double Dt) const
		{
			RigidBodyComponent* Rb = Reg->GetComponent<RigidBodyComponent>(Entity);
			SphereColliderComponent* Sphere = Reg->GetComponent<SphereColliderComponent>(Entity);

			Sphere->Pos += Rb->Velocity * (float)Dt;
		}

		void SphereVsSphereColliderSystem::FixedUpdate(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
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

						std::pair<glm::vec3, float> IntersectionDepth = GetIntersection(SphereA, SphereB);

						// if dynamic : bounce
						if (IntersectionDepth.second <= 0.f && Reg->HasComponent<RigidBodyComponent>(EntityB) && Reg->HasComponent<RigidBodyComponent>(EntityA))
						{
							RigidBodyComponent* RbA = Reg->GetComponent<RigidBodyComponent>(EntityA);
							RigidBodyComponent* RbB = Reg->GetComponent<RigidBodyComponent>(EntityB);

							Transform* TransA = Reg->GetComponent<Transform>(EntityA);
							Transform* TransB = Reg->GetComponent<Transform>(EntityB);

							glm::vec3 RelativeVel = glm::length(RbA->Velocity - RbB->Velocity) * IntersectionDepth.first;

							glm::vec3 Fi = -RelativeVel / ((float)DeltaTime * (1/RbA->Mass + 1/RbB->Mass));

							RbA->Force += Fi * (2.f - SphereB->FrictionCoefficient);
							RbB->Force -= Fi * (2.f - SphereA->FrictionCoefficient);

							TransA->Position += IntersectionDepth.first * IntersectionDepth.second;
							SphereA->Pos = TransA->Position;

							TransA->Position -= IntersectionDepth.first * IntersectionDepth.second;
							SphereA->Pos = TransA->Position;
						}
					}
				}
			}
		}

		std::pair<glm::vec3, float> SphereVsSphereColliderSystem::GetIntersection(SphereColliderComponent* SphereA, SphereColliderComponent* SphereB) const
		{
			glm::vec3 centDist = SphereB->Pos - SphereA->Pos;
			float radDist = SphereB->Radius + SphereB->Radius;
			return std::pair<glm::vec3, float>(glm::normalize(centDist), glm::length(centDist) - radDist);
		}

#pragma endregion

#pragma region PlaneVsSphere

		PlaneVsSphereColliderSystem::PlaneVsSphereColliderSystem(Registry* InReg, const String& InName)
			: System(InReg, InName)
		{
		}

		void PlaneVsSphereColliderSystem::Update(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
		{
		}

		void PlaneVsSphereColliderSystem::UpdateEntity(const EID Entity, double Dt) const
		{
		}

		void PlaneVsSphereColliderSystem::FixedUpdate(const Array<SharedPtr<Entity>>& Entities, double DeltaTime) const
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

					std::pair<glm::vec3, float> IntersectionDepth = GetIntersection(Plane, Sphere);

					if (IntersectionDepth.second <= 0.f && RbSphere)
					{
						glm::vec3 Reflect = glm::reflect(RbSphere->Velocity, glm::reflect(glm::normalize(IntersectionDepth.first), glm::normalize(RbSphere->Velocity)));
						glm::vec3 NewVel = glm::length(RbSphere->Velocity) == 0 ? glm::vec3(0.f, 0.f, 0.f) : Reflect;
																				 ;
						glm::vec3 NewPos = Plane->Normal * -IntersectionDepth.second;

						RbSphere->Velocity =  (1-Plane->FrictionCoefficient) * NewVel;
						TransSphere->Position += NewPos;
						Sphere->Pos += NewPos;

						if (RbSphere->UsesGravity)
						{
							RbSphere->Force -= RbSphere->GravityEarth * RbSphere->Mass;
						}
					}
				}
			}
		}

		std::pair<glm::vec3, float> PlaneVsSphereColliderSystem::GetIntersection(PlaneColliderComponent* Plane, SphereColliderComponent* Sphere) const
		{
			float Dist = (abs(glm::dot(Plane->Normal, Sphere->Pos) + Plane->Distance) - Sphere->Radius);
			return std::pair<glm::vec3, float>(Plane->Normal * Dist, Dist); // Intersects if negative
		}
#pragma endregion
	};
};
