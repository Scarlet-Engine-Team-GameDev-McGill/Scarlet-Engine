#include "RigidBodySystem.h"

namespace ScarletEngine::Achilles
{
	void RigidBodySystem::Update() const
	{
	}

	void RigidBodySystem::UpdateEntity(const EID Entity, Transform* Trans, RigidBodyComponent* Rb, float Dt) const
	{
		// translational
		if (Rb->UsesGravity)
		{
			Rb->Force += Rb->GravityEarth * Rb->Mass;
		}

		Rb->Velocity += Rb->Force / Rb->Mass * Dt;
		Trans->Position += Rb->Velocity * Dt;

		Rb->Force = glm::vec3(0.f, 0.f, 0.f);

		// rotational
		Rb->AngularVelocity += glm::inverse(Rb->Inertia) * (Rb->Moment - glm::cross(Rb->AngularVelocity, (Rb->Inertia * Rb->AngularVelocity))) * Dt;

		glm::quat QOrientation = glm::quat(glm::radians(Trans->Rotation)); // pitch yaw roll
		glm::quat DQ = QOrientation * Rb->AngularVelocity * 0.5f * Dt;
		QOrientation += DQ;

		Trans->Rotation = glm::degrees(glm::eulerAngles(glm::normalize(QOrientation))); // returns XYZ (PYR) 

		Rb->Moment = glm::vec3(0.f, 0.f, 0.f);
	}	

	void RigidBodySystem::FixedUpdate() const
	{
		const auto Entities = GetEntities<Transform, RigidBodyComponent>();

		// Dynamics
		for (const auto [EntityID, Trans, Rb] : Entities)
		{
			UpdateEntity(EntityID, Trans, Rb, FIXED_UPDATE_S);
		}

		// Compute Gravities for Kepler
		int size = Entities.size();
		for (int i = 0; i < size - 1; i++)
		{
			const auto [EntityA, TransA, RbA] = Entities.at(i);
			for (int j = i + 1; j < size; j++)
			{
				const auto [EntityB, TransB, RbB] = Entities.at(j);

				if (RbA->UsesKeplerGravity && RbB->UsesKeplerGravity)
				{

					glm::vec3 posA = TransA->Position;
					glm::vec3 posB = TransB->Position;

					float d = abs(glm::distance(posA, posB));

					if (d != 0)
					{
						glm::vec3 u = (posB - posA) / d;

						float mA = RbA->Mass;
						float mB = RbB->Mass;

						glm::vec3 G = 6.14f * pow(10.f, -11.f) * mA * mB * u / pow(d, 2.f);

						RbA->Force += G;
						RbB->Force -= G;
					}
				}
			}
		}
	}
}
