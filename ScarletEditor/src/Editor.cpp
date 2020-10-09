#include "Editor.h"

#include "UI/UISystem.h"
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
		// #todo: this should be handled by the engine
		EditorWorld->Initialize();

		UISystem::Get().SetActiveLayer(MakeShared<EditorUILayer>());
	}

	void Editor::Tick(double)
	{
		ZoneScoped
	}

	void Editor::SetSelection(const Array<Entity*>& NewSelection)
	{
		ZoneScoped
		SelectedEntities.clear();
		for (Entity* Ent : NewSelection)
		{
			SelectedEntities.insert(Ent);
		}
		OnSelectionChanged.Broadcast();
	}
	
	void Editor::SetSelection(Entity* SelectedItem)
	{
		ZoneScoped
		SelectedEntities.clear();
		SelectedEntities.insert(SelectedItem);
		OnSelectionChanged.Broadcast();
	}

	void Editor::AddToSelection(const Array<Entity*>& EntitiesToAdd)
	{
		ZoneScoped
		for (Entity* Ent : EntitiesToAdd)
		{
			SelectedEntities.insert(Ent);
		}
		OnSelectionChanged.Broadcast();
	}

	void Editor::AddToSelection(Entity* EntityToAdd)
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

	void Editor::RemoveFromSelection(Entity* EntityToRemove)
	{
		ZoneScoped
		SelectedEntities.erase(EntityToRemove);
		OnSelectionChanged.Broadcast();
	}

	bool Editor::IsEntitySelected(Entity* Ent) const
	{
		ZoneScoped
		return SelectedEntities.find(Ent) != SelectedEntities.end();
	}
}