#include "Core.h"
#include "Engine.h"
#include "Editor.h"
#include "RAL.h"
#include "StaticMeshComponent.h"
#include "AssetManager.h"
#include "RenderModule.h"
#include "UIModule.h"

#include "AchillesCore.h"

using namespace ScarletEngine;
using namespace ScarletEngine::Achilles;

#pragma region Demos

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


EID MakeSphereCollider(glm::vec3 Pos, glm::vec3 Scale = glm::vec3(0.1f), glm::vec3 V0 = glm::vec3(0.f), float Mass=1.f, float Radius = 0.0125f)
{
	auto [Ent, Trans, Mesh, Rb, Sphere] = GEditor->GetActiveWorld()->CreateEntity<Transform, StaticMeshComponent, RigidBodyComponent, SphereColliderComponent>("Ball");

	Trans->Position = Pos;
	Trans->Rotation = glm::vec3(0.f, 0.f, 0.f);
	Trans->Scale = Scale;
		
	Rb->Velocity = V0;
	Rb->Mass = Mass;

	Sphere->Radius = Radius;
	Sphere->Pos = Pos;
	Sphere->FrictionCoefficient = 0.15f;

	MakeMesh(Mesh, "/ScarletEngine/Content/Sphere.obj");

	return Ent->ID;
};


EID MakeGround(float Dist=0.f, float Friction=0.15f)
{
	auto [Ent, Plane] = GEditor->GetActiveWorld()->CreateEntity<PlaneColliderComponent>("Ground");

	Plane->Distance = Dist;
	Plane->Normal = glm::vec3(0.f, 1.f, 0.f);
	Plane->FrictionCoefficient = Friction;
	return Ent->ID;
};


EID AddSpring(EID Ent, std::vector<EID> Anchors, std::vector<float> Lengths = { 0.25f }, float Stiffness = 1000.f)
{
	SpringCollection* SpringArray = GEditor->GetActiveWorld()->AddComponent<SpringCollection>(Ent);

	for (int i = 0; i < Anchors.size(); i++)
	{
		SpringArray->emplace_back();
		SpringArray->at(i).Anchor = Anchors.at(i);
		SpringArray->at(i).RestLength = Lengths.size() == 1 ? Lengths.at(0) : Lengths.at(i);
		SpringArray->at(i).Stiffness = Stiffness;
	}

	return Ent;
};


EID MakeNode(glm::vec3 Pos, std::vector<EID> Anchors, std::vector<float> Lengths = { 0.25f })
{
	EID Ent = MakeSphereCollider(Pos, glm::vec3(0.05f));
	AddSpring(Ent, Anchors, Lengths);
	return Ent;
};

void DemoSpring()
{
	auto [Anchor, AnchorTrans] = GEditor->GetActiveWorld()->CreateEntity<Transform>("Anchor");

	EID AnchorPtr = Anchor->ID;
	AnchorTrans->Position = glm::vec3(2.f, 2.f, 0.f);
	MakeNode(glm::vec3(0.f, 2.f, 0.f), { AnchorPtr }, { 2.f });

	auto [Anchor2, AnchorTrans2] = GEditor->GetActiveWorld()->CreateEntity<Transform>("Anchor");

	EID AnchorPtr2 = Anchor2->ID;
	AnchorTrans2->Position = glm::vec3(-2.f, 2.f, 0.f);
	MakeNode(glm::vec3(-2.f, -0.5f, 0.f), { AnchorPtr2 }, { 2.f });
}


void DemoRope(glm::vec3 Interval)
{
	auto [Anchor, AnchorTrans] = GEditor->GetActiveWorld()->CreateEntity<Transform>("Anchor");

	glm::vec3 pos = glm::vec3(0.f, 2.f, 0.f);
	EID AnchorPtr = Anchor->ID;
	AnchorTrans->Position = pos;

	for (int i = 0; i < 15; i++)
	{
		pos -= Interval;
		AnchorPtr = MakeNode(pos, {AnchorPtr});
	}
}


