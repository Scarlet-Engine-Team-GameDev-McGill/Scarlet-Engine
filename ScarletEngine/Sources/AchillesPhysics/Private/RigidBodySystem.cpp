#include "RigidBodySystem.h"

namespace ScarletEngine::Achilles
{
	void RigidBodySystem::UpdateEntity(Transform* Trans, RigidBodyComponent* Rb, const float Dt) const
	{
		ZoneScoped
		// translational
		if (Rb->UsesGravity)
		{
			Rb->Force += Rb->Gravity * Rb->Mass;
		}

		Rb->Velocity += Rb->Force / Rb->Mass * Dt;
		Trans->Position += Rb->Velocity * Dt;

		Rb->Force = glm::vec3(0.f, 0.f, 0.f);

		// rotational
		Rb->AngularVelocity += glm::inverse(Rb->Inertia) * (Rb->Moment - glm::cross(Rb->AngularVelocity, (Rb->Inertia * Rb->AngularVelocity))) * Dt;

		glm::quat QOrientation = glm::quat(glm::radians(Trans->Rotation)); // pitch yaw roll
		const glm::quat DQ = QOrientation * Rb->AngularVelocity * 0.5f * Dt;
		QOrientation += DQ;

		Trans->Rotation = glm::degrees(glm::eulerAngles(glm::normalize(QOrientation))); // returns XYZ (PYR) 

		Rb->Moment = glm::vec3(0.f, 0.f, 0.f);
	}	

	void RigidBodySystem::FixedUpdate() const
	{
		ZoneScoped
		const auto& Entities = GetEntities<Transform, RigidBodyComponent>();

		// Dynamics
		for (const auto& [EntityID, Trans, Rb] : Entities)
		{
			UpdateEntity(Trans, Rb, static_cast<float>(FIXED_UPDATE_S));
		}

		// Compute Gravities for Kepler
		const size_t Size = Entities.size();

		if (Size == 0)
		{
			return;
		}

		for (size_t i = 0; i < Size - 1; i++)
		{
			const auto& [EntityA, TransA, RbA] = Entities[i];
			for (size_t j = i + 1; j < Size; j++)
			{
				const auto& [EntityB, TransB, RbB] = Entities[j];
				if (RbA->UsesKeplerGravity && RbB->UsesKeplerGravity)
				{

					const glm::vec3 PosA = TransA->Position;
					const glm::vec3 PosB = TransB->Position;

					const float Dist = std::abs(glm::distance(PosA, PosB));

					if (Dist != 0)
					{
						const glm::vec3 UnitVec = (PosB - PosA) / Dist;
						const glm::vec3 GravForce = (6.14f * std::pow(10.f, -11.f) * RbA->Mass * RbB->Mass / (Dist * Dist)) * UnitVec;

						RbA->Force += GravForce;
						RbB->Force -= GravForce;
					}
				}
			}
		}
	}
}
