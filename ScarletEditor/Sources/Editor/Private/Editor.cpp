#include "Editor.h"

#include "UIModule.h"
#include "EditorUILayer.h"

namespace ScarletEngine
{
	UniquePtr<Editor> GEditor = nullptr;

	Editor::Editor()
		: EditorWorld(nullptr)
		, SelectedEntities()
	{
	}

	void Editor::Initialize()
	{
		EditorWorld = MakeShared<World>();
		// #todo_core: this should be probably be handled by the engine
		EditorWorld->Initialize();

		ModuleManager::GetModuleChecked<UIModule>("UIModule")->SetActiveLayer(MakeShared<EditorUILayer>());
	}

	void Editor::Tick(double)
	{
		ZoneScopedN("Editor Tick")
	}

	void Editor::SetSelection(const Array<EntityHandle*>& NewSelection)
	{
		SelectedEntities.clear();
		for (EntityHandle* Ent : NewSelection)
		{
			SelectedEntities.insert(Ent);
		}
		OnSelectionChanged.Broadcast();
	}
	
	void Editor::SetSelection(EntityHandle* SelectedItem)
	{
		SelectedEntities.clear();
		SelectedEntities.insert(SelectedItem);
		OnSelectionChanged.Broadcast();
	}

	void Editor::AddToSelection(const Array<EntityHandle*>& EntitiesToAdd)
	{
		for (EntityHandle* Ent : EntitiesToAdd)
		{
			SelectedEntities.insert(Ent);
		}
		OnSelectionChanged.Broadcast();
	}

	void Editor::AddToSelection(EntityHandle* EntityToAdd)
	{
		SelectedEntities.insert(EntityToAdd);
		OnSelectionChanged.Broadcast();
	}

	void Editor::ClearSelection()
	{
		SelectedEntities.clear();
		OnSelectionCleared.Broadcast();
	}

	void Editor::RemoveFromSelection(EntityHandle* EntityToRemove)
	{
		SelectedEntities.erase(EntityToRemove);
		OnSelectionChanged.Broadcast();
	}

	bool Editor::IsEntitySelected(EntityHandle* Ent) const
	{
		return SelectedEntities.find(Ent) != SelectedEntities.end();
	}
}