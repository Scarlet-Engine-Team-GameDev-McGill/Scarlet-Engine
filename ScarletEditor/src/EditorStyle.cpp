#include "EditorStyle.h"

#include "UI/Style.h"

namespace ScarletEngine
{
	void EditorStyle::Initialize()
	{
		Style::SetColor("Text", Color(1.00f, 1.00f, 1.00f, 1.00f));
		Style::SetColor("TextDisabled", Color(0.50f, 0.50f, 0.50f, 1.00f));
		Style::SetColor("WindowBg", Color(0.13f, 0.14f, 0.15f, 1.00f));
		Style::SetColor("ChildBg", Color(0.13f, 0.14f, 0.15f, 1.00f));
		Style::SetColor("PopupBg", Color(0.13f, 0.14f, 0.15f, 1.00f));
		Style::SetColor("Border", Color(0.43f, 0.43f, 0.50f, 0.50f));
		Style::SetColor("BorderShadow", Color(0.00f, 0.00f, 0.00f, 0.00f));
		Style::SetColor("FrameBg", Color(0.25f, 0.25f, 0.25f, 1.00f));
		Style::SetColor("FrameBgHovered", Color(0.38f, 0.38f, 0.38f, 1.00f));
		Style::SetColor("FrameBgActive", Color(0.67f, 0.67f, 0.67f, 0.39f));
		Style::SetColor("TitleBg", Color(0.129f, 0.141f, 0.149f, 1.0f));
		Style::SetColor("TitleBgActive", Color(0.129f, 0.141f, 0.149f, 1.0F));
		Style::SetColor("TitleBgCollapsed", Color(0.00f, 0.00f, 0.00f, 0.51f));
		Style::SetColor("MenuBarBg", Color(0.14f, 0.14f, 0.14f, 1.00f));
		Style::SetColor("ScrollbarBg", Color(0.02f, 0.02f, 0.02f, 0.53f));
		Style::SetColor("ScrollbarGrab", Color(0.31f, 0.31f, 0.31f, 1.00f));
		Style::SetColor("ScrollbarGrabHovered", Color(0.41f, 0.41f, 0.41f, 1.00f));
		Style::SetColor("ScrollbarGrabActive", Color(0.51f, 0.51f, 0.51f, 1.00f));
		Style::SetColor("CheckMark", Color(0.11f, 0.64f, 0.92f, 1.00f));
		Style::SetColor("SliderGrab", Color(0.11f, 0.64f, 0.92f, 1.00f));
		Style::SetColor("SliderGrabActive", Color(0.08f, 0.50f, 0.72f, 1.00f));
		Style::SetColor("Button", Color(0.25f, 0.25f, 0.25f, 1.00f));
		Style::SetColor("ButtonHovered", Color(0.38f, 0.38f, 0.38f, 1.00f));
		Style::SetColor("ButtonActive", Color(0.67f, 0.67f, 0.67f, 0.39f));
		Style::SetColor("Header", Color(0.22f, 0.22f, 0.22f, 1.00f));
		Style::SetColor("HeaderHovered", Color(0.25f, 0.25f, 0.25f, 1.00f));
		Style::SetColor("HeaderActive", Color(0.67f, 0.67f, 0.67f, 0.39f));
		Style::SetColor("Separator", Style::GetColor("Border"));
		Style::SetColor("SeparatorHovered", Color(0.41f, 0.42f, 0.44f, 1.00f));
		Style::SetColor("SeparatorActive", Color(0.26f, 0.59f, 0.98f, 0.95f));
		Style::SetColor("ResizeGrip", Color(0.00f, 0.00f, 0.00f, 0.00f));
		Style::SetColor("ResizeGripHovered", Color(0.29f, 0.30f, 0.31f, 0.67f));
		Style::SetColor("ResizeGripActive", Color(0.26f, 0.59f, 0.98f, 0.95f));
		Style::SetColor("Tab", Color(0.129f, 0.141f, 0.149f, 1.0f));
		Style::SetColor("TabHovered", Color(0.33f, 0.34f, 0.36f, 0.83f));
		Style::SetColor("TabActive", Color(0.23f, 0.23f, 0.24f, 1.00f));
		Style::SetColor("TabUnfocused", Color(0.08f, 0.08f, 0.09f, 1.00f));
		Style::SetColor("TabUnfocusedActive", Color(0.13f, 0.14f, 0.15f, 1.00f));
		Style::SetColor("DockingPreview", Color(0.26f, 0.59f, 0.98f, 0.70f));
		Style::SetColor("DockingEmptyBg", Color(0.20f, 0.20f, 0.20f, 1.00f));
		Style::SetColor("PlotLines", Color(0.61f, 0.61f, 0.61f, 1.00f));
		Style::SetColor("PlotLinesHovered", Color(1.00f, 0.43f, 0.35f, 1.00f));
		Style::SetColor("PlotHistogram", Color(0.90f, 0.70f, 0.00f, 1.00f));
		Style::SetColor("PlotHistogramHovered", Color(1.00f, 0.60f, 0.00f, 1.00f));
		Style::SetColor("TextSelectedBg", Color(0.26f, 0.59f, 0.98f, 0.35f));
		Style::SetColor("DragDropTarget", Color(0.11f, 0.64f, 0.92f, 1.00f));
		Style::SetColor("NavHighlight", Color(0.26f, 0.59f, 0.98f, 1.00f));
		Style::SetColor("NavWindowingHighlight", Color(1.00f, 1.00f, 1.00f, 0.70f));
		Style::SetColor("NavWindowingDimBg", Color(0.80f, 0.80f, 0.80f, 0.20f));
		Style::SetColor("ModalWindowDimBg", Color(0.80f, 0.80f, 0.80f, 0.35f));
	}
}