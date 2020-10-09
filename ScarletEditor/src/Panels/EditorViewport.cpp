#include "Panels/EditorViewport.h"

#include "Renderer/Renderer.h"

namespace ScarletEngine
{
	uint32_t EditorViewportPanel::NextViewportID = 0;

	EditorViewportPanel::EditorViewportPanel(const SharedPtr<World>& InWorld)
		: UIWindow("TempViewportTitle")
		, View(nullptr)
		, ViewportCam(nullptr)
		, RepresentingWorld(InWorld)
		, PanelSize(1280, 720)
		, bViewportIsFocused(false)
		, bViewportIsHovered(false)
	{
		ZoneScoped
		char Buffer[32];
		snprintf(Buffer, 32, "%s Viewport##%d", ICON_MD_CROP_ORIGINAL, NextViewportID++);
		SetWindowTitle(Buffer);
	}

	void EditorViewportPanel::Construct()
	{
		ZoneScoped
		View = UniquePtr<Viewport>(Renderer::Get().CreateViewport((uint32_t)PanelSize.x, (uint32_t)PanelSize.y));
		ViewportCam = MakeShared<Camera>();

		ViewportCam->SetPosition({ 0, -2, 0 });
		ViewportCam->SetFoV(45.f);
		ViewportCam->SetAspectRatio(PanelSize.x / PanelSize.y);
		View->SetCamera(ViewportCam);
	}

	void EditorViewportPanel::Tick(double)
	{
		ZoneScoped
		glm::ivec2 ViewportFramebufferSize = View->GetSize();

		if ((PanelSize.x >= 1.f && PanelSize.y >= 1.f) &&
			(std::fabs((float)ViewportFramebufferSize.x - PanelSize.x) > 1.0 ||
			std::fabs((float)ViewportFramebufferSize.y - PanelSize.y) > 1.0))
		{
			View->ResizeFramebuffer((uint32_t)PanelSize.x, (uint32_t)PanelSize.y);
			if (PanelSize.x > 0 && PanelSize.y > 0)
			{
				ViewportCam->SetAspectRatio(PanelSize.x / PanelSize.y);
			}
			SCAR_LOG(LogVerbose, "Framebuffer Resized");
		}

		{
			ZoneScopedN("Render world");
			Renderer::Get().DrawScene(RepresentingWorld->GetRenderSceneProxy(), View.get());
		}
	}

	void EditorViewportPanel::PushWindowFlags()
	{
		ZoneScoped
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGuiWindowClass WindowClass;
		WindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
		ImGui::SetNextWindowClass(&WindowClass);
		ImGui::SetNextWindowSize({ 1280, 720 }, ImGuiCond_FirstUseEver);
	}

	void EditorViewportPanel::PopWindowFlags()
	{
		ImGui::PopStyleVar();
	}

	void EditorViewportPanel::DrawWindowContent()
	{
		ZoneScoped
		bViewportIsFocused = ImGui::IsWindowFocused();
		bViewportIsHovered = ImGui::IsWindowHovered();

		PanelSize = ImGui::GetContentRegionAvail();

		uint64_t TextureID = View->GetColorAttachmentID();

		ImGui::Image(reinterpret_cast<void*>(TextureID), ImVec2{ PanelSize.x, PanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}
}