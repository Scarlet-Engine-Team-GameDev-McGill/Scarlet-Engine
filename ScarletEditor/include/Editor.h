#pragma once
#include "Core/Core.h"
#include "ECS/ECS.h"
#include "World.h"
#include "Renderer/Viewport.h"
#include "AssetManager/AssetHandle.h"
#include "RAL/RALResources.h"
#include "Core/Camera.h"

namespace ScarletEngine
{
	using OnSelectionChangedEvent = Event<>;
	using OnSelectionClearedEvent = Event<>;

	class Editor : public ITickable
	{
	public:
		Editor();
		virtual ~Editor() {}

		virtual void Initialize() override;
		virtual void Tick(double DeltaTime) override;

	public:
		/* Selection */
		void SetSelection(const Array<Entity*>& NewSelection);
		void SetSelection(Entity* SelectedItem);

		void AddToSelection(const Array<Entity*>& EntitiesToAdd);
		void AddToSelection(Entity* EntityToAdd);

		void RemoveFromSelection(Entity* EntityToRemove);

		void ClearSelection();

		bool IsEntitySelected(Entity* Ent) const;

		const Set<Entity*>& GetSelection() const { return SelectedEntities; }

		SharedPtr<World>& GetActiveWorld() { return EditorWorld; }

		const OnSelectionChangedEvent& GetOnSelectionChanged() const { return OnSelectionChanged; }
		const OnSelectionClearedEvent& GetOnSelectionCleared() const { return OnSelectionCleared; }
	private:
		SharedPtr<World> EditorWorld;

		Set<Entity*> SelectedEntities;

		OnSelectionChangedEvent OnSelectionChanged;
		OnSelectionClearedEvent OnSelectionCleared;
	};

	extern UniquePtr<Editor> GEditor;
}