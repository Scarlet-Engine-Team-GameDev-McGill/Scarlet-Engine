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
		, EntityIcon(nullptr)
		, EntityTexture(nullptr)
		, SelectedEntityID(INVALID_EID)
	{
		RepresentingWorld.lock()->GetOnEntityAddedToWorldEvent().Bind(this, &SceneHierarchyPanel::OnEntityAddedToWorld);
		Editor::Get().GetOnSelectionChanged().Bind(this, &SceneHierarchyPanel::OnWorldSelectionChanged);
		RepopulateItems();

		EntityIcon = AssetManager::LoadTextureFile("../ScarletEngine/content/EntityIcon.png");
		EntityTexture = std::shared_ptr<RALTexture2D>(RAL::Get().CreateTexture2D(EntityIcon));
	}

	void SceneHierarchyPanel::Draw()
	{
		ImGui::Begin("Scene Hierarchy");

		if (ImGui::TreeNodeEx("World", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (const auto& [ID, EntItem] : Items)
			{
				ImGui::PushID((uint32_t)ID);
				ImGui::Image(reinterpret_cast<void*>(EntityTexture->GetTextureResource()), ImVec2(28, 28));
				ImGui::SameLine();
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