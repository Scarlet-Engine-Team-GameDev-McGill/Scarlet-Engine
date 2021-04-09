#include "PropertyEditor.h"

#include "Editor.h"
#include "ECS.h"
#include "Widgets.h"

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

			ImGui::Text("ID: %lu", FocusedEntity->ID);

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
			if (Transform* TransformComponent = FocusedEntity->OwningWorld->GetComponent<Transform>(*FocusedEntity))
			{
				Widgets::DrawTransformInput("Transform Component", *TransformComponent);
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