void DemoSpherePlaneCollision()
{
	MakeSphereCollider(glm::vec3(5.f, 3.f, 0.f), glm::vec3(0.1f), glm::vec3(-1.5f, 0.f, 0.f), 1.0f, 0.25f);
	MakeGround();
}


void DemoSphereSphereCollision()
{
	MakeSphereCollider(glm::vec3(3.f, 3.f, 0.f), glm::vec3(0.1f), glm::vec3(-1.5f, 0.f, 0.f), 1.0f, 0.25f);
	MakeSphereCollider(glm::vec3(-3.f, 3.f, 0.f), glm::vec3(0.1f), glm::vec3(1.5f, 0.f, 0.f), 1.0f, 0.25f);

	MakeSphereCollider(glm::vec3(0.f, 2.f, -2.f), glm::vec3(0.1f), glm::vec3(0.f, 10.f, 0.f), 1.0f, 0.25f);
	MakeSphereCollider(glm::vec3(0.f, 4.f, -2.f), glm::vec3(0.1f), glm::vec3(0.f, 11.f, 0.f), 1.0f, 0.25f);
	MakeGround();
}


void DemoRopeCollision()
{
	DemoRope(glm::vec3(0.f, 0.25f, 0.f));
	MakeSphereCollider(glm::vec3(8.f, -3.f, 0.f), glm::vec3(0.1f), glm::vec3(-18.f, 0.f, 0.f), 15.f, 0.25f);
	MakeGround(-3.f, 0.1f);
}


void DemoFlag()
{
	const int height = 4;
	const int width = 7;
	float scale = 0.5f;
	float offset = 2.f;
	EID Grid[width][height];
	float sqrt2 = 1.414214f * scale;


	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (x == 0)
			{
				auto [Anchor, AnchorTrans] = GEditor->GetActiveWorld()->CreateEntity<Transform>("Anchor");
				AnchorTrans->Position = glm::vec3(0.f - offset, y * scale + offset * 0.5f, 0.f);
				Grid[x][y] = Anchor->ID;
			}
			else 
			{
				Grid[x][y] = MakeSphereCollider(glm::vec3(x * scale - offset, y * scale + offset * 0.5f, 0.f), glm::vec3(0.05f));
			}
		}
	}

	for (int x = 1; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			std::vector<EID> Anchors;
			std::vector<float> Lengths;
			
			if (x == 1)
			{
				Anchors.push_back(Grid[0][y]);
				Lengths.push_back(scale);
				if (y < height - 1)
				{
					Anchors.push_back(Grid[0][y+1]);
					Lengths.push_back(sqrt2);
				}
			}
			if (y < height - 1)
			{
				Anchors.push_back(Grid[x][y+1]);
				Lengths.push_back(scale);
			}
			if (x < width - 1)
			{
				Anchors.push_back(Grid[x+1][y]);
				Lengths.push_back(scale);
			}
			if (x < width - 1 && y < height - 1)
			{
				Anchors.push_back(Grid[x + 1][y + 1]);
				Lengths.push_back(sqrt2);
			}

			AddSpring(Grid[x][y], Anchors, Lengths, 100.f);
		}
	}

	MakeGround();
}

void DemoSoftBodies(float heightOffset = 2.f)
{
	const int height = 3;
	const int width = 3;
	const int depth = 3;
	float scale = 0.5f;
	float offset = 2.f;
	EID Grid[width][height][depth];
	float sqrt2 = 1.414214f * scale;
	float sqrt3 = 1.73205f * scale;


	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int z = 0; z < depth; z++)
			{
				Grid[x][y][z] = MakeSphereCollider(glm::vec3(x * scale - offset, y * scale + heightOffset, z * scale), glm::vec3(0.05f));
			}
		}
	}

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int z = 0; z < depth; z++)
			{
				std::vector<EID> Anchors;
				std::vector<float> Lengths;

				if (y < height - 1)
				{
					Anchors.push_back(Grid[x][y + 1][z]);
					Lengths.push_back(scale);
				}
				if (x < width - 1)
				{
					Anchors.push_back(Grid[x + 1][y][z]);
					Lengths.push_back(scale);
				}
				if (x < width - 1 && y < height - 1)
				{
					Anchors.push_back(Grid[x + 1][y + 1][z]);
					Lengths.push_back(sqrt2);
				}
				if (z < depth - 1)
				{
					Anchors.push_back(Grid[x][y][z+1]);
					Lengths.push_back(scale);
				}
				if (x < width - 1 && z < depth - 1)
				{
					Anchors.push_back(Grid[x + 1][y][z+1]);
					Lengths.push_back(sqrt2);
				}
				if (x < width - 1 && y < height - 1 && z < depth - 1)
				{
					Anchors.push_back(Grid[x + 1][y + 1][z + 1]);
					Lengths.push_back(sqrt3);
				}

				AddSpring(Grid[x][y][z], Anchors, Lengths, 1000.f);
			}
		}
	}

	MakeGround();
}

