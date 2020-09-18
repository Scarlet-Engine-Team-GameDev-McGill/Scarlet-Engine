#include "Panels/PropertyEditor.h"

#include <imgui.h>
#include "Editor.h"

namespace ScarletEngine
{
	PropertyEditorPanel::PropertyEditorPanel()
		: FocusedEntity()
		, bHasSelection(false)
	{
		Editor::Get().OnSelectionChangedEvent.Bind(this, &PropertyEditorPanel::OnSelectionChanged);
		Editor::Get().OnSelectionCleared.Bind(this, &PropertyEditorPanel::OnSelectionCleared);
	}

	void PropertyEditorPanel::Draw()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10, ImGui::GetStyle().FramePadding.y });

		ImGui::Begin("Property Editor");

		if (bHasSelection)
		{
			ImGui::Text("Name");
			ImGui::SameLine();
			ImGui::InputText("###Name", &FocusedEntity.lock()->Name);

			ImGui::Text("ID: %d", FocusedEntity.lock()->ID);

			ImGui::Separator();
			
			DrawTransformEditor();
			
			ImGui::Separator();
			
			ImGui::Button("Add Component");
		}

		ImGui::End();

		ImGui::PopStyleVar(1);
	}

	void PropertyEditorPanel::DrawTransformEditor()
	{
		static bool bOpen = true;
		if (auto FocusedEntityPtr = FocusedEntity.lock())
		{
			Transform* TransformComponent = FocusedEntityPtr->OwningWorld->GetComponent<Transform>(*FocusedEntityPtr);
			
			if (ImGui::CollapsingHeader("Transform Component", &bOpen, ImGuiTreeNodeFlags_DefaultOpen))
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

				ImGui::InputFloat3("###Position", glm::value_ptr(TransformComponent->Position), 3);
				ImGui::InputFloat3("###Rotation", glm::value_ptr(TransformComponent->Rotation), 3);
				ImGui::InputFloat3("###Scale", glm::value_ptr(TransformComponent->Scale), 3);

				// Restore default columns
				ImGui::Columns(1);
			}
		}
	}

	void PropertyEditorPanel::OnSelectionChanged(const std::shared_ptr<Entity>& Ent)
	{
		FocusedEntity = Ent;
		bHasSelection = true;
	}
	
	void PropertyEditorPanel::OnSelectionCleared()
	{
		bHasSelection = false;
		FocusedEntity.reset();
	}
}