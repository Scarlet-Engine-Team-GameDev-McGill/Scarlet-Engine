#pragma once

#include "Core/Core.h"
#include "ECS/ECS.h"

#include "SceneHierarchyItem.h"

namespace ScarletEngine
{
	class World;
	class SceneHierarchyItem;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(const std::shared_ptr<World>& InRepresentingWorld);
		void Draw();

		void RepopulateItems();
	private:
		void OnEntityAddedToWorld(const EntityPtr& AddedEntity);
		void OnWorldSelectionChanged(const EntityPtr& LastSelectedEntity);
	private:
		 std::weak_ptr<World> RepresentingWorld;

		 std::unordered_map<EID, std::unique_ptr<SceneHierarchyItem>> Items;

		 EID SelectedEntityID;
	};
}