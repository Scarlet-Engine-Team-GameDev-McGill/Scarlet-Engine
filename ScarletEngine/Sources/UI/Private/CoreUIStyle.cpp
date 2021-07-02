#include "CoreUIStyle.h"

namespace ScarletEngine
{
	void CoreUIStyle::Initialize()
	{
		SetColor("Text", Color(1.00f, 1.00f, 1.00f, 1.00f));
		SetColor("TextDisabled", Color(0.50f, 0.50f, 0.50f, 1.00f));
		SetColor("WindowBg", Color(0.13f, 0.14f, 0.15f, 1.00f));
		SetColor("ChildBg", Color(0.13f, 0.14f, 0.15f, 1.00f));
		SetColor("PopupBg", Color(0.13f, 0.14f, 0.15f, 1.00f));
		SetColor("Border", Color(0.43f, 0.43f, 0.50f, 0.50f));
		SetColor("BorderShadow", Color(0.00f, 0.00f, 0.00f, 0.00f));
		SetColor("FrameBg", Color(0.25f, 0.25f, 0.25f, 1.00f));
		SetColor("FrameBgHovered", Color(0.38f, 0.38f, 0.38f, 1.00f));
		SetColor("FrameBgActive", Color(0.67f, 0.67f, 0.67f, 0.39f));
		SetColor("TitleBg", Color(0.129f, 0.141f, 0.149f, 1.0f));
		SetColor("TitleBgActive", Color(0.129f, 0.141f, 0.149f, 1.0F));
		SetColor("TitleBgCollapsed", Color(0.00f, 0.00f, 0.00f, 0.51f));
		SetColor("MenuBarBg", Color(0.14f, 0.14f, 0.14f, 1.00f));
		SetColor("ScrollbarBg", Color(0.02f, 0.02f, 0.02f, 0.53f));
		SetColor("ScrollbarGrab", Color(0.31f, 0.31f, 0.31f, 1.00f));
		SetColor("ScrollbarGrabHovered", Color(0.41f, 0.41f, 0.41f, 1.00f));
		SetColor("ScrollbarGrabActive", Color(0.51f, 0.51f, 0.51f, 1.00f));
		SetColor("CheckMark", Color(0.11f, 0.64f, 0.92f, 1.00f));
		SetColor("SliderGrab", Color(0.11f, 0.64f, 0.92f, 1.00f));
		SetColor("SliderGrabActive", Color(0.08f, 0.50f, 0.72f, 1.00f));
		SetColor("Button", Color(0.25f, 0.25f, 0.25f, 1.00f));
		SetColor("ButtonHovered", Color(0.38f, 0.38f, 0.38f, 1.00f));
		SetColor("ButtonActive", Color(0.67f, 0.67f, 0.67f, 0.39f));
		SetColor("Header", Color(0.22f, 0.22f, 0.22f, 1.00f));
		SetColor("HeaderHovered", Color(0.25f, 0.25f, 0.25f, 1.00f));
		SetColor("HeaderActive", Color(0.67f, 0.67f, 0.67f, 0.39f));
		SetColor("Separator", GetColor("Border"));
		SetColor("SeparatorHovered", Color(0.41f, 0.42f, 0.44f, 1.00f));
		SetColor("SeparatorActive", Color(0.26f, 0.59f, 0.98f, 0.95f));
		SetColor("ResizeGrip", Color(0.00f, 0.00f, 0.00f, 0.00f));
		SetColor("ResizeGripHovered", Color(0.29f, 0.30f, 0.31f, 0.67f));
		SetColor("ResizeGripActive", Color(0.26f, 0.59f, 0.98f, 0.95f));
		SetColor("Tab", Color(0.129f, 0.141f, 0.149f, 1.0f));
		SetColor("TabHovered", Color(0.33f, 0.34f, 0.36f, 0.83f));
		SetColor("TabActive", Color(0.23f, 0.23f, 0.24f, 1.00f));
		SetColor("TabUnfocused", Color(0.08f, 0.08f, 0.09f, 1.00f));
		SetColor("TabUnfocusedActive", Color(0.23f, 0.23f, 0.24f, 1.00f));
		SetColor("DockingPreview", Color(0.26f, 0.59f, 0.98f, 0.70f));
		SetColor("DockingEmptyBg", Color(0.20f, 0.20f, 0.20f, 1.00f));
		SetColor("PlotLines", Color(0.61f, 0.61f, 0.61f, 1.00f));
		SetColor("PlotLinesHovered", Color(1.00f, 0.43f, 0.35f, 1.00f));
		SetColor("PlotHistogram", Color(0.90f, 0.70f, 0.00f, 1.00f));
		SetColor("PlotHistogramHovered", Color(1.00f, 0.60f, 0.00f, 1.00f));
		SetColor("TextSelectedBg", Color(0.26f, 0.59f, 0.98f, 0.35f));
		SetColor("DragDropTarget", Color(0.11f, 0.64f, 0.92f, 1.00f));
		SetColor("NavHighlight", Color(0.26f, 0.59f, 0.98f, 1.00f));
		SetColor("NavWindowingHighlight", Color(1.00f, 1.00f, 1.00f, 0.70f));
		SetColor("NavWindowingDimBg", Color(0.80f, 0.80f, 0.80f, 0.20f));
		SetColor("ModalWindowDimBg", Color(0.80f, 0.80f, 0.80f, 0.35f));
	}
}