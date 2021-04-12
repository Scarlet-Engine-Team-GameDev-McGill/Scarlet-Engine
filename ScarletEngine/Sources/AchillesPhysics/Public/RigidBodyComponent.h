#pragma once
#include "Core.h"

namespace ScarletEngine::Achilles
{
	struct RigidBodyComponent
	{
		glm::mat3x3 Inertia = glm::mat3x3(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
		glm::vec3 Gravity = glm::vec3(0.f, -9.81f, 0.f);
		glm::vec3 Force = glm::vec3(0.f);
		glm::vec3 Velocity = glm::vec3(0.f);
		glm::vec3 Moment = glm::vec3(0.f);
		glm::vec3 AngularVelocity = glm::vec3(0.f);
		float Mass = 1.f;
		bool bUsesKeplerGravity = false;
	};
}
