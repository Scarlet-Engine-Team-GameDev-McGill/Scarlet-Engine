#include "Editor.h"

#include "Renderer/Renderer.h"
#include <imgui.h>
#include <cmath>

#include "AssetManager/AssetManager.h"

namespace ScarletEngine
{
	UniquePtr<Editor> GEditor = nullptr;

	Editor::Editor()
		: FrameTimes()
		, EditorWorld(nullptr)
		, SceneHierarchy(nullptr)
		, PropertyEditor(nullptr)
		, SelectedEntities()
	{
	}

	void Editor::Initialize()
	{
		EditorWorld = MakeShared<World>();
		SceneHierarchy = MakeShared<SceneHierarchyPanel>(EditorWorld);
		PropertyEditor = MakeShared<PropertyEditorPanel>();

		// Test entities
		EditorWorld->CreateEntity<Transform>("Entity 1");
		EditorWorld->CreateEntity<Transform>("Entity 2");
		EditorWorld->CreateEntity<Transform>("Entity 3");
		EditorWorld->CreateEntity<Transform>("Entity 4");
		EditorWorld->CreateEntity<Transform>("Entity 5");
		EditorWorld->CreateEntity<Transform>("Entity 6");
		EditorWorld->CreateEntity<Transform>("Entity 7");

		Viewports.emplace_back(Renderer::Get().CreateViewport(1280, 720));
	}

	void Editor::Tick(double DeltaTime)
	{
		FrameTimes[CurrentFrameTimeIndex] = (float)DeltaTime;

		for (const auto& EdViewport : Viewports)
		{
			glm::ivec2 ViewportFramebufferSize = EdViewport.View->GetSize();
			if (std::fabs((float)ViewportFramebufferSize.x - EdViewport.ViewportSize.x) > 1.0 ||
				std::fabs((float)ViewportFramebufferSize.y - EdViewport.ViewportSize.y) > 1.0)
			{
				EdViewport.View->ResizeFramebuffer((uint32_t)EdViewport.ViewportSize.x, (uint32_t)EdViewport.ViewportSize.y);
				SCAR_LOG(LogVerbose, "Framebuffer Resized");
			}

			Renderer::Get().DrawScene(nullptr, EdViewport.View.get());
		}

		DrawUI();

		CurrentFrameTimeIndex = (CurrentFrameTimeIndex + 1) % MaxFrameTimes;
	}

	void Editor::DrawUI()
	{
		static bool bDockspaceOpen = true;
		static ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* ImGuiMainViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImGuiMainViewport->Pos);
		ImGui::SetNextWindowSize(ImGuiMainViewport->Size);
		ImGui::SetNextWindowViewport(ImGuiMainViewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (DockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		{
			WindowFlags |= ImGuiWindowFlags_NoBackground;
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &bDockspaceOpen, WindowFlags);
		ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);

		ImGuiID DockspaceID = ImGui::GetID("DockSpace");
		ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), DockspaceFlags);

		// Draw the top menu bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					GEngine->SignalQuit();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Add Viewport"))
				{
					if (Viewports.size() < 10)
					{
						Viewports.emplace_back(Renderer::Get().CreateViewport(1280, 720));
					}
				}
				if (ImGui::MenuItem("Output Log"))
				{
					// Create an output log
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// Draw individual viewports
		uint32_t ViewportIndex = 0;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		for (auto& EdViewport : Viewports)
		{
			char Buff[32];
			snprintf(Buff, 32, "%s Viewport##%d", ICON_MD_CROP_ORIGINAL, ViewportIndex);
			ImGui::Begin(Buff);

			EdViewport.bViewportIsFocused = ImGui::IsWindowFocused();
			EdViewport.bViewportIsHovered = ImGui::IsWindowHovered();

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			EdViewport.ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			uint64_t TextureID = EdViewport.View->GetColorAttachmentID();

			ImGui::Image(reinterpret_cast<void*>(TextureID), ImVec2{ EdViewport.ViewportSize.x, EdViewport.ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();

			ViewportIndex++;
		}
		ImGui::PopStyleVar();

		SceneHierarchy->Draw();
		PropertyEditor->Draw();

		ImGui::Begin("Stats");
		ImGui::Text("CPU");
		ImGui::Separator();
		float FrameTimeSum = 0.f;
		const uint32_t NumberFramesToAverageOver = 50;
		for (uint32_t i = 0; i < NumberFramesToAverageOver; ++i)
		{
			FrameTimeSum += FrameTimes[(CurrentFrameTimeIndex - i) % MaxFrameTimes];
		}
		const float FrameTimeAverage = FrameTimeSum / NumberFramesToAverageOver;
		ImVec2 ContentRegion = ImGui::GetContentRegionAvail();
		char Buff[32];
		snprintf(Buff, 32, "Frame time: %.2f ms", FrameTimeAverage);
		ImGui::PlotLines("", FrameTimes, IM_ARRAYSIZE(FrameTimes), CurrentFrameTimeIndex, Buff, 0.f, 20.f, ImVec2(ContentRegion.x, 80.0f));
		ImGui::Text("FPS: %.1f", (double)(1.f / FrameTimeAverage) * 1000.f);
		
		ImGui::Separator();
		ImGui::Text("Memory");
		ImGui::Text("Number of allocations: %lu", MemoryTracker::GetNumAllocs());
		ImGui::Text("Memory used: %lu bytes", MemoryTracker::GetMemUsed());

		ImGui::Text("GPU");
		ImGui::Separator();
		// No gpu stats available yet
		ImGui::End();

		ImGui::ShowDemoWindow();

		ImGui::End();
	}

	void Editor::SetSelection(const Array<Entity*>& NewSelection)
	{
		SelectedEntities.clear();
		for (Entity* Ent : NewSelection)
		{
			SelectedEntities.insert(Ent);
		}
		OnSelectionChanged.Broadcast();
	}
	
	void Editor::SetSelection(Entity* SelectedItem)
	{
		SelectedEntities.clear();
		SelectedEntities.insert(SelectedItem);
		OnSelectionChanged.Broadcast();
	}

	void Editor::AddToSelection(const Array<Entity*>& EntitiesToAdd)
	{
		for (Entity* Ent : EntitiesToAdd)
		{
			SelectedEntities.insert(Ent);
		}
		OnSelectionChanged.Broadcast();
	}

	void Editor::AddToSelection(Entity* EntityToAdd)
	{
		SelectedEntities.insert(EntityToAdd);
		OnSelectionChanged.Broadcast();
	}

	void Editor::ClearSelection()
	{
		SelectedEntities.clear();
		OnSelectionCleared.Broadcast();
	}

	void Editor::RemoveFromSelection(Entity* EntityToRemove)
	{
		SelectedEntities.erase(EntityToRemove);
		OnSelectionChanged.Broadcast();
	}

	bool Editor::IsEntitySelected(Entity* Ent) const
	{
		return SelectedEntities.find(Ent) != SelectedEntities.end();
	}
}