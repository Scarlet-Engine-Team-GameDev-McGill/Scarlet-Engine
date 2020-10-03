#pragma once

#include "Core/Core.h"
#include "ECS/ECS.h"

namespace ScarletEngine
{
	class PropertyEditorPanel
	{
	public:
		PropertyEditorPanel();

		void OnSelectionChanged();
		void OnSelectionCleared();

		void Draw();
	private:
		void DrawTransformEditor();
	private:
		Entity* FocusedEntity;
	};
}