#include "UI/UIWidget.h"
#include "UI/UILayer.h"

namespace ScarletEngine
{
	UIWidget::UIWidget(const String& Title, uint32_t Flags)
		: WindowTitle(Title)
		, WindowFlags(Flags)
		, OwningLayer(nullptr)
		, bOpen(true)
	{

	}

	void UIWidget::Paint()
	{
		PreDraw();
		if (ImGui::Begin(WindowTitle.c_str(), &bOpen, WindowFlags))
		{
			Draw();
		}
		ImGui::End();
		PostDraw();

		if (!bOpen)
		{
			OwningLayer->RemoveWidget(this);
		}
	}

	void UIWidget::SetWindowTitle(const String& NewTitle)
	{
		WindowTitle = NewTitle;
	}
}