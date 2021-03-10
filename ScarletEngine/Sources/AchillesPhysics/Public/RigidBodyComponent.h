#pragma once
#include "Core.h"

namespace Achilles
{
	struct RigidBodyComponent
	{
		glm::vec3 GravityEarth = glm::vec3(0.f, -9.81f, 0.f);

		float Mass;
		bool UsesGravity;

		glm::vec3 Force = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 Velocity = glm::vec3(0.f, 0.f, 0.f);
	};
};