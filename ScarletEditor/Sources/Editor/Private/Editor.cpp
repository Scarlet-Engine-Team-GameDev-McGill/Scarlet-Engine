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
		ZoneScoped
		EditorWorld = MakeShared<World>();
		// #todo_core: this should be handled by the engine
		EditorWorld->Initialize();

		ModuleManager::GetModuleChecked<UIModule>("UIModule")->SetActiveLayer(MakeShared<EditorUILayer>());
	}

	void Editor::Tick(double)
	{
		ZoneScoped
	}

	void Editor::SetSelection(const Array<EntityHandle*>& NewSelection)
	{
		ZoneScoped
		SelectedEntities.clear();
		for (EntityHandle* Ent : NewSelection)
		{
			SelectedEntities.insert(Ent);
		}
		OnSelectionChanged.Broadcast();
	}
	
	void Editor::SetSelection(EntityHandle* SelectedItem)
	{
		ZoneScoped
		SelectedEntities.clear();
		SelectedEntities.insert(SelectedItem);
		OnSelectionChanged.Broadcast();
	}

	void Editor::AddToSelection(const Array<EntityHandle*>& EntitiesToAdd)
	{
		ZoneScoped
		for (EntityHandle* Ent : EntitiesToAdd)
		{
			SelectedEntities.insert(Ent);
		}
		OnSelectionChanged.Broadcast();
	}

	void Editor::AddToSelection(EntityHandle* EntityToAdd)
	{
		ZoneScoped
		SelectedEntities.insert(EntityToAdd);
		OnSelectionChanged.Broadcast();
	}

	void Editor::ClearSelection()
	{
		ZoneScoped
		SelectedEntities.clear();
		OnSelectionCleared.Broadcast();
	}

	void Editor::RemoveFromSelection(EntityHandle* EntityToRemove)
	{
		ZoneScoped
		SelectedEntities.erase(EntityToRemove);
		OnSelectionChanged.Broadcast();
	}

	bool Editor::IsEntitySelected(EntityHandle* Ent) const
	{
		ZoneScoped
		return SelectedEntities.find(Ent) != SelectedEntities.end();
	}
}