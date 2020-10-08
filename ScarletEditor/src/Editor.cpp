#include "Editor.h"

#include "Renderer/Renderer.h"
#include "RAL/RAL.h"
#include <imgui.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer/StaticMeshComponent.h"
#include "UI/UISystem.h"
#include "EditorUILayer.h"
#include "AssetManager/AssetManager.h"

namespace ScarletEngine
{
	UniquePtr<Editor> GEditor = nullptr;

	Editor::Editor()
		: EditorWorld(nullptr)
		, SelectedEntities()
		, EditorCam(nullptr)
		, Viewports()
	{
	}

	void Editor::Initialize()
	{
		ZoneScoped
		EditorWorld = MakeShared<World>();
		EditorCam = MakeShared<Camera>();
		Viewports.emplace_back(Renderer::Get().CreateViewport(1280, 720));

		// Test entities
		auto [Ent, Trans, Mesh] = EditorWorld->CreateEntity<Transform, StaticMeshComponent>("Monkey");

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

		UISystem::Get().SetActiveLayer(MakeShared<EditorUILayer>());
	}

	void Editor::Tick(double)
	{
		ZoneScoped

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
	}

	void Editor::DrawUI()
	{
		ZoneScoped

		{
			ZoneScopedN("Draw Editor Viewports")
			// Draw individual viewports
			uint32_t ViewportIndex = 0;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			for (auto& EdViewport : Viewports)
			{
				char Buff[32];
				snprintf(Buff, 32, "%s Viewport##%d", ICON_MD_CROP_ORIGINAL, ViewportIndex);
				ImGuiWindowClass WindowClass;
				WindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
				ImGui::SetNextWindowClass(&WindowClass);
				ImGui::Begin(Buff);
				EdViewport.bViewportIsFocused = ImGui::IsWindowFocused();
				EdViewport.bViewportIsHovered = ImGui::IsWindowHovered();

				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
				EdViewport.ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

				uint64_t TextureID = EdViewport.View->GetColorAttachmentID();

				ImGui::Image(reinterpret_cast<void*>(TextureID), ImVec2{ EdViewport.ViewportSize.x, EdViewport.ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }); \
				ImGui::End();

				ViewportIndex++;
			}
			ImGui::PopStyleVar();
		}
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

	bool Editor::AddViewport()
	{
		if (Viewports.size() < MaxViewports)
		{
			Viewports.emplace_back(Renderer::Get().CreateViewport(1280, 720));
			// #todo: each viewport should have its own camera.
			Viewports.back().View->SetCamera(EditorCam);
			return true;
		}

		return false;
	}
}