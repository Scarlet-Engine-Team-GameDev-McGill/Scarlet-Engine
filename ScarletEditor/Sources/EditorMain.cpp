#include "Core.h"
#include "Engine.h"
#include "Editor.h"
#include "RAL.h"
#include "StaticMeshComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "AssetManager.h"
#include "RenderModule.h"
#include "UIModule.h"

using namespace ScarletEngine;
using namespace Achilles;


void makeCube(glm::vec3 Pos, float Mass, glm::vec3 V0)
{
	auto [Ent, Trans, Mesh, Rb, Sphere] = GEditor->GetActiveWorld()->CreateEntity<Transform, StaticMeshComponent, RigidBodyComponent, SphereColliderComponent>("Sphere");

	Trans->Position = Pos;
	Trans->Rotation = glm::vec3(45.f, 45.f, 0.f);
	Trans->Scale = glm::vec3(0.05f);

	Rb->Mass = Mass;
	Rb->Velocity = V0;

	Sphere->Pos = Pos;
	Sphere->Radius = 2.f;

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
		MakeSphere(glm::vec3(0.f, 10.f, 0.f), 1.f, glm::vec3(0.f, 10.f, 0.f));

		auto [Ent, Plane] = GEditor->GetActiveWorld()->CreateEntity<PlaneColliderComponent>("Plane");
		Plane->Normal = glm::vec3(0.f, 1.f, 0.f);
		Plane->Distance = 10.f;
		Plane->FrictionCoefficient = 0.3f;
	}

	GEngine->Run();

	GEditor.reset();
	GEngine->Terminate();
	GEngine.reset();

	SCAR_LOG(LogInfo, "Terminating");

	return 0;
}
