#pragma once

#include "Core/Core.h"
#include "ECS/ECS.h"

namespace ScarletEngine
{
	class PropertyEditorPanel
	{
	public:
		PropertyEditorPanel();

		void OnSelectionChanged(const SharedPtr<Entity>& Ent);
		void OnSelectionCleared();

		void Draw();
	private:
		void DrawTransformEditor();
	private:
		WeakPtr<Entity> FocusedEntity;
		bool bHasSelection;

	};
}