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
		void OnWorldSelectionChanged();

		void SynchronizeSelection();

		bool SelectItem(const SceneHierarchyItem& Item);
	private:
		WeakPtr<World> RepresentingWorld;

		Map<EID, UniquePtr<SceneHierarchyItem>> Items;
		EID CurrentSelectionIndex;
	};
}