#pragma once

#include "Core.h"

namespace ScarletEngine
{
	struct Transform
	{
	public:
		glm::vec3 Position = { 0.f, 0.f, 0.f };
		glm::vec3 Rotation = { 0.f, 0.f, 0.f };
		glm::vec3 Scale = { 1.f, 1.f, 1.f };

		// #todo: cache this
		glm::mat4 GetTransformMatrix() const
		{
			glm::mat4 Trans(1.f);
			Trans = glm::scale(Trans, Scale);
			Trans = glm::rotate(Trans, glm::radians(Rotation.x), { 1, 0, 0 });
			Trans = glm::rotate(Trans, glm::radians(Rotation.y), { 0, 1, 0 });
			Trans = glm::rotate(Trans, glm::radians(Rotation.z), { 0, 0, 1 });
			Trans = glm::translate(Trans, Position);
			return Trans;
		}
	};
}