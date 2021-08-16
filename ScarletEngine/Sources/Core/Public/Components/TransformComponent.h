#pragma once

#include "Core.h"

namespace ScarletEngine
{
    class TransformComponent final : public IComponent
    {
        COMPONENT_DEFINITION(Transform)
        REFLECTION();

        glm::vec3 Position = { 0.f, 0.f, 0.f };
        glm::vec3 Rotation = { 0.f, 0.f, 0.f };
        glm::vec3 Scale = { 1.f, 1.f, 1.f };

        // #todo: cache this
        /** Compute the transformation matrix for this Transform */
        glm::mat4 GetTransformMatrix() const;
    };
}
