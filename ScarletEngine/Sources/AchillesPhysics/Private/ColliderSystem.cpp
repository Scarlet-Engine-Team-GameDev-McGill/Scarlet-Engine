#include "ColliderSystem.h"
#include "RigidBodyComponent.h"
#include "Core.h"

namespace ScarletEngine::Achilles
{
	void AABBvsAABBColliderSystem::FixedUpdate() const
	{
		ZoneScoped
		// Move collider
		for (const auto& [Entity, Box, Rb] : GetEntities<BoxColliderComponent, RigidBodyComponent>())
		{
			const glm::vec3 Dx = Rb->Velocity * static_cast<float>(FIXED_UPDATE_S);
			Box->Max += Dx;
			Box->Min += Dx;
		}

		// @todo : bounce
	}

	void SphereVsSphereColliderSystem::FixedUpdate() const
	{
		ZoneScoped
		// Move collider
		for (const auto& [Entity, Rb, Sphere] : GetEntities<RigidBodyComponent, SphereColliderComponent>())
		{
			Sphere->Pos += Rb->Velocity * static_cast<float>(FIXED_UPDATE_S);
		}

		// Compute intersection
		const auto& Entities = GetEntities<Transform, SphereColliderComponent, RigidBodyComponent>();
		const size_t Size = Entities.size();
		for (size_t i = 0; i < Size - 1; i++)
		{
			const auto& [EntityA, TransA, SphereA, RbA] = Entities[i];
			for (size_t j = i + 1; j < Size; j++)
			{
				const auto& [EntityB, TransB, SphereB, RbB] = Entities[j];
				const std::pair<glm::vec3, float> IntersectionDepth = GetIntersection(SphereA, SphereB);

				if (IntersectionDepth.second < 0.f)
				{
					const glm::vec3 Fi = - (glm::length(RbA->Velocity - RbB->Velocity) * IntersectionDepth.first) / (static_cast<float>(FIXED_UPDATE_S) * (1 / RbA->Mass + 1 / RbB->Mass));

					SolveIntersection(RbA, TransA, SphereA, SphereB, Fi, IntersectionDepth.first * IntersectionDepth.second);
					SolveIntersection(RbB, TransB, SphereB, SphereA, -Fi, -IntersectionDepth.first * IntersectionDepth.second);
				}
			}
		}
	}

	std::pair<glm::vec3, float> SphereVsSphereColliderSystem::GetIntersection(const SphereColliderComponent* SphereA, const SphereColliderComponent* SphereB) const
	{
		const glm::vec3 CentDist = SphereB->Pos - SphereA->Pos;
		const float RadDist = SphereB->Radius + SphereA->Radius;
		return std::pair<glm::vec3, float>(glm::normalize(CentDist), glm::length(CentDist) - RadDist);
	}

	void SphereVsSphereColliderSystem::SolveIntersection
		(RigidBodyComponent* Rb, Transform* Trans, SphereColliderComponent* Sphere, SphereColliderComponent* OtherSphere, const glm::vec3 Fi, const glm::vec3 newPos) const
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

	void PlaneVsSphereColliderSystem::FixedUpdate() const
	{
		ZoneScoped
		// Get Intersection
		for (const auto& [EntP, Plane] : GetEntities<PlaneColliderComponent>())
		{
			for (const auto& [EntS, TransSphere, Sphere, RbSphere] : GetEntities<Transform, SphereColliderComponent, RigidBodyComponent>())
			{
				const std::pair<glm::vec3, float> IntersectionDepth = GetIntersection(Plane, Sphere);

				if (IntersectionDepth.second < 0.f && RbSphere)
				{
					const glm::vec3 NewPos = Plane->Normal * -IntersectionDepth.second;

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

	std::pair<glm::vec3, float> PlaneVsSphereColliderSystem::GetIntersection(const PlaneColliderComponent* Plane, const SphereColliderComponent* Sphere) const
	{
		const float Dist = glm::dot(Plane->Normal, Sphere->Pos) - Plane->Distance - Sphere->Radius;
		return std::pair<glm::vec3, float>(Plane->Normal * Dist, Dist); // Intersects if negative
	}
}
