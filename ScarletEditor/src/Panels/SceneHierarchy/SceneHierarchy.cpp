#include "Panels/SceneHierarchy/SceneHierarchy.h"

#include "Core/Core.h"
#include "World.h"
#include "ECS/ECS.h"

#include "Editor.h"

#include "AssetManager/AssetManager.h"
#include "Renderer/Renderer.h"
#include "RAL/RAL.h"

namespace ScarletEngine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<World>& InRepresentingWorld)
		: RepresentingWorld(InRepresentingWorld)
	{
		RepresentingWorld.lock()->GetOnEntityAddedToWorldEvent().Bind(this, &SceneHierarchyPanel::OnEntityAddedToWorld);
		Editor::Get().GetOnSelectionChanged().Bind(this, &SceneHierarchyPanel::OnWorldSelectionChanged);
		RepopulateItems();
	}

	void SceneHierarchyPanel::Draw()
	{
		ImGui::Begin(ICON_MD_ACCOUNT_TREE " Scene Hierarchy");

		ImGuiTreeNodeFlags BaseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		for (const auto& [ID, EntItem] : Items)
		{
			ImGui::PushID((uint32_t)ID);
			char Buffer[128];
			snprintf(Buffer, 128, "%s %s", u8"\ue9fe", EntItem->GetDisplayString());
			
			ImGuiTreeNodeFlags Flags = BaseFlags;
			const bool bIsAlreadySelected = EntItem->bIsSelected;
			if (bIsAlreadySelected)
			{
				Flags |= ImGuiTreeNodeFlags_Selected;
			}

			if (EntItem->Children.size() > 0)
			{
				bool bNodeOpen = ImGui::TreeNodeEx(Buffer, Flags);
				if (ImGui::IsItemClicked() && !bIsAlreadySelected)
				{
					EntItem->bIsSelected = true;
					Editor::Get().SetSelection(EntItem->Ent.lock());
				}

				if (bNodeOpen)
				{
					ImGui::TreePop();
				}
			}
			else
			{
				Flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				ImGui::TreeNodeEx(Buffer, Flags);
				if (ImGui::IsItemClicked() && !bIsAlreadySelected)
				{
					EntItem->bIsSelected = true;
					Editor::Get().SetSelection(EntItem->Ent.lock());
				}
			}

			

			ImGui::PopID();
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
		// For now just clear all old selection and add the newly selected
		for (const auto& [ID, EntItem] : Items)
		{
			EntItem->bIsSelected = false;
		}
		if (auto It = Items.find(LastSelectedEntity->ID); It != Items.end())
		{
			It->second->bIsSelected = true;
		}
	}
}