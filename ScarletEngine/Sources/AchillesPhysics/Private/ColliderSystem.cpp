#include "ColliderSystem.h"
#include "Components/RigidBodyComponent.h"
#include "Core.h"
#include "Engine.h"

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
		const auto& Entities = GetEntities<TransformComponent, SphereColliderComponent, RigidBodyComponent>();
		const size_t Size = Entities.size();

		if (Size == 0)
		{
			return;
		}

		for (size_t i = 0; i < Size - 1; i++)
		{
			const auto& [EntityA, TransA, SphereA, RbA] = Entities[i];
			for (size_t j = i + 1; j < Size; j++)
			{
				const auto& [EntityB, TransB, SphereB, RbB] = Entities[j];
				const IntersectionData IntersectionDepth = GetIntersection(SphereA, SphereB);

				if (IntersectionDepth.Distance < 0.f)
				{
					const glm::vec3 Fi = - (glm::length(RbA->Velocity - RbB->Velocity) * IntersectionDepth.Direction) / (static_cast<float>(FIXED_UPDATE_S) * (1 / RbA->Mass + 1 / RbB->Mass));

					SolveIntersection(RbA, TransA, SphereA, SphereB, Fi, IntersectionDepth.Direction * IntersectionDepth.Distance);
					SolveIntersection(RbB, TransB, SphereB, SphereA, -Fi, -IntersectionDepth.Direction * IntersectionDepth.Distance);
				}
			}
		}
	}

	IntersectionData SphereVsSphereColliderSystem::GetIntersection(const SphereColliderComponent* SphereA, const SphereColliderComponent* SphereB) const
	{
		ZoneScoped
		const glm::vec3 CentDist = SphereB->Pos - SphereA->Pos;
		const float RadDist = SphereB->Radius + SphereA->Radius;
		IntersectionData Data;
		Data.Direction = glm::normalize(CentDist);
		Data.Distance = glm::length(CentDist) - RadDist;
		return Data;
	}

	void SphereVsSphereColliderSystem::SolveIntersection
		(RigidBodyComponent* Rb, TransformComponent* Trans, SphereColliderComponent* Sphere, SphereColliderComponent* OtherSphere, const glm::vec3 Fi, const glm::vec3 newPos) const
	{
		Rb->Force += Fi;
		Rb->Velocity *= (1.f - OtherSphere->FrictionCoefficient);
		Trans->Position += newPos;
		Sphere->Pos = Trans->Position;
		Rb->Force -= Rb->Gravity * Rb->Mass;
	}

	void PlaneVsSphereColliderSystem::FixedUpdate() const
	{
		ZoneScoped
		// Get Intersection
		for (const auto& [EntP, Plane] : GetEntities<PlaneColliderComponent>())
		{
			for (const auto& [EntS, TransSphere, Sphere, RbSphere] : GetEntities<TransformComponent, SphereColliderComponent, RigidBodyComponent>())
			{
				const IntersectionData IntersectionDepth = GetIntersection(Plane, Sphere);

				if (IntersectionDepth.Distance < 0.f && RbSphere)
				{
					const glm::vec3 NewPos = Plane->Normal * -IntersectionDepth.Direction;

					TransSphere->Position += NewPos;
					RbSphere->Velocity = (1.f - Plane->FrictionCoefficient) * glm::reflect(RbSphere->Velocity, Plane->Normal);

					Sphere->Pos += NewPos;
					RbSphere->Force -= RbSphere->Gravity * RbSphere->Mass;
				}
			}
		}
	}

	IntersectionData PlaneVsSphereColliderSystem::GetIntersection(const PlaneColliderComponent* Plane, const SphereColliderComponent* Sphere) const
	{
		const float Dist = glm::dot(Plane->Normal, Sphere->Pos) - Plane->Distance - Sphere->Radius;
		IntersectionData Data;
		Data.Direction = Plane->Normal * Dist;
		Data.Distance = Dist;
		return Data;
	}
}
