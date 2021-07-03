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

    void DrawSeparator(const char* Label)
    {
        const ImVec2 WindowPos = ImGui::GetWindowPos();
        const float WindowWidth = ImGui::GetWindowWidth();
        const ImVec2 TextSize = ImGui::CalcTextSize(Label);
        
        if (Label == nullptr)
        {
            const ImVec2 StartPos = {
                WindowPos.x + ImGui::GetCursorPos().x,
                WindowPos.y + ImGui::GetCursorPos().y + TextSize.y / 2
            };

            const ImVec2 EndPos = {
                WindowPos.x + WindowWidth,
                WindowPos.y + ImGui::GetCursorPos().y + TextSize.y / 2
            };

            ImGui::GetWindowDrawList()->AddLine(StartPos, EndPos, ImGui::GetColorU32({ 0.3f, 0.3f, 0.3f, 1.f }), 2);

            ImGui::NewLine();
        }
        else
        {
            const float ItemWidth = ImGui::GetStyle().ItemSpacing.x;

            ImVec2 StartPos = {
                WindowPos.x + ImGui::GetCursorPos().x,
                WindowPos.y + ImGui::GetCursorPos().y + TextSize.y / 2
            };

            ImVec2 EndPos = {
                WindowPos.x + ImGui::GetCursorPos().x + ImGui::GetWindowWidth() / 2 - TextSize.x / 2 - 2 * ItemWidth,
                WindowPos.y + ImGui::GetCursorPos().y + TextSize.y / 2
            };

            ImGui::GetWindowDrawList()->AddLine(StartPos, EndPos, ImGui::GetColorU32({ 0.3f, 0.3f, 0.3f, 1.f }), 2);

            // Since we're going to use SameLine to set the x-offset, we need to ensure a newline is created here
            ImGui::NewLine();
            ImGui::SameLine(EndPos.x - WindowPos.x + 2 * ItemWidth);
            ImGui::Text("%s", Label);

            StartPos.x = EndPos.x + TextSize.x + 4 * ItemWidth;
            EndPos.x = WindowPos.x + WindowWidth;

            ImGui::GetWindowDrawList()->AddLine(StartPos, EndPos, ImGui::GetColorU32({ 0.3f, 0.3f, 0.3f, 1.f }), 2);
        }
    }

    bool DrawToggleButton(const char* Label, bool& Boolean)
    {
        if (Boolean)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
        }
        else
        {
            // Push a dummy value so the next call to Pop is ignored.
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_Button));
        }

        bool bStateChanged = false;
        if (ImGui::Button(Label))
        {
            bStateChanged = true;
            Boolean = !Boolean;
        }

        ImGui::PopStyleColor();

        return bStateChanged;
    }

    bool DrawTextInput(const char* Label, String& Output, ImGuiInputTextFlags Flags)
    {
        static auto Callback = +[](ImGuiInputTextCallbackData* Data)
        {
            String* Str = static_cast<String*>(Data->UserData);
            if (Data->EventFlag == ImGuiInputTextFlags_CallbackResize)
            {
                // Resize string callback
                // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
                IM_ASSERT(Data->Buf == Str->c_str());
                Str->resize(Data->BufTextLen);
                Data->Buf = const_cast<char*>(Str->c_str());
            }
            return 0;
        };

        return ImGui::InputText(Label, const_cast<char*>(Output.c_str()), Output.capacity(), Flags | ImGuiInputTextFlags_CallbackResize, Callback, &Output);
    }
}
