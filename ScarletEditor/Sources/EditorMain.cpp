#include "Core.h"
#include "Editor.h"
#include "RAL.h"
#include "StaticMeshComponent.h"
#include "AssetManager.h"

int main()
{
	using namespace ScarletEngine;

	GEngine = MakeUnique<Engine>();
	GEngine->Initialize();

	// initialize the global editor
	GEditor = MakeUnique<Editor>();
	// #todo_core: this should be handled automatically by the engine
	GEditor->Initialize();

	{
		// Test entity
		auto [Ent, Trans, Mesh] = GEditor->GetActiveWorld()->CreateEntity<Transform, StaticMeshComponent>("Monkey");

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
	}

	GEngine->Run();

	GEditor.reset();
	GEngine->Terminate();
	GEngine.reset();

	SCAR_LOG(LogInfo, "Terminating");

	return 0;
}