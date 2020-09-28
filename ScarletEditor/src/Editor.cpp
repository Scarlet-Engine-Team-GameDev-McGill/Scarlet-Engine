#include "Editor.h"

#include "Renderer/Renderer.h"
#include "RAL/RAL.h"
#include <imgui.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer/StaticMeshComponent.h"

#include "AssetManager/AssetManager.h"

namespace ScarletEngine
{
	UniquePtr<Editor> GEditor = nullptr;

	Editor::Editor()
		: FrameTimes()
		, EditorWorld(nullptr)
		, SceneHierarchy(nullptr)
		, PropertyEditor(nullptr)
		, OutputLog(nullptr)
		, SelectedEntities()
		, EditorCam(nullptr)
	{
		ZoneScoped
	}

	void Editor::Initialize()
	{
		ZoneScoped
		EditorWorld = MakeShared<World>();
		SceneHierarchy = MakeShared<SceneHierarchyPanel>(EditorWorld);
		PropertyEditor = MakeShared<PropertyEditorPanel>();
		OutputLog = MakeShared<OutputLogPanel>();
		EditorCam = MakeShared<Camera>();
		Viewports.emplace_back(Renderer::Get().CreateViewport(1280, 720));

		// Test entities
		auto [Ent, Trans, Mesh] = EditorWorld->CreateEntity<Transform, StaticMeshComponent>("Entity 1");

		Trans->Position = glm::vec3(0.f);
		Trans->Rotation = glm::vec3(90.f, 0.f, 0.f);
		Trans->Scale = glm::vec3(0.5f);

		Mesh->MeshHandle = AssetManager::LoadStaticMesh("../ScarletEngine/content/Monkey.obj");
		Mesh->VertexBuff = RAL::Get().CreateBuffer((uint32_t)Mesh->MeshHandle->Vertices.size() * sizeof(Vertex), RALBufferUsage::STATIC_DRAW);
		Mesh->VertexBuff->UploadData(Mesh->MeshHandle->Vertices.data(), Mesh->MeshHandle->Vertices.size() * sizeof(Vertex));
		Mesh->IndexBuff = RAL::Get().CreateBuffer((uint32_t)Mesh->MeshHandle->Indices.size() * sizeof(uint32_t), RALBufferUsage::STATIC_DRAW);
		Mesh->IndexBuff->UploadData(Mesh->MeshHandle->Indices.data(), Mesh->MeshHandle->Indices.size() * sizeof(uint32_t));
		Mesh->VertexArray = RAL::Get().CreateVertexArray(Mesh->VertexBuff, Mesh->IndexBuff);

		auto VertShader = RAL::Get().CreateShader(RALShaderStage::Vertex, "../ScarletEngine/shaders/test_shader.vert");
		auto FragShader = RAL::Get().CreateShader(RALShaderStage::Pixel, "../ScarletEngine/shaders/test_shader.frag");
		Mesh->Shader = RAL::Get().CreateShaderProgram(VertShader, FragShader, nullptr, nullptr);
		GlobalAllocator<RALShader>::Free(VertShader);
		GlobalAllocator<RALShader>::Free(FragShader);

		EditorCam->SetPosition({ 0, -2, 0 });
		EditorCam->SetFoV(45.f);
		EditorCam->SetAspectRatio((float)1280 / (float)720);
		Viewports.back().View->SetCamera(EditorCam);
	}

	void Editor::Tick(double DeltaTime)
	{
		ZoneScoped
		FrameTimes[CurrentFrameTimeIndex] = (float)DeltaTime;

		for (const auto& EdViewport : Viewports)
		{
			glm::ivec2 ViewportFramebufferSize = EdViewport.View->GetSize();
			if ((EdViewport.ViewportSize.x >= 1.f && EdViewport.ViewportSize.y >= 1.f) &&
				(std::fabs((float)ViewportFramebufferSize.x - EdViewport.ViewportSize.x) > 1.0 ||
				std::fabs((float)ViewportFramebufferSize.y - EdViewport.ViewportSize.y) > 1.0))
			{
				EdViewport.View->ResizeFramebuffer((uint32_t)EdViewport.ViewportSize.x, (uint32_t)EdViewport.ViewportSize.y);
				if (EdViewport.ViewportSize.y > 0)
				{
					EditorCam->SetAspectRatio(EdViewport.ViewportSize.x / EdViewport.ViewportSize.y);
				}
				SCAR_LOG(LogVerbose, "Framebuffer Resized");
			}

			Renderer::Get().DrawScene(EditorWorld->GetRenderSceneProxy(), EdViewport.View.get());
			
		}

		DrawUI();

		CurrentFrameTimeIndex = (CurrentFrameTimeIndex + 1) % MaxFrameTimes;
	}

