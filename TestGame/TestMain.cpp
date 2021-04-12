#include "Engine.h"
#include "AssetManager.h"
#include "World.h"
#include "RAL.h"
#include "ModuleManager.h"
#include "RenderModule.h"

#include "VulkanRAL.h" // temp for testing

int main()
{
	using namespace ScarletEngine;

	// #todo_Core: this should be loaded by a config file or something, for now default it to this.
	AssetManager::SetAssetRoot("../");
	
	ModuleManager::RegisterModule<RenderModule>();

	GEngine = MakeUnique<Engine>();	
	GEngine->Initialize();

	{
		SharedPtr<World> TestWorld = MakeShared<World>();
		TestWorld->Initialize();
		
		const Array<Vertex> Vertices = {
            {{0.0f, -0.5f, 0.f}, {1.0f, 1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f}}
        };

		const Array<uint32_t> Indices = { 0, 1, 2};
		
		auto [Ent, Trans, Mesh] = TestWorld->CreateEntity<Transform, StaticMeshComponent>("TestEntity");

		Trans->Position = glm::vec3(0.f, 0.5f, 0.f);
		Trans->Rotation = glm::vec3(0.f, 0.f, 0.f);
		Trans->Scale = glm::vec3(0.5f);

		Mesh->VertexBuff = RAL::Get().CreateBuffer((uint32_t)Vertices.size() * sizeof(Vertex), RALBufferType::VERTEX_BUFFER, 
			RALBufferUsage::STATIC_DRAW, RALBufferPropertyFlagBits::HOST_COHERENT_BIT | RALBufferPropertyFlagBits::HOST_VISIBLE_BIT);
		Mesh->VertexBuff->UploadData((void*)Vertices.data(), Vertices.size() * sizeof(Vertex));
		
		Mesh->IndexBuff = RAL::Get().CreateBuffer((uint32_t)Indices.size() * sizeof(uint32_t), RALBufferType::INDEX_BUFFER,
			RALBufferUsage::STATIC_DRAW, RALBufferPropertyFlagBits::HOST_COHERENT_BIT | RALBufferPropertyFlagBits::HOST_VISIBLE_BIT);
		Mesh->IndexBuff->UploadData((void*)Indices.data(), Indices.size() * sizeof(uint32_t));
		
		Mesh->VertexArray = RAL::Get().CreateVertexArray(Mesh->VertexBuff, Mesh->IndexBuff);

		//auto VertShader = RAL::Get().CreateShader(RALShaderStage::Vertex, "/ScarletEngine/Shaders/test_shader.vert");
		//auto FragShader = RAL::Get().CreateShader(RALShaderStage::Pixel, "/ScarletEngine/Shaders/test_shader.frag");
		//Mesh->Shader = RAL::Get().CreateShaderProgram(VertShader, FragShader, nullptr, nullptr);
		//ScarDelete(VertShader);
		//ScarDelete(FragShader);

		GEngine->Run();
	}


	GEngine->Terminate();
	GEngine.reset();

	return 0;
}