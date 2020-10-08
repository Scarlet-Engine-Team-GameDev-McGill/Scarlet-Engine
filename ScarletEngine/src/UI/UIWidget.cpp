#include "UI/UIWidget.h"

namespace ScarletEngine
{
	UIWidget::UIWidget(const String& Title, uint32_t Flags)
		: WindowTitle(Title)
		, WindowFlags(Flags)
	{

	}

	void UIWidget::Paint()
	{
		PreDraw();
		if (ImGui::Begin(WindowTitle.c_str(), &bShouldClose, WindowFlags))
		{
			Draw();
		}
		ImGui::End();
		PostDraw();
	}

	void UIWidget::SetWindowTitle(const String& NewTitle)
	{
		WindowTitle = NewTitle;
	}
}