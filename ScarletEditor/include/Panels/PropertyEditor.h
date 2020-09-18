#pragma once

#include "Core/Core.h"
#include "ECS/ECS.h"

namespace ScarletEngine
{
	class PropertyEditorPanel
	{
	public:
		PropertyEditorPanel();

		void OnSelectionChanged(const std::shared_ptr<Entity>& Ent);
		void OnSelectionCleared();

		void Draw();
	private:
		void DrawTransformEditor();
	private:
		std::weak_ptr<Entity> FocusedEntity;
		bool bHasSelection;

	};
}