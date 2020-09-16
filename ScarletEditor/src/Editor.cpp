#include "Editor.h"

#include "Renderer/Renderer.h"
#include <imgui.h>
#include <cmath>

namespace ScarletEngine
{
	Editor::Editor()
	{
		MainViewport = std::unique_ptr<Viewport>(Renderer::Get().CreateViewport(1280, 720));
	}

	void Editor::Tick(double DeltaTime)
	{
		FrameTimes[CurrentFrameTimeIndex] = (float)DeltaTime;
		
		glm::ivec2 ViewportFramebufferSize = MainViewport->GetSize();
		if (std::fabs((float)ViewportFramebufferSize.x - ViewportSize.x) < 1.0 ||
			std::fabs((float)ViewportFramebufferSize.y - ViewportSize.y) < 1.0)
		{
			MainViewport->ResizeFramebuffer((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
		}

		
		Renderer::Get().DrawScene(nullptr, MainViewport.get());
		DrawUI();

		CurrentFrameTimeIndex = (CurrentFrameTimeIndex + 1) % 100;
	}

	void Editor::DrawUI()
	{
		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) Engine::Get().SignalQuit();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		bViewportIsFocused = ImGui::IsWindowFocused();
		bViewportIsHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t TextureID = MainViewport->GetColorAttachmentID();

		ImGui::Image(reinterpret_cast<void*>(TextureID), ImVec2{ ViewportSize.x, ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Stats");
		ImGui::Text("Last frame time: %.2f ms", FrameTimes[CurrentFrameTimeIndex]);
		ImGui::PlotLines("Frame Times", FrameTimes, IM_ARRAYSIZE(FrameTimes), CurrentFrameTimeIndex, "", -0.5f, 20.f, ImVec2(0, 80.0f));
		ImGui::End();		

		ImGui::ShowDemoWindow();

		ImGui::End();
	}
}