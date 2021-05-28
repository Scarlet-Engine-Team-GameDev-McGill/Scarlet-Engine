#include "Widgets.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace ScarletEngine::Widgets
{
    float GetDefaultColumnWidth()
    {
        return ImGui::CalcTextSize("XXXXXXXXXXXX").x + 2 * ImGui::GetStyle().ItemSpacing.x;
    }

    void DrawVec3Input(const char* Label, glm::vec3& Vec)
    {
        ImGui::PushID(Label);
        ImVec2 Size = ImGui::CalcTextSize("X");
        Size.x += GImGui->Style.FramePadding.x * 2;
        Size.y += GImGui->Style.FramePadding.y * 2;

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth() * 0.85f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, GImGui->Style.ItemSpacing.y });

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::Button("X", Size);
        ImGui::SameLine();
        ImGui::DragFloat("###X", &Vec.x, -0.1f, 0.f, 0.f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::Dummy(ImVec2(GImGui->Style.FramePadding.x, 0.f));
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::Button("Y", Size);
        ImGui::SameLine();
        ImGui::DragFloat("###Y", &Vec.y, -0.1f, 0.f, 0.f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::Dummy(ImVec2(GImGui->Style.FramePadding.x, 0.f));
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::Button("Z", Size);
        ImGui::SameLine();
        ImGui::DragFloat("###Z", &Vec.z, -0.1f, 0.f, 0.f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleColor(3);

        ImGui::PopStyleVar();
        ImGui::PopID();
    }

    void DrawTransformInput(const char* Label, TransformComponent& Trans)
    {
        if (ImGui::CollapsingHeader(Label, ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::PushID(Label);
            ImGui::BeginTable(Label, 2, ImGuiTableFlags_Resizable);

            ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, GetDefaultColumnWidth());
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthAlwaysAutoResize);

            ImGui::TableNextColumn();
            ImGui::Text("Position");
            ImGui::TableNextColumn();
            DrawVec3Input("Position", Trans.Position);

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Text("Rotation");
            ImGui::TableNextColumn();
            DrawVec3Input("Rotation", Trans.Rotation);

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Text("Scale");
            ImGui::TableNextColumn();
            DrawVec3Input("Scale", Trans.Scale);

            // Restore default columns
            ImGui::EndTable();
            ImGui::PopID();
        }
    }

    void DrawBooleanInput(const char* Label, bool& Boolean)
    {
        ImGui::Text("%s", Label);
        ImGui::PushID(Label);
        const ImVec2 WindowPos = ImGui::GetWindowPos();
        ImVec2 StartPos = {
            WindowPos.x + ImGui::GetCursorPos().x + ImGui::CalcTextSize(Label).x + 2 * ImGui::GetStyle().
            ItemSpacing.x,
            WindowPos.y + ImGui::GetCursorPos().y
        };
        ImGui::SameLine(ImGui::GetWindowWidth() - 50);
        ImVec2 EndPos = {
            ImGui::GetWindowPos().x + ImGui::GetCursorPos().x - 2 * ImGui::GetStyle().ItemSpacing.x,
            WindowPos.y + ImGui::GetCursorPos().y
        };
        const float MiddleY = (EndPos.y + StartPos.y) / 2;
        StartPos.y = EndPos.y = MiddleY;
        ImGui::GetWindowDrawList()->AddLine(StartPos, EndPos, ImGui::GetColorU32({ 0.2f, 0.2f, 0.2f, 1.f }), 2);
        ImGui::Checkbox("", &Boolean);
        ImGui::PopID();
    }
}
