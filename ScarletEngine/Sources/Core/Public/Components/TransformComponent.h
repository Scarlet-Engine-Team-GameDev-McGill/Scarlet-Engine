#pragma once

#include "Core.h"

namespace ScarletEngine
{
	class TransformComponent final : public IComponent
	{
		COMPONENT_DEFINITION(TransformComponent)

		glm::vec3 Position = { 0.f, 0.f, 0.f };
		glm::vec3 Rotation = { 0.f, 0.f, 0.f };
		glm::vec3 Scale = { 1.f, 1.f, 1.f };

		// #todo: cache this
		/** Compute the transformation matrix for this Transform */
		glm::mat4 GetTransformMatrix() const
		{
			const glm::mat4 Identity(1.f);
			const glm::mat4 ScaleMatrix = glm::scale(Identity, Scale);
			glm::mat4 RotationMatrix = glm::rotate(Identity, glm::radians(Rotation.x), { 1, 0, 0 });
			RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Rotation.y), { 0, 1, 0 });
			RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Rotation.z), { 0, 0, 1 });
			const glm::mat4 TranslationMatrix = glm::translate(Identity, Position);
			return TranslationMatrix * RotationMatrix * ScaleMatrix;
		}
	};
}