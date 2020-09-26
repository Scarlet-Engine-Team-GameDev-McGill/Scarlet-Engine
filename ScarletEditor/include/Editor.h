#pragma once
#include "Core/Core.h"
#include "ECS/ECS.h"
#include "World.h"
#include "Renderer/Viewport.h"
#include "Panels/SceneHierarchy/SceneHierarchy.h"
#include "Panels/PropertyEditor.h"
#include "AssetManager/AssetHandle.h"
#include "RAL/RALResources.h"

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

		const OnSelectionChangedEvent& GetOnSelectionChanged() const { return OnSelectionChanged; }
		const OnSelectionClearedEvent& GetOnSelectionCleared() const { return OnSelectionCleared; }

	private:
		void DrawUI();
	private:
		struct EditorViewport
		{
			EditorViewport(Viewport* InView)
				: View(InView)
				, bViewportIsFocused(false)
				, bViewportIsHovered(false)
				, ViewportSize({ 0, 0 })
			{}

			UniquePtr<Viewport> View;
			bool bViewportIsFocused;
			bool bViewportIsHovered;
			glm::vec2 ViewportSize;
		};
		Array<EditorViewport> Viewports;

		uint32_t CurrentFrameTimeIndex = 0;
		static const uint32_t MaxFrameTimes = 200;
		float FrameTimes[MaxFrameTimes];

		SharedPtr<World> EditorWorld;
		
		SharedPtr<SceneHierarchyPanel> SceneHierarchy;
		SharedPtr<PropertyEditorPanel> PropertyEditor;

		Set<Entity*> SelectedEntities;

		OnSelectionChangedEvent OnSelectionChanged;
		OnSelectionClearedEvent OnSelectionCleared;
	};

	extern UniquePtr<Editor> GEditor;
}