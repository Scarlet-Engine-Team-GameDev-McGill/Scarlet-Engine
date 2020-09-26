#pragma once

#include "Core/Core.h"
#include "ECS/ECS.h"
#include "RAL/RALResources.h"

#include "SceneHierarchyItem.h"

namespace ScarletEngine
{
	class World;
	class SceneHierarchyItem;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(const SharedPtr<World>& InRepresentingWorld);
		void Draw();

		void RepopulateItems();
	private:
		void OnEntityAddedToWorld(const EntityPtr& AddedEntity);
		void OnWorldSelectionChanged(const EntityPtr& LastSelectedEntity);
	private:
		WeakPtr<World> RepresentingWorld;

		UnorderedMap<EID, UniquePtr<SceneHierarchyItem>> Items;
	};
}