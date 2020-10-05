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
	SceneHierarchyPanel::SceneHierarchyPanel(const SharedPtr<World>& InRepresentingWorld)
		: RepresentingWorld(InRepresentingWorld)
		, Items()
		, CurrentSelectionIndex(INVALID_EID)
	{
	}

	void SceneHierarchyPanel::Initialize()
	{
		ZoneScoped
		RepresentingWorld.lock()->GetOnEntityAddedToWorldEvent().Bind(this, &SceneHierarchyPanel::OnEntityAddedToWorld);
		GEditor->GetOnSelectionChanged().Bind(this, &SceneHierarchyPanel::OnWorldSelectionChanged);
		RepopulateItems();
	}

	void SceneHierarchyPanel::Draw()
	{
		ZoneScoped
		ImGui::Begin(ICON_MD_ACCOUNT_TREE " Scene Hierarchy");

		ImGuiTreeNodeFlags BaseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		for (const auto& [ID, EntItem] : Items)
		{
			ImGui::PushID((uint32_t)ID);
			char Buffer[128];
			snprintf(Buffer, 128, "%s %s", ICON_MD_CUBE, EntItem->GetDisplayString());
			
			ImGuiTreeNodeFlags Flags = BaseFlags;
			if (EntItem->bIsSelected)
			{
				Flags |= ImGuiTreeNodeFlags_Selected;
			}

			if (EntItem->Children.size() > 0)
			{
				bool bNodeOpen = ImGui::TreeNodeEx(Buffer, Flags);
				if (ImGui::IsItemClicked())
				{
					if (SelectItem(*EntItem))
					{
						CurrentSelectionIndex = ID;
					}
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
				if (ImGui::IsItemClicked())
				{
					if (SelectItem(*EntItem))
					{
						CurrentSelectionIndex = ID;
					}
				}
			}

			

			ImGui::PopID();
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::RepopulateItems()
	{
		ZoneScoped
		for (const auto& Ent : RepresentingWorld.lock()->GetEntities())
		{
			if (Items.find(Ent->ID) == Items.end())
			{
				Items.emplace(Ent->ID, GlobalAllocator<SceneHierarchyItem>::New(Ent));
			}
		}
	}

	void SceneHierarchyPanel::OnEntityAddedToWorld(const EntityPtr& AddedEntity)
	{
		ZoneScoped
		// If the item is not in the map, emplace it
		if (Items.find(AddedEntity->ID) == Items.end())
		{
			Items.emplace(AddedEntity->ID, GlobalAllocator<SceneHierarchyItem>::New(AddedEntity));
		}
	}

	void SceneHierarchyPanel::OnWorldSelectionChanged()
	{
		ZoneScoped
		SynchronizeSelection();
	}

	void SceneHierarchyPanel::SynchronizeSelection()
	{
		ZoneScoped
		// We need to resynchronize the entire selection when something changes since we may have selected multiple
		for (const auto& [ID, EntItem] : Items)
		{
			EntItem->bIsSelected = GEditor->IsEntitySelected(EntItem->Ent.lock().get());
		}
	}

	bool SceneHierarchyPanel::SelectItem(const SceneHierarchyItem& Item)
	{
		ZoneScoped
		const ImGuiIO& IO = ImGui::GetIO();
		if (IO.KeyCtrl)
		{
			// If the item is already selected we need to remove it
			if (Item.bIsSelected)
			{
				GEditor->RemoveFromSelection(Item.Ent.lock().get());
				return false;
			}
			else
			{
				GEditor->AddToSelection(Item.Ent.lock().get());
			}
		}
		else if (IO.KeyShift)
		{
			// Select items from the current selection index until the newly selected item
			Array<Entity*> EntitiesToSelect;
			EID ClickedIndex = Item.Ent.lock()->ID;

			// Determine the iterator direction
			if (ClickedIndex > CurrentSelectionIndex)
			{
				auto It = Items.find(CurrentSelectionIndex);
				// We want to select everything up to _and including_ the clicked item
				auto EndIndex = (++Items.find(Item.Ent.lock()->ID));
				for (; It != EndIndex; ++It)
				{
					EntitiesToSelect.push_back(It->second->Ent.lock().get());
				}
			}
			else if (ClickedIndex < CurrentSelectionIndex)
			{
				auto It = Items.find(CurrentSelectionIndex);
				// We want to select everything up to _and including_ the clicked item
				auto EndIndex = (--Items.find(Item.Ent.lock()->ID));
				for (; It != EndIndex; --It)
				{
					EntitiesToSelect.push_back(It->second->Ent.lock().get());
				}
			}
			else
			{
				EntitiesToSelect.push_back(Item.Ent.lock().get());
			}
			

			if (EntitiesToSelect.size() > 0)
			{
				GEditor->AddToSelection(EntitiesToSelect);
			}
		}
		else
		{
			GEditor->SetSelection(Item.Ent.lock().get());		
		}

		return true;
	}
}