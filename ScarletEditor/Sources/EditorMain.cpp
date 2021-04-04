#include "Core.h"
#include "Engine.h"
#include "Editor.h"
#include "RAL.h"
#include "StaticMeshComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "SpringComponent.h"
#include "AssetManager.h"
#include "RenderModule.h"
#include "UIModule.h"

using namespace ScarletEngine;
using namespace Achilles;

void MakeMesh(StaticMeshComponent* Mesh, ScarletEngine::String Model)
{
	Mesh->MeshHandle = AssetManager::LoadStaticMesh(Model);
	Mesh->VertexBuff = RAL::Get().CreateBuffer((uint32_t)Mesh->MeshHandle->Vertices.size() * sizeof(Vertex), RALBufferUsage::STATIC_DRAW);
	Mesh->VertexBuff->UploadData(Mesh->MeshHandle->Vertices.data(), Mesh->MeshHandle->Vertices.size() * sizeof(Vertex));
	Mesh->IndexBuff = RAL::Get().CreateBuffer((uint32_t)Mesh->MeshHandle->Indices.size() * sizeof(uint32_t), RALBufferUsage::STATIC_DRAW);
	Mesh->IndexBuff->UploadData(Mesh->MeshHandle->Indices.data(), Mesh->MeshHandle->Indices.size() * sizeof(uint32_t));
	Mesh->VertexArray = RAL::Get().CreateVertexArray(Mesh->VertexBuff, Mesh->IndexBuff);

	auto VertShader = RAL::Get().CreateShader(RALShaderStage::Vertex, "/ScarletEngine/Shaders/test_shader.vert");
	auto FragShader = RAL::Get().CreateShader(RALShaderStage::Pixel, "/ScarletEngine/Shaders/test_shader.frag");
	Mesh->Shader = RAL::Get().CreateShaderProgram(VertShader, FragShader, nullptr, nullptr);
	ScarDelete(VertShader);
	ScarDelete(FragShader);
}


EID MakeNode(glm::vec3 Pos, glm::vec3 V0, EID Anchor)
{
	auto [Ent, Trans, Mesh, Rb, Spring] = GEditor->GetActiveWorld()->CreateEntity<Transform, StaticMeshComponent, RigidBodyComponent, SpringComponent>("Node");

	Trans->Position = Pos;
	Trans->Rotation = glm::vec3(0.f, 0.f, 0.f);
	Trans->Scale = glm::vec3(0.04f);

	Rb->Velocity = V0;

	Spring->Anchor = Anchor;

	MakeMesh(Mesh, "/ScarletEngine/Content/Sphere.obj");

	return Ent->ID;
};


void MakeRope()
{
	auto [Anchor, AnchorTrans] = GEditor->GetActiveWorld()->CreateEntity<Transform>("Anchor");

	glm::vec3 pos = glm::vec3(0.f, 2.f, 0.f);
	EID AnchorPtr = Anchor->ID;
	AnchorTrans->Position = pos;

	for (int i = 0; i < 15; i++)
	{
		pos -= glm::vec3(0.2f, 0.f, 0.f);
		AnchorPtr = MakeNode(pos, glm::vec3(0.f, 0.f, 0.f), AnchorPtr);
	}
}

int main()
{
	// #todo_Core: this should be loaded by a config file or something, for now default it to this.
	AssetManager::SetAssetRoot("../");

	ModuleManager::GetInstance().RegisterModule<RenderModule>();
	ModuleManager::GetInstance().RegisterModule<UIModule>();

	GEngine = MakeUnique<Engine>();
	GEngine->Initialize();

	// initialize the global editor
	GEditor = MakeUnique<Editor>();
	// #todo_core: this should be handled automatically by the engine
	GEditor->Initialize();
	
	{
		// Test entity
		MakeRope();
	}

	GEngine->Run();

	GEditor.reset();
	GEngine->Terminate();
	GEngine.reset();

	SCAR_LOG(LogInfo, "Terminating");

	return 0;
}
