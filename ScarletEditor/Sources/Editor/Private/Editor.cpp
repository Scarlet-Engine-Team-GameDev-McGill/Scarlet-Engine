#include "Editor.h"

#include "UIModule.h"
#include "EditorUILayer.h"

namespace ScarletEngine
{
	Editor* GEditor = nullptr;

	Editor::Editor()
	{
		bStartGameplaySystemsOnLoad = false;
	}

	void Editor::Initialize()
	{
		Engine::Initialize();

		ModuleManager::GetModuleChecked<UIModule>("UIModule")->SetActiveLayer(MakeShared<EditorUILayer>());
	}

	void Editor::SetSelection(const Array<Entity*>& NewSelection)
	{
		SelectedEntities.clear();
		for (Entity* Ent : NewSelection)
		{
			SelectedEntities.insert(Ent);
		}
		OnSelectionChanged.Broadcast();
	}
	
	void Editor::SetSelection(Entity* SelectedItem)
	{
		SelectedEntities.clear();
		SelectedEntities.insert(SelectedItem);
		OnSelectionChanged.Broadcast();
	}

	void Editor::AddToSelection(const Array<Entity*>& EntitiesToAdd)
	{
		for (Entity* Ent : EntitiesToAdd)
		{
			SelectedEntities.insert(Ent);
		}
		OnSelectionChanged.Broadcast();
	}

	void Editor::AddToSelection(Entity* EntityToAdd)
	{
		SelectedEntities.insert(EntityToAdd);
		OnSelectionChanged.Broadcast();
	}

	void Editor::ClearSelection()
	{
		SelectedEntities.clear();
		OnSelectionCleared.Broadcast();
	}

	void Editor::RemoveFromSelection(Entity* EntityToRemove)
	{
		SelectedEntities.erase(EntityToRemove);
		OnSelectionChanged.Broadcast();
	}

	bool Editor::IsEntitySelected(Entity* Ent) const
	{
		return SelectedEntities.find(Ent) != SelectedEntities.end();
	}

	void Editor::StartPlayInEditor()
	{
		SystemScheduler::Get().EnableGameplaySystems();

		bPlayingInEditor = true;
	}

	void Editor::StopPlayInEditor()
	{
		SystemScheduler::Get().DisableGameplaySystems();

		bPlayingInEditor = false;
	}
}
