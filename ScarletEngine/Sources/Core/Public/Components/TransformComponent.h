#pragma once

#include "Core.h"

namespace ScarletEngine
{
	struct TransformComponent
	{
	public:
		glm::vec3 Position = { 0.f, 0.f, 0.f };
		glm::vec3 Rotation = { 0.f, 0.f, 0.f };
		glm::vec3 Scale = { 1.f, 1.f, 1.f };

		// #todo: cache this
		glm::mat4 GetTransformMatrix() const
		{
			glm::mat4 Identity(1.f);
			glm::mat4 ScaleMatrix = glm::scale(Identity, Scale);
			glm::mat4 RotationMatrix = glm::rotate(Identity, glm::radians(Rotation.x), { 1, 0, 0 });
			RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Rotation.y), { 0, 1, 0 });
			RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Rotation.z), { 0, 0, 1 });
			glm::mat4 TranslationMatrix = glm::translate(Identity, Position);
			return TranslationMatrix * RotationMatrix * ScaleMatrix;
		}
	};
}