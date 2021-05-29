#include "EditorViewport.h"

#include "RenderModule.h"
#include "Editor.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/gtx/matrix_decompose.hpp"
#include "InputManager.h"
#include "Widgets.h"

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
	{
		char Buffer[32];
		snprintf(Buffer, 32, "%s Viewport##%d", ICON_MD_CROP_ORIGINAL, NextViewportID++);
		SetWindowTitle(Buffer);
	}

	void EditorViewportPanel::Construct()
	{
		RenderModule* Renderer = ModuleManager::GetModuleChecked<RenderModule>("RenderModule");
		View = UniquePtr<Viewport>(Renderer->CreateViewport(static_cast<uint32_t>(PanelSize.x), static_cast<uint32_t>(PanelSize.y)));
		ViewportCam = MakeShared<Camera>();

		ViewportCam->SetPosition({ 0, 5, 5 });
		ViewportCam->SetFoV(45.f);
		ViewportCam->SetAspectRatio(PanelSize.x / PanelSize.y);
		ViewportCam->LookAtPoint({ 0.f, 0.f, 0.f });
		View->SetCamera(ViewportCam);
	}

	void EditorViewportPanel::Tick(double DeltaTime)
	{
		const glm::ivec2 ViewportFramebufferSize = View->GetSize();

		// Resize internal framebuffer if representing viewport window was resized
		if ((PanelSize.x >= 1.f && PanelSize.y >= 1.f) &&
			(std::fabs(static_cast<float>(ViewportFramebufferSize.x) - PanelSize.x) > 1.0 ||
			std::fabs(static_cast<float>(ViewportFramebufferSize.y) - PanelSize.y) > 1.0))
		{
			View->ResizeFramebuffer(static_cast<uint32_t>(PanelSize.x), static_cast<uint32_t>(PanelSize.y));
			if (PanelSize.x > 0 && PanelSize.y > 0)
			{
				ViewportCam->SetAspectRatio(PanelSize.x / PanelSize.y);
			}
			SCAR_LOG(LogVerbose, "Framebuffer Resized");
		}

		// Move the view camera if right mouse button is down
		// #todo_core: make this event-based
		const InputManager& InputManager = InputManager::Get();
		if (InputManager.IsMouseButtonHeld(EMouseCode::MouseButtonRight))
		{
			Camera& ViewCam = View->GetCamera();

			// Update camera rotation
			glm::vec2 DeltaRot = InputManager::Get().GetMouseDelta();
			DeltaRot *= ViewCam.Sensitivity * DeltaTime;
			ViewCam.Rotate(-DeltaRot.x, DeltaRot.y);

			// Update camera position
			const float Velocity = ViewCam.Speed * DeltaTime;
			glm::vec3 Position = ViewCam.GetPosition();
			const glm::vec3 ForwardVec = ViewCam.GetForwardVector();
			const glm::vec3 RightVec = ViewCam.GetRightVector();
			if (InputManager.IsKeyHeld(EKeyCode::KeyW))
			{
				Position += ForwardVec * Velocity;
			}
			if (InputManager.IsKeyHeld(EKeyCode::KeyS))
			{
				Position -= ForwardVec * Velocity;
			}
			if (InputManager.IsKeyHeld(EKeyCode::KeyA))
			{
				Position -= RightVec * Velocity;
			}
			if (InputManager.IsKeyHeld(EKeyCode::KeyD))
			{
				Position += RightVec * Velocity;
			}
			if (InputManager.IsKeyHeld(EKeyCode::KeyE))
			{
				Position += WorldUp * Velocity;
			}
			if (InputManager.IsKeyHeld(EKeyCode::KeyQ))
			{
				Position -= WorldUp * Velocity;
			}
			ViewCam.SetPosition(Position);
		}

		// #todo_rendering: draw calls should be handled by the renderer not by the editor viewports
		{
			ZoneScopedN("Render world");
			RenderModule* Renderer = ModuleManager::GetModuleChecked<RenderModule>("RenderModule");
			Renderer->DrawScene(RepresentingWorld->GetRenderSceneProxy(), View.get());
		}
	}

	void EditorViewportPanel::PushWindowFlags()
	{
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
		ZoneScopedN("Draw Viewport Content")
		bViewportIsFocused = ImGui::IsWindowFocused();
		bViewportIsHovered = ImGui::IsWindowHovered();

		const ImVec2 WindowPos = ImGui::GetWindowPos();
		ImGui::SetNextWindowPos(ImVec2(WindowPos.x + 16.f, WindowPos.y + 16.f));
		ImGui::SetNextWindowBgAlpha(0.8f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
		if (ImGui::Begin("View Options", nullptr, ImGuiWindowFlags_NoMove))
		{
			Widgets::DrawSeparator("Camera");
			
			ImGui::BeginTable("View Options", 2, ImGuiTableFlags_Resizable);

			ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthAlwaysAutoResize);

			ImGui::TableNextColumn();
			ImGui::Text("Sensitivity");
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(100.f);
			ImGui::DragFloat("###CamSens", &ViewportCam.get()->Sensitivity, 0.1f, 0.f, 0.f, "%.2f");

			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::Text("Speed");
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(100.f);
			ImGui::DragFloat("###CamSpeed", &ViewportCam.get()->Speed, 0.1f, 0.f, 0.f, "%.2f");

			ImGui::EndTable();

			Widgets::DrawSeparator();

			Widgets::DrawBooleanInput("Show Grid", bShowGrid);
		}
		ImGui::End();
		ImGui::PopStyleVar(3);

		PanelSize = ImGui::GetContentRegionAvail();

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, PanelSize.x, PanelSize.y);

		Camera& ViewportCamera = View->GetCamera();
		glm::mat4 ViewMatrix = ViewportCamera.GetView();
		glm::mat4 ProjMatrix = ViewportCamera.GetProj();

		if (bShowGrid)
		{
			ImGuizmo::DrawGrid(glm::value_ptr(ViewMatrix), glm::value_ptr(ProjMatrix), glm::value_ptr(glm::mat4(1.f)), 100.f);
		}

		const uint64_t TextureID = View->GetColorAttachmentID();
		ImGui::Image(reinterpret_cast<void*>(TextureID), ImVec2{ PanelSize.x, PanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		Set<Entity*> Selection = GEditor->GetSelection();
		if (Selection.size() == 1)
		{
			Entity* SelectedEntity = *Selection.begin();
			check(SelectedEntity != nullptr);

			TransformComponent* Transform = SelectedEntity->OwningWorld->GetComponent<TransformComponent>(*SelectedEntity);
			check(Transform != nullptr);

			glm::mat4 TransformMatrix = Transform->GetTransformMatrix();
			ImGuizmo::Manipulate(glm::value_ptr(ViewportCamera.GetView()), glm::value_ptr(ViewportCamera.GetProj()), ImGuizmo::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(TransformMatrix));
			
			if (ImGui::IsWindowFocused())
			{
				glm::vec3 Position{};
				glm::quat Rotation{};
				glm::vec3 Scale{};
				glm::vec3 Skew{};
				glm::vec4 Perspective{};
				glm::decompose(TransformMatrix, Scale, Rotation, Position, Skew, Perspective);
				Transform->Position = Position;
				Transform->Rotation = glm::degrees(glm::eulerAngles(Rotation));
				Transform->Scale = Scale;
			}
		}
	}
}
