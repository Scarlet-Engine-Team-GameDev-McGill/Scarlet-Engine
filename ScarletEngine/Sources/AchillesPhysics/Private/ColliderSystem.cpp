#include "ColliderSystem.h"
#include "RigidBodyComponent.h"
#include "Core.h"

namespace ScarletEngine::Achilles
{
	void AABBvsAABBColliderSystem::Update() const
	{
	}

	void AABBvsAABBColliderSystem::FixedUpdate() const
	{
		// Move collider
		for (const auto& [Entity, Box, Rb] : GetEntities<BoxColliderComponent, RigidBodyComponent>())
		{
			glm::vec3 Dx = Rb->Velocity * (float)FIXED_UPDATE_S;
			Box->Max += Dx;
			Box->Min += Dx;
		}

		// @todo : bounce
	}

	void SphereVsSphereColliderSystem::Update() const
	{
	}

	void SphereVsSphereColliderSystem::FixedUpdate() const
	{
		// Move collider
		for (const auto& [Entity, Rb, Sphere] : GetEntities<RigidBodyComponent, SphereColliderComponent>())
		{
			Sphere->Pos += Rb->Velocity * (float)FIXED_UPDATE_S;
		}

		// Compute intersection
		const auto& Entities = GetEntities<Transform, SphereColliderComponent, RigidBodyComponent>();
		int size = Entities.size();
		for (int i = 0; i < size - 1; i++)
		{
			const auto& [EntityA, TransA, SphereA, RbA] = Entities.at(i);
			for (int j = i + 1; j < size; j++)
			{
				const auto& [EntityB, TransB, SphereB, RbB] = Entities.at(j);
				std::pair<glm::vec3, float> IntersectionDepth = GetIntersection(SphereA, SphereB);

				if (IntersectionDepth.second < 0.f)
				{
					glm::vec3 Fi = - (glm::length(RbA->Velocity - RbB->Velocity) * IntersectionDepth.first) / ((float)FIXED_UPDATE_S * (1 / RbA->Mass + 1 / RbB->Mass));

					SolveIntersection(RbA, TransA, SphereA, SphereB, Fi, IntersectionDepth.first * IntersectionDepth.second);
					SolveIntersection(RbB, TransB, SphereB, SphereA, -Fi, -IntersectionDepth.first * IntersectionDepth.second);
				}
			}
		}
	}

	std::pair<glm::vec3, float> SphereVsSphereColliderSystem::GetIntersection(SphereColliderComponent* SphereA, SphereColliderComponent* SphereB) const
	{
		glm::vec3 centDist = SphereB->Pos - SphereA->Pos;
		float radDist = SphereB->Radius + SphereA->Radius;
		return std::pair<glm::vec3, float>(glm::normalize(centDist), glm::length(centDist) - radDist);
	}

	void SphereVsSphereColliderSystem::SolveIntersection
		(RigidBodyComponent* Rb, Transform* Trans, SphereColliderComponent* Sphere, SphereColliderComponent* OtherSphere, glm::vec3 Fi, glm::vec3 newPos) const
	{
		Rb->Force += Fi;
		Rb->Velocity *= (1.f - OtherSphere->FrictionCoefficient);
		Trans->Position += newPos;
		Sphere->Pos = Trans->Position;
		if (Rb->UsesGravity)
		{
			Rb->Force -= Rb->GravityEarth * Rb->Mass;
		}
	}

	void PlaneVsSphereColliderSystem::Update() const
	{
	}

	void PlaneVsSphereColliderSystem::FixedUpdate() const
	{
		// Get Intersection
		for (const auto& [EntP, Plane] : GetEntities<PlaneColliderComponent>())
		{
			for (const auto& [EntS, TransSphere, Sphere, RbSphere] : GetEntities<Transform, SphereColliderComponent, RigidBodyComponent>())
			{
				std::pair<glm::vec3, float> IntersectionDepth = GetIntersection(Plane, Sphere);

				if (IntersectionDepth.second < 0.f && RbSphere)
				{
					glm::vec3 NewPos = Plane->Normal * -IntersectionDepth.second;

					TransSphere->Position += NewPos;
					RbSphere->Velocity = (1.f - Plane->FrictionCoefficient) * glm::reflect(RbSphere->Velocity, Plane->Normal);

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
		float Dist = glm::dot(Plane->Normal, Sphere->Pos) - Plane->Distance - Sphere->Radius;
		return std::pair<glm::vec3, float>(Plane->Normal * Dist, Dist); // Intersects if negative
	}
}
