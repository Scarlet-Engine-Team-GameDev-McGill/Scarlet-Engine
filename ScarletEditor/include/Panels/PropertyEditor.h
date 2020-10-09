#pragma once

#include "Core/Core.h"
#include "UI/UIWidget.h"
#include "ECS/ECS.h"

namespace ScarletEngine
{
	class PropertyEditorPanel : public UIWidget
	{
	public:
		PropertyEditorPanel();

		virtual void Construct() override;
		virtual void Draw() override;

		void OnSelectionChanged();
		void OnSelectionCleared();

	private:
		void DrawTransformEditor();
	private:
		Entity* FocusedEntity;
	};
}