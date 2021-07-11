#include "Engine.h"
#include "AssetManager.h"
#include "World.h"
#include "ModuleManager.h"
#include "RenderModule.h"

#include "VulkanRAL.h" // temp for testing

int main()
{
	using namespace ScarletEngine;

	// #todo_Core: this should be loaded by a config file or something, for now default it to this.
	AssetManager::SetAssetRoot("../");
	
	ModuleManager::RegisterModule<RenderModule>();

	GEngine = ScarNew(Engine);
	GEngine->Initialize();
 
	{
		const Array<Vertex> Vertices = {
            {{0.0f, -0.5f, 0.f}, {1.0f, 1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f}}
        };

		const Array<uint32_t> Indices = { 0, 1, 2};
		
		auto [Ent, Trans, Mesh] = GEngine->GetActiveWorld()->CreateEntity<TransformComponent, StaticMeshComponent>("TestEntity");

		Trans->Position = glm::vec3(0.f, 0.5f, 0.f);
		Trans->Rotation = glm::vec3(0.f, 0.f, 0.f);
		Trans->Scale = glm::vec3(0.5f);

		Mesh->MeshHandle = AssetManager::LoadStaticMesh("/ScarletEngine/Content/Monkey.obj");
		Mesh->VertexBuff = RAL::Get().CreateBuffer((uint32_t)Mesh->MeshHandle->Vertices.size() * sizeof(Vertex), 
			RALBufferType::VERTEX_BUFFER, RALBufferUsage::STATIC_DRAW, RALBufferPropertyFlagBits::HOST_COHERENT_BIT | RALBufferPropertyFlagBits::HOST_VISIBLE_BIT);
		Mesh->VertexBuff->UploadData(Mesh->MeshHandle->Vertices.data(), Mesh->MeshHandle->Vertices.size() * sizeof(Vertex));
		Mesh->IndexBuff = RAL::Get().CreateBuffer((uint32_t)Mesh->MeshHandle->Indices.size() * sizeof(uint32_t),
			RALBufferType::VERTEX_BUFFER, RALBufferUsage::STATIC_DRAW, RALBufferPropertyFlagBits::HOST_COHERENT_BIT | RALBufferPropertyFlagBits::HOST_VISIBLE_BIT);
		Mesh->IndexBuff->UploadData(Mesh->MeshHandle->Indices.data(), Mesh->MeshHandle->Indices.size() * sizeof(uint32_t));
		Mesh->VertexArray = RAL::Get().CreateVertexArray(Mesh->VertexBuff, Mesh->IndexBuff);

		auto VertShader = RAL::Get().CreateShader(RALShaderStage::Vertex, "/ScarletEngine/Shaders/test_shader.vert");
		auto FragShader = RAL::Get().CreateShader(RALShaderStage::Pixel, "/ScarletEngine/Shaders/test_shader.frag");
		Mesh->Shader = RAL::Get().CreateShaderProgram(VertShader, FragShader, nullptr, nullptr);
		ScarDelete(VertShader);
		ScarDelete(FragShader);

		// initialize anything you want in the world above this commment
		GEngine->Run();
	}

	ScarDelete(GEngine);

	return 0;
}