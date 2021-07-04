#pragma once

#include "Core.h"
#include "World.h"
#include "Viewport.h"
#include "Engine.h"

namespace ScarletEngine
{
	using OnSelectionChangedEvent = Event<>;
	using OnSelectionClearedEvent = Event<>;

	class Editor final : public Engine
	{
	public:
		Editor();
		virtual ~Editor() {}

		virtual void Initialize() override;
		virtual void Terminate() override;

		/* Selection */
		void SetSelection(const Array<Entity*>& NewSelection);
		void SetSelection(Entity* SelectedItem);

		void AddToSelection(const Array<Entity*>& EntitiesToAdd);
		void AddToSelection(Entity* EntityToAdd);

		void RemoveFromSelection(Entity* EntityToRemove);

		void ClearSelection();

		bool IsEntitySelected(Entity* Ent) const;

		const Set<Entity*>& GetSelection() const { return SelectedEntities; }

		const OnSelectionChangedEvent& GetOnSelectionChanged() const { return OnSelectionChanged; }
		const OnSelectionClearedEvent& GetOnSelectionCleared() const { return OnSelectionCleared; }

		/* Play in editor */
		void StartPlayInEditor();

		void StopPlayInEditor();

		bool IsPlayingInEditor() const { return bPlayingInEditor; }

	private:
		void OnWorldChanged(const SharedPtr<World>& NewWorld);
	private:
		Set<Entity*> SelectedEntities;

		OnSelectionChangedEvent OnSelectionChanged;
		OnSelectionClearedEvent OnSelectionCleared;

		bool bPlayingInEditor = false;
	};

	extern Editor* GEditor;
}
