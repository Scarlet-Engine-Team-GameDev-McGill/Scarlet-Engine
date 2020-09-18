#include "Panels/SceneHierarchy.h"

#include "Editor.h"
#include "World.h"
#include "ECS/ECS.h"
#include <imgui.h>

namespace ScarletEngine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<World>& InRepresentingWorld)
		: RepresentingWorld(InRepresentingWorld)
	{
	}

	void SceneHierarchyPanel::Draw()
	{
		ImGui::Begin("Scene Hierarchy");

		if (ImGui::TreeNode("World"))
		{
			for (const auto& Ent : RepresentingWorld.lock()->GetEntities())
			{
				EID EntID = Ent->ID;
				ImGui::PushID((uint32_t)EntID);
				if (ImGui::TreeNodeEx(Ent->Name.c_str()))
				{
					ImGui::Text("Entity ID: %d", EntID);

					ImGui::TreePop();
					if (ImGui::IsItemClicked())
					{
						Editor::Get().SetSelection(Ent);
					}
				}

				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		/*
		if (ImGui::IsItemClicked())
		{
			Editor::Get().ClearSelection();
		}
		*/
		ImGui::End();
	}
}