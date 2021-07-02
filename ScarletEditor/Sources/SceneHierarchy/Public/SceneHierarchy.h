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
        virtual void Destroy() override;
        virtual void DrawWindowContent() override;

        void RepopulateItems();
    private:
        void Refresh();
        void SynchronizeSelection();

        bool SelectItem(const SceneHierarchyItem& Item);
    private:
        /* Event callbacks */
        void OnEntityAddedToWorld(const EntityPtr& AddedEntity);
        void OnWorldSelectionChanged();
        void OnWorldChange(const SharedPtr<World>& InNewWorld);
    private:
        WeakPtr<World> RepresentingWorld;

        Map<EID, UniquePtr<SceneHierarchyItem>> Items;
        EID CurrentSelectionIndex = INVALID_EID;
        bool bShowingComponents = false;
    };
}
