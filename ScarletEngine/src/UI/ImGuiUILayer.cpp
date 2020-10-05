#include "UI/ImGuiUILayer.h"

#include "Core/Core.h"
#include "UI/Style.h"
#include "RAL/RAL.h"
#include "Renderer/Renderer.h"

#ifdef RAL_USE_OPENGL
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#elif RAL_USE_VULKAN
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#endif

namespace ScarletEngine
{
	void ImGuiUILayer::Initialize()
	{
		ZoneScoped

		IMGUI_CHECKVERSION();
		ImGui::SetAllocatorFunctions(
			+[](size_t Size, void*)
			{
				return ScarMalloc(Size);
			},
			+[](void* Ptr, void*)
			{
				ScarFree(Ptr);
			});

		ImGui::CreateContext();
		ImGuiIO& IO = ImGui::GetIO();
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		IO.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
		IO.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
		// Enable floating windows
		//IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImFontConfig Config;
		Config.OversampleH = 4;
		Config.OversampleV = 4;
		Config.GlyphOffset.y -= 1.0f;
		IO.Fonts->AddFontFromFileTTF("../ScarletEngine/content/OpenSans-Regular.ttf", 18.0f, &Config);

		Config.MergeMode = true;
		Config.GlyphOffset.y -= -4.f;
		const ImWchar MDIconRanges[] = { ICON_MIN_MD, ICON_MAX_MD, 0 };
		IO.Fonts->AddFontFromFileTTF("../ScarletEngine/content/MaterialIcons-Regular.ttf", 18.0f, &Config, MDIconRanges);
		IO.Fonts->Build();

		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Text] = Style::GetColor("Text").AsVec4();
		style.Colors[ImGuiCol_TextDisabled] = Style::GetColor("TextDisabled").AsVec4();
		style.Colors[ImGuiCol_WindowBg] = Style::GetColor("WindowBg").AsVec4();
		style.Colors[ImGuiCol_ChildBg] = Style::GetColor("ChildBg").AsVec4();
		style.Colors[ImGuiCol_PopupBg] = Style::GetColor("PopupBg").AsVec4();
		style.Colors[ImGuiCol_Border] = Style::GetColor("Border").AsVec4();
		style.Colors[ImGuiCol_BorderShadow] = Style::GetColor("BorderShadow").AsVec4();
		style.Colors[ImGuiCol_FrameBg] = Style::GetColor("FrameBg").AsVec4();
		style.Colors[ImGuiCol_FrameBgHovered] = Style::GetColor("FrameBgHovered").AsVec4();
		style.Colors[ImGuiCol_FrameBgActive] = Style::GetColor("FrameBgActive").AsVec4();
		style.Colors[ImGuiCol_TitleBg] = Style::GetColor("TitleBg").AsVec4();
		style.Colors[ImGuiCol_TitleBgActive] = Style::GetColor("TitleBgActive").AsVec4();
		style.Colors[ImGuiCol_TitleBgCollapsed] = Style::GetColor("TitleBgCollapsed").AsVec4();
		style.Colors[ImGuiCol_MenuBarBg] = Style::GetColor("MenuBarBg").AsVec4();
		style.Colors[ImGuiCol_ScrollbarBg] = Style::GetColor("ScrollbarBg").AsVec4();
		style.Colors[ImGuiCol_ScrollbarGrab] = Style::GetColor("ScrollbarGrab").AsVec4();
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = Style::GetColor("ScrollbarGrabHovered").AsVec4();
		style.Colors[ImGuiCol_ScrollbarGrabActive] = Style::GetColor("ScrollbarGrabActive").AsVec4();
		style.Colors[ImGuiCol_CheckMark] = Style::GetColor("CheckMark").AsVec4();
		style.Colors[ImGuiCol_SliderGrab] = Style::GetColor("SliderGrab").AsVec4();
		style.Colors[ImGuiCol_SliderGrabActive] = Style::GetColor("SliderGrabActive").AsVec4();
		style.Colors[ImGuiCol_Button] = Style::GetColor("Button").AsVec4();
		style.Colors[ImGuiCol_ButtonHovered] = Style::GetColor("ButtonHovered").AsVec4();
		style.Colors[ImGuiCol_ButtonActive] = Style::GetColor("ButtonActive").AsVec4();
		style.Colors[ImGuiCol_Header] = Style::GetColor("Header").AsVec4();
		style.Colors[ImGuiCol_HeaderHovered] = Style::GetColor("HeaderHovered").AsVec4();
		style.Colors[ImGuiCol_HeaderActive] = Style::GetColor("HeaderActive").AsVec4();
		style.Colors[ImGuiCol_Separator] = Style::GetColor("Separator").AsVec4();
		style.Colors[ImGuiCol_SeparatorActive] = Style::GetColor("SeparatorActive").AsVec4();
		style.Colors[ImGuiCol_ResizeGrip] = Style::GetColor("ResizeGrip").AsVec4();
		style.Colors[ImGuiCol_ResizeGripHovered] = Style::GetColor("ResizeGripHovered").AsVec4();
		style.Colors[ImGuiCol_ResizeGripActive] = Style::GetColor("ResizeGripActive").AsVec4();
		style.Colors[ImGuiCol_Tab] = Style::GetColor("Tab").AsVec4();
		style.Colors[ImGuiCol_TabHovered] = Style::GetColor("TabHovered").AsVec4();
		style.Colors[ImGuiCol_TabActive] = Style::GetColor("TabActive").AsVec4();
		style.Colors[ImGuiCol_TabUnfocused] = Style::GetColor("TabUnfocused").AsVec4();
		style.Colors[ImGuiCol_TabUnfocusedActive] = Style::GetColor("TabUnfocusedActive").AsVec4();
		style.Colors[ImGuiCol_DockingPreview] = Style::GetColor("DockingPreview").AsVec4();
		style.Colors[ImGuiCol_DockingEmptyBg] = Style::GetColor("DockingEmptyBg").AsVec4();
		style.Colors[ImGuiCol_PlotLines] = Style::GetColor("PlotLines").AsVec4();
		style.Colors[ImGuiCol_PlotLinesHovered] = Style::GetColor("PlotLinesHovered").AsVec4();
		style.Colors[ImGuiCol_PlotHistogram] = Style::GetColor("PlotHistogram").AsVec4();
		style.Colors[ImGuiCol_PlotHistogramHovered] = Style::GetColor("PlotHistogramHovered").AsVec4();
		style.Colors[ImGuiCol_TextSelectedBg] = Style::GetColor("TextSelectedBg").AsVec4();
		style.Colors[ImGuiCol_DragDropTarget] = Style::GetColor("DragDropTarget").AsVec4();
		style.Colors[ImGuiCol_NavHighlight] = Style::GetColor("NavHighlight").AsVec4();
		style.Colors[ImGuiCol_NavWindowingHighlight] = Style::GetColor("NavWindowingHighlight").AsVec4();
		style.Colors[ImGuiCol_NavWindowingDimBg] = Style::GetColor("NavWindowingDimBg").AsVec4();
		style.Colors[ImGuiCol_ModalWindowDimBg] = Style::GetColor("ModalWindowDimBg").AsVec4();
		style.GrabRounding = style.FrameRounding = 2.3f;
		style.WindowRounding = 0.f;
		style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.TabRounding = 0.f;
		style.FramePadding.x = 8.f;

#ifdef RAL_USE_OPENGL
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)RAL::Get().GetWindowPtr(), true);
		ImGui_ImplOpenGL3_Init("#version 450");
#elif RAL_USE_VULKAN
		// #todo: implement vulkan set for UI
#endif
	}

	void ImGuiUILayer::BeginFrame()
	{
		ZoneScoped
#ifdef RAL_USE_OPENGL
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
#endif
	}

	void ImGuiUILayer::Draw()
	{
		ZoneScoped
		ImGui::Render();
#ifdef RAL_USE_OPENGL
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ZoneScopedN("Render ImGUI::Viewports")
			void* BackupCurrentContext = Renderer::Get().GetWindowPtr();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			Renderer::Get().SetWindowCtx(BackupCurrentContext);
		}
	}

	void ImGuiUILayer::Terminate()
	{
#ifdef RAL_USE_OPENGL
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
#elif RAL_USE_VULKAN
		// #todo: implement vulkan set for UI
#endif
		ImGui::DestroyContext();

		UILayer::Terminate();
	}
}