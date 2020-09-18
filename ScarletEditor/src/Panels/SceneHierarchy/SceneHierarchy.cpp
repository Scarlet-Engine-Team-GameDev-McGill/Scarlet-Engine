#include "Panels/SceneHierarchy/SceneHierarchy.h"

#include "Editor.h"
#include "World.h"
#include "ECS/ECS.h"
#include <imgui.h>

namespace ScarletEngine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<World>& InRepresentingWorld)
		: RepresentingWorld(InRepresentingWorld)
		, SelectedEntityID(INVALID_EID)
	{
		RepresentingWorld.lock()->GetOnEntityAddedToWorldEvent().Bind(this, &SceneHierarchyPanel::OnEntityAddedToWorld);
		Editor::Get().OnSelectionChangedEvent.Bind(this, &SceneHierarchyPanel::OnWorldSelectionChanged);
		RepopulateItems();
	}

	void SceneHierarchyPanel::Draw()
	{
		ImGui::Begin("Scene Hierarchy");

		if (ImGui::TreeNodeEx("World", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (const auto& [ID, EntItem] : Items)
			{
				ImGui::PushID((uint32_t)ID);
				if (ImGui::Selectable(EntItem->GetDisplayString(), ID == SelectedEntityID))
				{
					Editor::Get().SetSelection(EntItem->Ent.lock());
				}

				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::RepopulateItems()
	{
		for (const auto& Ent : RepresentingWorld.lock()->GetEntities())
		{
			if (Items.find(Ent->ID) == Items.end())
			{
				Items.emplace(Ent->ID, new SceneHierarchyItem(Ent));
			}
		}
	}

	void SceneHierarchyPanel::OnEntityAddedToWorld(const EntityPtr& AddedEntity)
	{
		// If the item is not in the map, emplace it
		if (Items.find(AddedEntity->ID) == Items.end())
		{
			Items.emplace(AddedEntity->ID, new SceneHierarchyItem(AddedEntity));
		}
	}

	void SceneHierarchyPanel::OnWorldSelectionChanged(const EntityPtr& LastSelectedEntity)
	{
		SelectedEntityID = LastSelectedEntity->ID;
	}
}