#include "EditorViewport.h"

#include "RenderModule.h"
#include "Editor.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtx/matrix_decompose.hpp>

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
		, bShowGrid(true)
		, bShowCube(true)
	{
		ZoneScoped
		char Buffer[32];
		snprintf(Buffer, 32, "%s Viewport##%d", ICON_MD_CROP_ORIGINAL, NextViewportID++);
		SetWindowTitle(Buffer);
	}

	void EditorViewportPanel::Construct()
	{
		ZoneScoped

		RenderModule* Renderer = ModuleManager::GetModuleChecked<RenderModule>("RenderModule");
		View = UniquePtr<Viewport>(Renderer->CreateViewport((uint32_t)PanelSize.x, (uint32_t)PanelSize.y));
		ViewportCam = MakeShared<Camera>();

		ViewportCam->SetPosition({ 0, 5, 5 });
		ViewportCam->SetFoV(45.f);
		ViewportCam->SetAspectRatio(PanelSize.x / PanelSize.y);
		ViewportCam->LookAtPoint({ 0.f, 0.f, 0.f });
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

		// @todo: draw calls should be handled by the renderer not by the editor viewports
		{
			ZoneScopedN("Render world");
			RenderModule* Renderer = ModuleManager::GetModuleChecked<RenderModule>("RenderModule");
			Renderer->DrawScene(RepresentingWorld->GetRenderSceneProxy(), View.get());
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

		const ImVec2 WindowPos = ImGui::GetWindowPos();
		ImGui::SetNextWindowPos(ImVec2(WindowPos.x + 16.f, WindowPos.y + 16.f));
		ImGui::SetNextWindowBgAlpha(0.8f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.f);
		if (ImGui::Begin("View Options", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{
			ImGui::Checkbox("Show Grid", &bShowGrid);
			ImGui::Checkbox("Show Cube", &bShowCube);
		}
		ImGui::End();
		ImGui::PopStyleVar(2);

		PanelSize = ImGui::GetContentRegionAvail();

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, PanelSize.x, PanelSize.y);

		uint64_t TextureID = View->GetColorAttachmentID();
		ImGui::Image(reinterpret_cast<void*>(TextureID), ImVec2{ PanelSize.x, PanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		Camera& ViewportCamera = View->GetCamera();
		glm::mat4 ViewMatrix = ViewportCamera.GetView();
		glm::mat4 ProjMatrix = ViewportCamera.GetProj();

		if (bShowGrid)
		{
			ImGuizmo::DrawGrid(glm::value_ptr(ViewMatrix), glm::value_ptr(ProjMatrix), glm::value_ptr(glm::mat4(1.f)), 100.f);
		}

		if (bShowCube)
		{
			const float ViewManipulateRight = ImGui::GetWindowPos().x + PanelSize.x;
			const float ViewManipulateTop = ImGui::GetWindowPos().y;
			ImGuizmo::ViewManipulate(glm::value_ptr(ViewMatrix), 8.f, ImVec2(ViewManipulateRight - 128, ViewManipulateTop), ImVec2(128, 128), 0x10101010);
			ViewportCamera.SetView(ViewMatrix);
		}
		
		Set<EntityHandle*> Selection = GEditor->GetSelection();
		if (Selection.size() == 1)
		{
			EntityHandle* SelectedEntity = *Selection.begin();
			check(SelectedEntity != nullptr);

			Transform* TransformComponent = SelectedEntity->OwningWorld->GetComponent<Transform>(*SelectedEntity);
			check(TransformComponent != nullptr);

			glm::mat4 TransformMatrix = TransformComponent->GetTransformMatrix();
			ImGuizmo::Manipulate(glm::value_ptr(ViewportCamera.GetView()), glm::value_ptr(ViewportCamera.GetProj()), ImGuizmo::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(TransformMatrix));
			
			if (ImGui::IsWindowFocused())
			{
				glm::vec3 Position{};
				glm::quat Rotation{};
				glm::vec3 Scale{};
				glm::vec3 Skew{};
				glm::vec4 Perspective{};
				glm::decompose(TransformMatrix, Scale, Rotation, Position, Skew, Perspective);
				TransformComponent->Position = Position;
				TransformComponent->Rotation = glm::degrees(glm::eulerAngles(Rotation));
				TransformComponent->Scale = Scale;
			}
		}
	}
}