EID MakePlanet(glm::vec3 Pos, glm::vec3 V0, float Mass, float Scale, const char* Name)
{
	auto [Ent, Trans, Mesh, Rb] = GEditor->GetActiveWorld()->CreateEntity<Transform, StaticMeshComponent, RigidBodyComponent>(Name);

	Trans->Position = Pos;
	Trans->Rotation = glm::vec3(0.f, 0.f, 0.f);
	Trans->Scale = glm::vec3(Scale * 0.05f);

	Rb->Velocity = V0;
	Rb->Mass = Mass;
	Rb->UsesGravity = false;
	Rb->UsesKeplerGravity = true;

	MakeMesh(Mesh, "/ScarletEngine/Content/Sphere.obj");

	return Ent->ID;
};

void DemoKepler()
{
	MakePlanet(glm::vec3(4.3f, -1.f, 0.f), glm::vec3(0.f, 0.f, 2.f), 1., 1.2f, "Jupiter");
	MakePlanet(glm::vec3(3.f, 0.6f, 0.f), glm::vec3(0.f, 0.f, 2.5f), 1.f, 0.5f, "Venus");
	MakePlanet(glm::vec3(4.15f, -0.4f, 0.f), glm::vec3(0., 0.f, 2.5f), 1.f, .3f, "Moon");
	MakePlanet(glm::vec3(4.f, -0.4f, 0.f), glm::vec3(0.f, 0.f, 2.f), 7 * glm::pow(10, 8), 0.6f, "Earth");
	MakePlanet(glm::vec3(2.f, 0.2f, 0.f), glm::vec3(0.f, 0.f, 2.5f), 1.f, 0.5f, "Mercury");
	MakePlanet(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 3 * glm::pow(10, 11), 2.f, "Sun");
}

#pragma endregion

int main()
{
	// #todo_Core: this should be loaded by a config file or something, for now default it to this.
	AssetManager::SetAssetRoot("../");

	ModuleManager::GetInstance().RegisterModule<RenderModule>();
	ModuleManager::GetInstance().RegisterModule<UIModule>();

	SystemScheduler::Get().RegisterSystem<Achilles::SpringSystem>();
	SystemScheduler::Get().RegisterSystem<Achilles::RigidBodySystem>();
	SystemScheduler::Get().RegisterSystem<Achilles::AABBvsAABBColliderSystem>();
	SystemScheduler::Get().RegisterSystem<Achilles::SphereVsSphereColliderSystem>();
	SystemScheduler::Get().RegisterSystem<Achilles::PlaneVsSphereColliderSystem>();

	GEngine = MakeUnique<Engine>();
	GEngine->Initialize();

	// initialize the global editor
	GEditor = MakeUnique<Editor>();
	// #todo_core: this should be handled automatically by the engine
	GEditor->Initialize();
	{
		//DemoKepler();
		DemoSpherePlaneCollision();
		//DemoSphereSphereCollision();
		//DemoSpring();
		//DemoRope(glm::vec3(0.25f, 0.f, 0.f));
		//DemoRopeCollision();
		//DemoFlag();
		//DemoSoftBodies();
	}

	GEngine->Run();

	GEditor.reset();
	GEngine->Terminate();
	GEngine.reset();

	SCAR_LOG(LogInfo, "Terminating");

	return 0;
}
