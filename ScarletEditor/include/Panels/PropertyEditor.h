#pragma once

#include "Core/Core.h"
#include "UI/UIWindow.h"

namespace ScarletEngine
{
	struct Entity;

	class PropertyEditorPanel : public UIWindow
	{
	public:
		PropertyEditorPanel();

		virtual void Construct() override;
		virtual void DrawWindowContent() override;

		void OnSelectionChanged();
		void OnSelectionCleared();

	private:
		void DrawTransformEditor();
	private:
		Entity* FocusedEntity;
	};
}