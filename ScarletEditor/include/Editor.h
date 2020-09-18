#pragma once
#include "Core/Core.h"
#include "ECS/ECS.h"
#include "World.h"
#include "Renderer/Viewport.h"
#include "Panels/SceneHierarchy/SceneHierarchy.h"
#include "Panels/PropertyEditor.h"

#include <memory>
#include <glm/glm.hpp>

namespace ScarletEngine
{
	class Editor : public ITickable
	{
	public:
		Editor();

		virtual void Initialize() override;

		virtual void Tick(double DeltaTime) override;

		void SetSelection(const std::shared_ptr<Entity>& InSelectedEntity) 
		{
			SelectedEntity = InSelectedEntity;
			OnSelectionChangedEvent.Broadcast(InSelectedEntity);
		}

		void ClearSelection()
		{
			SelectedEntity.reset();
			OnSelectionCleared.Broadcast();
		}

		Event<const std::shared_ptr<Entity>&> OnSelectionChangedEvent;
		Event<> OnSelectionCleared;

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

			std::unique_ptr<Viewport> View;
			bool bViewportIsFocused;
			bool bViewportIsHovered;
			glm::vec2 ViewportSize;
		};
		std::vector<EditorViewport> Viewports;

		uint32_t CurrentFrameTimeIndex = 0;
		static const uint32_t MaxFrameTimes = 200;
		float FrameTimes[MaxFrameTimes];

		std::shared_ptr<World> EditorWorld;
		
		std::shared_ptr<SceneHierarchyPanel> SceneHierarchy;
		std::shared_ptr<PropertyEditorPanel> PropertyEditor;

		std::weak_ptr<Entity> SelectedEntity;
	};
}