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
	{
	}

	void Editor::Initialize()
	{
		ZoneScoped
		EditorWorld = MakeShared<World>();

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

		UISystem::Get().SetActiveLayer(MakeShared<EditorUILayer>());
	}

	void Editor::Tick(double)
	{
		ZoneScoped

		DrawUI();
	}

	void Editor::DrawUI()
	{
		ZoneScoped

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