#include "PropertyEditor.h"

#include "Editor.h"
#include "ECS.h"
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
		ZoneScoped
		GEditor->GetOnSelectionChanged().BindMember(this, &PropertyEditorPanel::OnSelectionChanged);
		GEditor->GetOnSelectionCleared().BindMember(this, &PropertyEditorPanel::OnSelectionCleared);
	}

	void PropertyEditorPanel::DrawWindowContent()
	{
		ZoneScoped
		if (FocusedEntity)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10, ImGui::GetStyle().FramePadding.y });
			ImGui::Text("Name");
			ImGui::SameLine();
			ImGui::InputText("###Name", FocusedEntity->Name.data(), FocusedEntity->Name.capacity());

			ImGui::Text("ID: %u", FocusedEntity->ID);

			ImGui::Separator();
			
			if (FocusedEntity != nullptr)
			{
				World* OwningWorld = FocusedEntity->OwningWorld;
				check(OwningWorld);
				
				if (TransformComponent* Transform = OwningWorld->GetComponent<TransformComponent>(*FocusedEntity))
				{
					DrawTransformWidget(*Transform);
				}

				ImGui::Separator();

				if (Achilles::RigidBodyComponent* RigidBody = OwningWorld->GetComponent<Achilles::RigidBodyComponent>(*FocusedEntity))
				{
					DrawRigidBodyWidget(*RigidBody);
				}
			}

			ImGui::Separator();
			
			ImGui::Button("Add Component");
			ImGui::PopStyleVar(1);
		}
	}

	void PropertyEditorPanel::DrawTransformWidget(TransformComponent& Transform) const
	{
		ZoneScoped
		Widgets::DrawTransformInput("Transform Component", Transform);
	}

	void PropertyEditorPanel::DrawRigidBodyWidget(Achilles::RigidBodyComponent& RigidBody) const
	{
		if (ImGui::CollapsingHeader("Rigidbody Component", ImGuiTreeNodeFlags_DefaultOpen))
		{
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
		ZoneScoped
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
		ZoneScoped
		FocusedEntity = nullptr;
	}
}
