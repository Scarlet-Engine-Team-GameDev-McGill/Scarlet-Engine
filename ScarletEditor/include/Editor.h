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
	using OnSelectionChangedEvent = Event<const SharedPtr<Entity>&>;
	using OnSelectionClearedEvent = Event<>;

	class Editor : public ITickable
	{
	public:
		Editor();

		virtual void Initialize() override;

		virtual void Tick(double DeltaTime) override;

		void SetSelection(const SharedPtr<Entity>& InSelectedEntity)
		{
			SelectedEntity = InSelectedEntity;
			OnSelectionChanged.Broadcast(InSelectedEntity);
		}

		void ClearSelection()
		{
			SelectedEntity.reset();
			OnSelectionCleared.Broadcast();
		}

		const OnSelectionChangedEvent& GetOnSelectionChanged() const { return OnSelectionChanged; }
		const OnSelectionClearedEvent& GetOnSelectionCleared() const { return OnSelectionCleared; }

		static Editor& Get() { static Editor Instance; return Instance; }
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

		WeakPtr<Entity> SelectedEntity;

		OnSelectionChangedEvent OnSelectionChanged;
		OnSelectionClearedEvent OnSelectionCleared;
	};
}