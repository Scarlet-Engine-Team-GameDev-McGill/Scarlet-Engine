#include "SceneHierarchy.h"

#include "Core.h"
#include "World.h"
#include "Editor.h"
#include "AssetManager.h"
#include "EngineDelegates.h"
#include "Widgets.h"

#include <imgui_internal.h>

namespace ScarletEngine
{
    SceneHierarchyPanel::SceneHierarchyPanel(const SharedPtr<World>& InRepresentingWorld)
        : UIWindow(ICON_MD_ACCOUNT_TREE " Scene Hierarchy", ImGuiWindowFlags_MenuBar)
          , RepresentingWorld(InRepresentingWorld)
    {
        FilterText.reserve(128);
    }

    void SceneHierarchyPanel::Construct()
    {
        RepresentingWorld.lock()->GetOnEntityAddedToWorldEvent().BindMember(this, &SceneHierarchyPanel::OnEntityAddedToWorld);
        GEditor->GetOnSelectionChanged().BindMember(this, &SceneHierarchyPanel::OnWorldSelectionChanged);

        EngineDelegates::OnWorldChanged.BindMember(this, &SceneHierarchyPanel::OnWorldChange);

        Refresh();
    }

    void SceneHierarchyPanel::Destroy()
    {
        RepresentingWorld.lock()->GetOnEntityAddedToWorldEvent().Unbind(this);
        GEditor->GetOnSelectionChanged().Unbind(this);

        EngineDelegates::OnWorldChanged.Unbind(this);
    }

    void SceneHierarchyPanel::DrawWindowContent()
    {
        ZoneScopedN("Draw Scene Hierarchy")

        if (ImGui::BeginMenuBar())
        {
            ImGui::Text(ICON_MD_FILTER_LIST);
            ImGui::SameLine();
            if (ImGui::InputText("###HierarchyTextFilter", FilterText.data(), FilterText.capacity()))
            {
                Refresh();
            }

            ImGui::PushID("SceneHierarchyShowComponents");
            if (Widgets::DrawToggleButton(ICON_MD_SETTINGS, bShowingComponents))
            {
                Refresh();
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Show Components");
            }
            ImGui::PopID();


            ImGui::EndMenuBar();
        }

        const ImGuiTreeNodeFlags BaseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

        if (ImGui::TreeNodeEx(ICON_MD_PUBLIC "  World", BaseFlags | ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (const auto& [ID, EntItem] : Items)
            {
                ImGui::PushID(static_cast<uint32_t>(ID));
                char Buffer[128];
                snprintf(Buffer, 128, "%s   %s", ICON_MD_FILTER_CENTER_FOCUS, EntItem->GetDisplayString().c_str());

                ImGuiTreeNodeFlags Flags = BaseFlags;
                if (EntItem->bIsSelected)
                {
                    Flags |= ImGuiTreeNodeFlags_Selected;
                }

                if (EntItem->Children.size() > 0)
                {
                    const bool bNodeOpen = ImGui::TreeNodeEx(Buffer, Flags);
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

            ImGui::TreePop();
        }
    }

    void SceneHierarchyPanel::RepopulateItems()
    {
        ZoneScoped
        for (const auto& Ent : RepresentingWorld.lock()->GetEntities())
        {
            if (Items.find(Ent->GetEntityID()) == Items.end())
            {
                Items.emplace(Ent->GetEntityID(), ScarNew(SceneHierarchyItem, Ent));
            }
        }
    }

    void SceneHierarchyPanel::Refresh()
    {
        const std::clock_t StartTime = std::clock();

        Items.clear();
        RepopulateItems();

        const float Duration = static_cast<float>(std::clock() - StartTime) / CLOCKS_PER_SEC;
        SCAR_LOG(LogInfo, "Finished SceneHierarchy refresh in %.2f ms", Duration);
    }

    void SceneHierarchyPanel::OnEntityAddedToWorld(const EntityPtr& AddedEntity)
    {
        // If the item is not in the map, emplace it
        if (Items.find(AddedEntity->GetEntityID()) == Items.end())
        {
            Items.emplace(AddedEntity->GetEntityID(), ScarNew(SceneHierarchyItem, AddedEntity));
        }
    }

    void SceneHierarchyPanel::SynchronizeSelection()
    {
        // We need to resynchronize the entire selection when something changes since we may have selected multiple
        for (const auto& [ID, EntItem] : Items)
        {
            EntItem->bIsSelected = GEditor->IsEntitySelected(EntItem->Ent.lock().get());
        }
    }

    void SceneHierarchyPanel::OnWorldSelectionChanged()
    {
        SynchronizeSelection();
    }

    void SceneHierarchyPanel::OnWorldChange(const SharedPtr<World>& InNewWorld)
    {
        RepresentingWorld = InNewWorld;
        Refresh();
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
            const EID ClickedIndex = Item.Ent.lock()->GetEntityID();

            // Determine the iterator direction
            if (ClickedIndex > CurrentSelectionIndex)
            {
                auto It = Items.find(CurrentSelectionIndex);
                // We want to select everything up to _and including_ the clicked item
                const auto EndIndex = (++Items.find(Item.Ent.lock()->GetEntityID()));
                for (; It != EndIndex; ++It)
                {
                    EntitiesToSelect.push_back(It->second->Ent.lock().get());
                }
            }
            else if (ClickedIndex < CurrentSelectionIndex)
            {
                auto It = Items.find(CurrentSelectionIndex);
                // We want to select everything up to _and including_ the clicked item
                const auto EndIndex = (--Items.find(Item.Ent.lock()->GetEntityID()));
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
