#include "PropertyEditor.h"

#include "Editor.h"
#include "ECS.h"

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
			ImGui::InputText("###Name", (char*)FocusedEntity->Name.c_str(), FocusedEntity->Name.capacity());

			ImGui::Text("ID: %u", FocusedEntity->ID);

			ImGui::Separator();
			
			DrawTransformEditor();
			
			ImGui::Separator();
			
			ImGui::Button("Add Component");
			ImGui::PopStyleVar(1);
		}
	}

	void PropertyEditorPanel::DrawTransformEditor()
	{
		ZoneScoped
		if (FocusedEntity != nullptr)
		{
			Transform* TransformComponent = FocusedEntity->OwningWorld->GetComponent<Transform>(*FocusedEntity);
			
			if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
			{
				static bool bFirstColumnOffsetSet = false;
				float FirstColumnOffset = ImGui::CalcTextSize("Position").x + 2 * ImGui::GetStyle().ItemSpacing.x;
				
				ImGui::Columns(2);

				if (!bFirstColumnOffsetSet)
				{
					bFirstColumnOffsetSet = true;
					ImGui::SetColumnWidth(-1, FirstColumnOffset);
				}

				ImGui::Text("Position");
				ImGui::Text("Rotation");
				ImGui::Text("Scale");

				ImGui::NextColumn();

				ImGui::InputFloat3("###Position", glm::value_ptr(TransformComponent->Position), "%.3f");
				ImGui::InputFloat3("###Rotation", glm::value_ptr(TransformComponent->Rotation), "%.3f");
				ImGui::InputFloat3("###Scale", glm::value_ptr(TransformComponent->Scale), "%.3f");

				// Restore default columns
				ImGui::Columns(1);
			}
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