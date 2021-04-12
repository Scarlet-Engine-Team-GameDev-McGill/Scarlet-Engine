#pragma once
#include "Core.h"
#include "ECS.h"
#include "World.h"
#include "Viewport.h"
#include "AssetHandle.h"
#include "RALResources.h"
#include "Camera.h"

namespace ScarletEngine
{
	using OnSelectionChangedEvent = Event<>;
	using OnSelectionClearedEvent = Event<>;

	class Editor : public ITickable
	{
	public:
		Editor();
		virtual ~Editor() {}

		void Initialize();
		virtual void Tick(double DeltaTime) override;

	public:
		/* Selection */
		void SetSelection(const Array<EntityHandle*>& NewSelection);
		void SetSelection(EntityHandle* SelectedItem);

		void AddToSelection(const Array<EntityHandle*>& EntitiesToAdd);
		void AddToSelection(EntityHandle* EntityToAdd);

		void RemoveFromSelection(EntityHandle* EntityToRemove);

		void ClearSelection();

		bool IsEntitySelected(EntityHandle* Ent) const;

		const Set<EntityHandle*>& GetSelection() const { return SelectedEntities; }

		SharedPtr<World>& GetActiveWorld() { return EditorWorld; }

		const OnSelectionChangedEvent& GetOnSelectionChanged() const { return OnSelectionChanged; }
		const OnSelectionClearedEvent& GetOnSelectionCleared() const { return OnSelectionCleared; }
	private:
		SharedPtr<World> EditorWorld;

		Set<EntityHandle*> SelectedEntities;

		OnSelectionChangedEvent OnSelectionChanged;
		OnSelectionClearedEvent OnSelectionCleared;
	};

	extern UniquePtr<Editor> GEditor;
}