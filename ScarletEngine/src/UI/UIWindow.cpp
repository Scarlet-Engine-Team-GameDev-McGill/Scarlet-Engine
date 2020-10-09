#include "UI/UIWindow.h"
#include "UI/UILayer.h"

namespace ScarletEngine
{
	UIWindow::UIWindow(const String& Title, uint32_t Flags)
		: WindowTitle(Title)
		, WindowFlags(Flags)
		, bOpen(true)
	{

	}

	void UIWindow::Paint()
	{
		PushWindowFlags();
		if (ImGui::Begin(WindowTitle.c_str(), &bOpen, WindowFlags))
		{
			DrawWindowContent();
		}
		ImGui::End();
		PopWindowFlags();

		if (!bOpen)
		{
			GetOwningLayer()->RemoveWidget(this);
		}
	}

	void UIWindow::SetWindowTitle(const String& NewTitle)
	{
		WindowTitle = NewTitle;
	}
}