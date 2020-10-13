#pragma once

#include "Core.h"
#include "UIWindow.h"
#include "ECS.h"
#include "RALResources.h"

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