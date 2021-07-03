#include "PropertyEditor.h"

#include "ComponentTypeRegistry.h"
#include "Editor.h"
#include "Widgets.h"
#include "Components/RigidBodyComponent.h"

namespace ScarletEngine
{
    PropertyEditorPanel::PropertyEditorPanel()
        : UIWindow("Property Editor")
          , FocusedEntity()
    {
    }

    void PropertyEditorPanel::Construct()
    {
        GEditor->GetOnSelectionChanged().BindMember(this, &PropertyEditorPanel::OnSelectionChanged);
        GEditor->GetOnSelectionCleared().BindMember(this, &PropertyEditorPanel::OnSelectionCleared);
    }

    void PropertyEditorPanel::DrawWindowContent()
    {
        if (FocusedEntity)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10, ImGui::GetStyle().FramePadding.y });
            ImGui::Text("Name");
            ImGui::SameLine();
            // #todo_editor: this is disgusting and needs to be fixed.
            String& EntityName = const_cast<String&>(FocusedEntity->GetName());
            if (ImGui::InputText("###Name", EntityName.data(), EntityName.capacity(), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                SCAR_LOG(LogInfo, "Set entity name to %s", EntityName.c_str());
            }

            ImGui::Text("ID: %u", FocusedEntity->GetEntityID());

            ImGui::Separator();

            if (FocusedEntity != nullptr)
            {
                const World* OwningWorld = FocusedEntity->GetWorld();
                check(OwningWorld);

                if (TransformComponent* Transform = OwningWorld->GetComponent<TransformComponent>(FocusedEntity->GetEntityID()))
                {
                    DrawTransformWidget(*Transform);

                    ImGui::Separator();
                }


                if (Achilles::RigidBodyComponent* RigidBody = OwningWorld->GetComponent<Achilles::RigidBodyComponent>(FocusedEntity->GetEntityID()))
                {
                    DrawRigidBodyWidget(*RigidBody);

                    ImGui::Separator();
                }
            }

            if (ImGui::Button("Add Component", ImVec2(-1.f, 0.f)))
            {
                ImGui::OpenPopup("AddComponentList");
            }
            if (ImGui::BeginPopup("AddComponentList"))
            {
                DrawComponentFactoryList();
                ImGui::EndPopup();
            }
                
            ImGui::PopStyleVar(1);
        }
    }

    void PropertyEditorPanel::DrawComponentFactoryList() const
    {
        const Array<std::pair<CTID, String>>& ComponentTypes = ComponentTypeRegistry::Get().GetRegisteredComponents();
        for (const auto& [ComponentTypeID, ComponentTypeName] : ComponentTypes)
        {
            if (ImGui::MenuItem(ComponentTypeName.c_str()))
            {
                ComponentTypeRegistry::Get().CreateComponentFromTypeID(ComponentTypeID, FocusedEntity->GetEntityID(), &FocusedEntity->GetWorld()->GetRegistry());
            }
        }
    }

    void PropertyEditorPanel::DrawTransformWidget(TransformComponent& Transform) const
    {
        if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::BeginPopupContextItem("Transform Component Context Menu"))
            {
                if (ImGui::MenuItem("Delete"))
                {
                    FocusedEntity->GetWorld()->RemoveComponent<TransformComponent>(FocusedEntity->GetEntityID());
                }
                ImGui::EndPopup();
            }
            Widgets::DrawTransformInput("Transform Component", Transform);
        }
    }

    void PropertyEditorPanel::DrawRigidBodyWidget(Achilles::RigidBodyComponent& RigidBody) const
    {
        if (ImGui::CollapsingHeader("Rigidbody Component", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::BeginPopupContextItem("RigidBody Component Context Menu"))
            {
                if (ImGui::MenuItem("Delete"))
                {
                    FocusedEntity->GetWorld()->RemoveComponent<Achilles::RigidBodyComponent>(FocusedEntity->GetEntityID());
                }
                ImGui::EndPopup();
            }

            ImGui::BeginTable("RigidbodyComponentContent", 2, ImGuiTableFlags_Resizable);

            ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, Widgets::GetDefaultColumnWidth());
            ImGui::TableSetupColumn("Value");

            ImGui::TableNextColumn();
            ImGui::Text("Gravity");
            ImGui::TableNextColumn();
            Widgets::DrawVec3Input("Gravity", RigidBody.Gravity);

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Text("Mass");
            ImGui::TableNextColumn();
            ImGui::DragFloat("###Mass", &RigidBody.Mass, 0.1f, 0.0000001f, std::numeric_limits<float>::max(), "%.2f kg", ImGuiSliderFlags_AlwaysClamp);

            ImGui::EndTable();

            Widgets::DrawBooleanInput("Use Kepler Gravity", RigidBody.bUsesKeplerGravity);
        }
    }

    void PropertyEditorPanel::OnSelectionChanged()
    {
        const auto& Selection = GEditor->GetSelection();
        if (Selection.size() == 1)
        {
            FocusedEntity = *Selection.begin();
        }
        else
        {
            OnSelectionCleared();
        }
    }

    void PropertyEditorPanel::OnSelectionCleared()
    {
        FocusedEntity = nullptr;
    }
}
