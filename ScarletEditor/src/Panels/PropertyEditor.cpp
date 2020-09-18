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
		ImGui::Begin("Property Editor");

		if (bHasSelection)
		{
			ImGui::Text("Editing Entity %d", FocusedEntity.lock()->ID);

			ImGui::Separator();
			DrawTransformEditor();
			ImGui::Separator();
			ImGui::Button("Add Component");
		}

		ImGui::End();
	}

	void PropertyEditorPanel::DrawTransformEditor()
	{
		static bool bOpen = true;
		if (auto FocusedEntityPtr = FocusedEntity.lock())
		{
			Transform* TransformComponent = FocusedEntityPtr->OwningWorld->GetComponent<Transform>(*FocusedEntityPtr);
			
			if (ImGui::CollapsingHeader("Transform Component", &bOpen))
			{
				ImGui::Columns(2);

				ImGui::Text("Position");
				ImGui::Text("Rotation");
				ImGui::Text("Scale");

				ImGui::NextColumn();

				ImGui::InputFloat3("###P", (float*)&TransformComponent->Position, 3);
				ImGui::InputFloat3("###R", (float*)&TransformComponent->Rotation, 3);
				ImGui::InputFloat3("###S", (float*)&TransformComponent->Scale, 3);

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