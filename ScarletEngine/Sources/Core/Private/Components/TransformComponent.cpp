#include "Components/TransformComponent.h"

namespace ScarletEngine
{
    BEGIN_REFLECTION_INFO(TransformComponent)
        Property(&TransformComponent::Position, "Position");
        Property(&TransformComponent::Rotation, "Rotation");
        Property(&TransformComponent::Scale, "Scale");
    END_REFLECTION_INFO()

    glm::mat4 TransformComponent::GetTransformMatrix() const
    {
        const glm::mat4 Identity(1.f);
        const glm::mat4 ScaleMatrix = glm::scale(Identity, Scale);
        glm::mat4 RotationMatrix = glm::rotate(Identity, glm::radians(Rotation.x), { 1, 0, 0 });
        RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Rotation.y), { 0, 1, 0 });
        RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Rotation.z), { 0, 0, 1 });
        const glm::mat4 TranslationMatrix = glm::translate(Identity, Position);
        return TranslationMatrix * RotationMatrix * ScaleMatrix;
    }
}
