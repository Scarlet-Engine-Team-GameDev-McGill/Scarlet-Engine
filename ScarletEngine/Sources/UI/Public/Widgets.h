#pragma once
#include "Transform.h"

namespace ScarletEngine::Widgets
{
    void DrawVec3Input(const char* Label, glm::vec3& Vec);
    void DrawTransformInput(const char* Label, Transform& Trans);
}
