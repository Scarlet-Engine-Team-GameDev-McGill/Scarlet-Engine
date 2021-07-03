#pragma once
#include "Core.h"

namespace ScarletEngine::Achilles
{
	class BoxColliderComponent final : public IComponent
	{
		COMPONENT_DEFINITION(Box Collider);

		glm::vec3 Min;
		glm::vec3 Max;
		float FrictionCoefficient = 0.01f;
	};

	class SphereColliderComponent final : public IComponent
	{
		COMPONENT_DEFINITION(Sphere Collider);

		glm::vec3 Pos;
		float Radius;
		float FrictionCoefficient = 0.01f;
	};

	class PlaneColliderComponent final : public IComponent
	{
		COMPONENT_DEFINITION(Plane Collider);

		glm::vec3 Normal;
		float Distance;
		float FrictionCoefficient = 0.01f;
	};
}
