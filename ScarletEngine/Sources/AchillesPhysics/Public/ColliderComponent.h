#pragma once
#include "Core.h"

namespace Achilles
{
	struct BoxColliderComponent
	{
		glm::vec3 Min;
		glm::vec3 Max;
	};

	struct SphereColliderComponent
	{
		glm::vec3 Pos;
		float Radius;
	};

	struct PlaneColliderComponent
	{
		glm::vec3 Normal;
		float Distance;
		float FrictionCoefficient;
	};
}