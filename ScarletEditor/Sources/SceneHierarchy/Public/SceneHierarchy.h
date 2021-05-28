#pragma once

#include "Core.h"
#include "UIWindow.h"
#include "ECS.h"
#include "SceneHierarchyItem.h"

namespace ScarletEngine
{
	class World;
	class SceneHierarchyItem;

	class SceneHierarchyPanel : public UIWindow
	{
	public:
		SceneHierarchyPanel(const SharedPtr<World>& InRepresentingWorld);
		virtual void Construct() override;
		virtual void DrawWindowContent() override;

		void RepopulateItems();
	private:
		void SynchronizeSelection();

		bool SelectItem(const SceneHierarchyItem& Item);
	private:
		/* Event callbacks */
		void OnEntityAddedToWorld(const EntityPtr& AddedEntity);
		void OnWorldSelectionChanged();
	private:
		WeakPtr<World> RepresentingWorld;

		Map<EID, UniquePtr<SceneHierarchyItem>> Items;
		EID CurrentSelectionIndex;
	};
}