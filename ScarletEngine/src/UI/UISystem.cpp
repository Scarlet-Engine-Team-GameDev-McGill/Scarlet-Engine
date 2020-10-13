#include "UI/UISystem.h"
#include "UI/UIStyleRegistry.h"
#include "UI/UIStyle.h"
#include "RAL/RAL.h"
#include "Renderer/Renderer.h"

#include <imgui.h>

#ifdef RAL_USE_OPENGL
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

namespace ScarletEngine
{
	void UISystem::Initialize()
	{
		ActiveLayer = nullptr;

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

		IUIStyle* CoreStyle = UIStyleRegistry::Get().GetStyle("Core");
		ImGuiStyle& Style = ImGui::GetStyle();
		Style.Colors[ImGuiCol_Text] = CoreStyle->GetColor("Text").AsVec4();
		Style.Colors[ImGuiCol_TextDisabled] = CoreStyle->GetColor("TextDisabled").AsVec4();
		Style.Colors[ImGuiCol_WindowBg] = CoreStyle->GetColor("WindowBg").AsVec4();
		Style.Colors[ImGuiCol_ChildBg] = CoreStyle->GetColor("ChildBg").AsVec4();
		Style.Colors[ImGuiCol_PopupBg] = CoreStyle->GetColor("PopupBg").AsVec4();
		Style.Colors[ImGuiCol_Border] = CoreStyle->GetColor("Border").AsVec4();
		Style.Colors[ImGuiCol_BorderShadow] = CoreStyle->GetColor("BorderShadow").AsVec4();
		Style.Colors[ImGuiCol_FrameBg] = CoreStyle->GetColor("FrameBg").AsVec4();
		Style.Colors[ImGuiCol_FrameBgHovered] = CoreStyle->GetColor("FrameBgHovered").AsVec4();
		Style.Colors[ImGuiCol_FrameBgActive] = CoreStyle->GetColor("FrameBgActive").AsVec4();
		Style.Colors[ImGuiCol_TitleBg] = CoreStyle->GetColor("TitleBg").AsVec4();
		Style.Colors[ImGuiCol_TitleBgActive] = CoreStyle->GetColor("TitleBgActive").AsVec4();
		Style.Colors[ImGuiCol_TitleBgCollapsed] = CoreStyle->GetColor("TitleBgCollapsed").AsVec4();
		Style.Colors[ImGuiCol_MenuBarBg] = CoreStyle->GetColor("MenuBarBg").AsVec4();
		Style.Colors[ImGuiCol_ScrollbarBg] = CoreStyle->GetColor("ScrollbarBg").AsVec4();
		Style.Colors[ImGuiCol_ScrollbarGrab] = CoreStyle->GetColor("ScrollbarGrab").AsVec4();
		Style.Colors[ImGuiCol_ScrollbarGrabHovered] = CoreStyle->GetColor("ScrollbarGrabHovered").AsVec4();
		Style.Colors[ImGuiCol_ScrollbarGrabActive] = CoreStyle->GetColor("ScrollbarGrabActive").AsVec4();
		Style.Colors[ImGuiCol_CheckMark] = CoreStyle->GetColor("CheckMark").AsVec4();
		Style.Colors[ImGuiCol_SliderGrab] = CoreStyle->GetColor("SliderGrab").AsVec4();
		Style.Colors[ImGuiCol_SliderGrabActive] = CoreStyle->GetColor("SliderGrabActive").AsVec4();
		Style.Colors[ImGuiCol_Button] = CoreStyle->GetColor("Button").AsVec4();
		Style.Colors[ImGuiCol_ButtonHovered] = CoreStyle->GetColor("ButtonHovered").AsVec4();
		Style.Colors[ImGuiCol_ButtonActive] = CoreStyle->GetColor("ButtonActive").AsVec4();
		Style.Colors[ImGuiCol_Header] = CoreStyle->GetColor("Header").AsVec4();
		Style.Colors[ImGuiCol_HeaderHovered] = CoreStyle->GetColor("HeaderHovered").AsVec4();
		Style.Colors[ImGuiCol_HeaderActive] = CoreStyle->GetColor("HeaderActive").AsVec4();
		Style.Colors[ImGuiCol_Separator] = CoreStyle->GetColor("Separator").AsVec4();
		Style.Colors[ImGuiCol_SeparatorActive] = CoreStyle->GetColor("SeparatorActive").AsVec4();
		Style.Colors[ImGuiCol_ResizeGrip] = CoreStyle->GetColor("ResizeGrip").AsVec4();
		Style.Colors[ImGuiCol_ResizeGripHovered] = CoreStyle->GetColor("ResizeGripHovered").AsVec4();
		Style.Colors[ImGuiCol_ResizeGripActive] = CoreStyle->GetColor("ResizeGripActive").AsVec4();
		Style.Colors[ImGuiCol_Tab] = CoreStyle->GetColor("Tab").AsVec4();
		Style.Colors[ImGuiCol_TabHovered] = CoreStyle->GetColor("TabHovered").AsVec4();
		Style.Colors[ImGuiCol_TabActive] = CoreStyle->GetColor("TabActive").AsVec4();
		Style.Colors[ImGuiCol_TabUnfocused] = CoreStyle->GetColor("TabUnfocused").AsVec4();
		Style.Colors[ImGuiCol_TabUnfocusedActive] = CoreStyle->GetColor("TabUnfocusedActive").AsVec4();
		Style.Colors[ImGuiCol_DockingPreview] = CoreStyle->GetColor("DockingPreview").AsVec4();
		Style.Colors[ImGuiCol_DockingEmptyBg] = CoreStyle->GetColor("DockingEmptyBg").AsVec4();
		Style.Colors[ImGuiCol_PlotLines] = CoreStyle->GetColor("PlotLines").AsVec4();
		Style.Colors[ImGuiCol_PlotLinesHovered] = CoreStyle->GetColor("PlotLinesHovered").AsVec4();
		Style.Colors[ImGuiCol_PlotHistogram] = CoreStyle->GetColor("PlotHistogram").AsVec4();
		Style.Colors[ImGuiCol_PlotHistogramHovered] = CoreStyle->GetColor("PlotHistogramHovered").AsVec4();
		Style.Colors[ImGuiCol_TextSelectedBg] = CoreStyle->GetColor("TextSelectedBg").AsVec4();
		Style.Colors[ImGuiCol_DragDropTarget] = CoreStyle->GetColor("DragDropTarget").AsVec4();
		Style.Colors[ImGuiCol_NavHighlight] = CoreStyle->GetColor("NavHighlight").AsVec4();
		Style.Colors[ImGuiCol_NavWindowingHighlight] = CoreStyle->GetColor("NavWindowingHighlight").AsVec4();
		Style.Colors[ImGuiCol_NavWindowingDimBg] = CoreStyle->GetColor("NavWindowingDimBg").AsVec4();
		Style.Colors[ImGuiCol_ModalWindowDimBg] = CoreStyle->GetColor("ModalWindowDimBg").AsVec4();
		Style.GrabRounding = Style.FrameRounding = 2.3f;
		Style.WindowRounding = 0.f;
		Style.WindowMenuButtonPosition = ImGuiDir_Right;
		Style.TabRounding = 0.f;
		Style.FramePadding.x = 8.f;

#ifdef RAL_USE_OPENGL
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)RAL::Get().GetWindowPtr(), true);
		ImGui_ImplOpenGL3_Init("#version 450");
#elif RAL_USE_VULKAN
		// #todo: implement vulkan setup for UI
#endif
	}

	void UISystem::Shutdown()
	{
		if (ActiveLayer != nullptr)
		{
			ActiveLayer->Terminate();
			ActiveLayer.reset();
		}

#ifdef RAL_USE_OPENGL
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
#elif RAL_USE_VULKAN
		// #todo: implement vulkan set for UI
#endif
		ImGui::DestroyContext();
	}

	void UISystem::PreUpdate() const
	{
#ifdef RAL_USE_OPENGL
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
#endif
		ImGui::NewFrame();

		if (ActiveLayer != nullptr)
		{
			ActiveLayer->BeginFrame();
		}
	}

	void UISystem::PostUpdate() const
	{
		if (ActiveLayer != nullptr)
		{
			ActiveLayer->Draw();
			ActiveLayer->DrawWidgets();
		}

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

	void UISystem::SetActiveLayer(const SharedPtr<UILayer>& InLayer)
	{
		ActiveLayer = InLayer;
		ActiveLayer->Initialize();
	}
}