#pragma once
#include "Components/TransformComponent.h"

namespace ScarletEngine::Widgets
{
    float GetDefaultColumnWidth();
    void DrawVec3Input(const char* Label, glm::vec3& Vec);
    void DrawTransformInput(const char* Label, TransformComponent& Trans);
    void DrawBooleanInput(const char* Label, bool& Boolean);
    void DrawSeparator(const char* Label = nullptr);
    bool DrawToggleButton(const char* Label, bool& Boolean);
}