	void Editor::DrawUI()
	{
		ZoneScoped
		static bool bDockspaceOpen = true;
		static ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;

		{
			ZoneScopedN("Editor Dockspace")
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
		}

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

		{
			ZoneScopedN("Draw Editor Viewports")
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
		}

		SceneHierarchy->Draw();
		PropertyEditor->Draw();
		OutputLog->Draw();

		{
			ZoneScopedN("Draw Stats Panel")
			ImGui::Begin("Stats");
			ImGui::Text("CPU");
			ImGui::Separator();
			float FrameTimeSum = 0.f;
			const uint32_t NumberFramesToAverageOver = 50;
			{
				ZoneScopedN("Calc Average Frametime")
				for (uint32_t i = 0; i < NumberFramesToAverageOver; ++i)
				{
					FrameTimeSum += FrameTimes[(CurrentFrameTimeIndex - i) % MaxFrameTimes];
				}
			}
			const float FrameTimeAverage = FrameTimeSum / NumberFramesToAverageOver;
			ImVec2 ContentRegion = ImGui::GetContentRegionAvail();
			char Buff[32];
			{
				ZoneScopedN("Format string");
				snprintf(Buff, 32, "Frame time: %.2f ms", FrameTimeAverage);
			}
			{
				ZoneScopedN("Plot Frametimes")
				ImGui::PlotLines("", FrameTimes, IM_ARRAYSIZE(FrameTimes), CurrentFrameTimeIndex, Buff, 0.f, 20.f, ImVec2(ContentRegion.x, 80.0f));
			}
			ImGui::Text("FPS: %.1f", (double)(1.f / FrameTimeAverage) * 1000.f);

			ImGui::Separator();
			ImGui::Text("Memory");
			ImGui::Text("Number of allocations: %lu", MemoryTracker::Get().GetNumAllocs());
			ImGui::Text("Memory used: %.2f KB", MemoryTracker::Get().GetMemUsed() / 1024.f);

			ImGui::Text("GPU");
			ImGui::Separator();
			// No gpu stats available yet
			ImGui::End();
		}

		{
			ZoneScopedN("Demo Window")
			ImGui::ShowDemoWindow();
		}

		ImGui::End();
	}

	void Editor::SetSelection(const Array<Entity*>& NewSelection)
	{
		ZoneScoped
		SelectedEntities.clear();
		for (Entity* Ent : NewSelection)
		{
			SelectedEntities.insert(Ent);
		}
		OnSelectionChanged.Broadcast();
	}
	
	void Editor::SetSelection(Entity* SelectedItem)
	{
		ZoneScoped
		SelectedEntities.clear();
		SelectedEntities.insert(SelectedItem);
		OnSelectionChanged.Broadcast();
	}

	void Editor::AddToSelection(const Array<Entity*>& EntitiesToAdd)
	{
		ZoneScoped
		for (Entity* Ent : EntitiesToAdd)
		{
			SelectedEntities.insert(Ent);
		}
		OnSelectionChanged.Broadcast();
	}

	void Editor::AddToSelection(Entity* EntityToAdd)
	{
		ZoneScoped
		SelectedEntities.insert(EntityToAdd);
		OnSelectionChanged.Broadcast();
	}

	void Editor::ClearSelection()
	{
		ZoneScoped
		SelectedEntities.clear();
		OnSelectionCleared.Broadcast();
	}

	void Editor::RemoveFromSelection(Entity* EntityToRemove)
	{
		ZoneScoped
		SelectedEntities.erase(EntityToRemove);
		OnSelectionChanged.Broadcast();
	}

	bool Editor::IsEntitySelected(Entity* Ent) const
	{
		ZoneScoped
		return SelectedEntities.find(Ent) != SelectedEntities.end();
	}
}