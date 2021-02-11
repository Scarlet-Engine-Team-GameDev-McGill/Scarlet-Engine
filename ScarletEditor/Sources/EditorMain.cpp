#include "Core.h"
#include "Engine.h"
#include "Editor.h"
#include "RAL.h"
#include "StaticMeshComponent.h"
#include "Ac_RigidBodyComponent.h"
#include "AssetManager.h"

using namespace ScarletEngine;

void makeCube(glm::vec3 Pos)
{
	auto [Ent, Trans, Mesh, Rb] = GEditor->GetActiveWorld()->CreateEntity<Transform, StaticMeshComponent, Ac_RigidBodyComponent>("Cube");

	Trans->Position = Pos;
	Trans->Rotation = glm::vec3(45.f, 45.f, 0.f);
	Trans->Scale = glm::vec3(0.05f);

	Rb->Mass = 1.f;

	Mesh->MeshHandle = AssetManager::LoadStaticMesh("/ScarletEngine/Content/Cube.obj");
	Mesh->VertexBuff = RAL::Get().CreateBuffer((uint32_t)Mesh->MeshHandle->Vertices.size() * sizeof(Vertex), RALBufferUsage::STATIC_DRAW);
	Mesh->VertexBuff->UploadData(Mesh->MeshHandle->Vertices.data(), Mesh->MeshHandle->Vertices.size() * sizeof(Vertex));
	Mesh->IndexBuff = RAL::Get().CreateBuffer((uint32_t)Mesh->MeshHandle->Indices.size() * sizeof(uint32_t), RALBufferUsage::STATIC_DRAW);
	Mesh->IndexBuff->UploadData(Mesh->MeshHandle->Indices.data(), Mesh->MeshHandle->Indices.size() * sizeof(uint32_t));
	Mesh->VertexArray = RAL::Get().CreateVertexArray(Mesh->VertexBuff, Mesh->IndexBuff);

	auto VertShader = RAL::Get().CreateShader(RALShaderStage::Vertex, "/ScarletEngine/Shaders/test_shader.vert");
	auto FragShader = RAL::Get().CreateShader(RALShaderStage::Pixel, "/ScarletEngine/Shaders/test_shader.frag");
	Mesh->Shader = RAL::Get().CreateShaderProgram(VertShader, FragShader, nullptr, nullptr);
	GlobalAllocator<RALShader>::Free(VertShader);
	GlobalAllocator<RALShader>::Free(FragShader);
};

int main()
{
	
	// #todo_Core: this should be loaded by a config file or something, for now default it to this.
	AssetManager::SetAssetRoot("../");

	GEngine = MakeUnique<Engine>();
	GEngine->Initialize();

	// initialize the global editor
	GEditor = MakeUnique<Editor>();
	// #todo_core: this should be handled automatically by the engine
	GEditor->Initialize();

	{
		// Test entity
		makeCube(glm::vec3(-10.f, 0.f, 0.f));
		makeCube(glm::vec3(10.f, 0.f, 0.f));
	}

	GEngine->Run();

	GEditor.reset();
	GEngine->Terminate();
	GEngine.reset();

	SCAR_LOG(LogInfo, "Terminating");

	return 0;
}