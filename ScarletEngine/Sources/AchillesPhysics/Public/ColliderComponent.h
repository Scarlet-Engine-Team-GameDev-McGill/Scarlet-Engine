#pragma once
#include "Core.h"

namespace ScarletEngine::Achilles
{
	struct BoxColliderComponent
	{
		glm::vec3 Min;
		glm::vec3 Max;
		float FrictionCoefficient = 0.01f;
	};

	struct SphereColliderComponent
	{
		glm::vec3 Pos;
		float Radius;
		float FrictionCoefficient = 0.01f;
	};

	struct PlaneColliderComponent
	{
		glm::vec3 Normal;
		float Distance;
		float FrictionCoefficient = 0.01f;
	};
}
