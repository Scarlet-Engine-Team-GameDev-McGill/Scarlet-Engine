#pragma once

#include "Core.h"
#include "AchillesCore.h"
#include "StaticMeshComponent.h"
#include "World.h"
#include "AssetManager.h"

namespace ScarletEngine::Achilles
{
	void MakeMesh(StaticMeshComponent* Mesh, ScarletEngine::String Model)
	{
		Mesh->MeshHandle = AssetManager::LoadStaticMesh(Model);
		Mesh->VertexBuff = RAL::Get().CreateBuffer((uint32_t)Mesh->MeshHandle->Vertices.size() * sizeof(Vertex), RALBufferType::VERTEX_BUFFER,
			RALBufferUsage::STATIC_DRAW, RALBufferPropertyFlagBits::DEVICE_LOCAL_BIT);
		Mesh->VertexBuff->UploadData(Mesh->MeshHandle->Vertices.data(), Mesh->MeshHandle->Vertices.size() * sizeof(Vertex));
		Mesh->IndexBuff = RAL::Get().CreateBuffer((uint32_t)Mesh->MeshHandle->Indices.size() * sizeof(uint32_t), RALBufferType::INDEX_BUFFER,
			RALBufferUsage::STATIC_DRAW, RALBufferPropertyFlagBits::DEVICE_LOCAL_BIT);
		Mesh->IndexBuff->UploadData(Mesh->MeshHandle->Indices.data(), Mesh->MeshHandle->Indices.size() * sizeof(uint32_t));
		Mesh->VertexArray = RAL::Get().CreateVertexArray(Mesh->VertexBuff, Mesh->IndexBuff);

		auto VertShader = RAL::Get().CreateShader(RALShaderStage::Vertex, "/ScarletEngine/Shaders/test_shader.vert");
		auto FragShader = RAL::Get().CreateShader(RALShaderStage::Pixel, "/ScarletEngine/Shaders/test_shader.frag");
		Mesh->Shader = RAL::Get().CreateShaderProgram(VertShader, FragShader, nullptr, nullptr);
		ScarDelete(VertShader);
		ScarDelete(FragShader);
	}


	EID MakeSphereCollider(const SharedPtr<World>& World, glm::vec3 Pos, glm::vec3 Scale = glm::vec3(0.1f), glm::vec3 V0 = glm::vec3(0.f), float Mass = 1.f, float Radius = 0.0125f)
	{
		auto [Ent, Trans, Mesh, Rb, Sphere] = World->CreateEntity<TransformComponent, StaticMeshComponent, RigidBodyComponent, SphereColliderComponent>("Ball");

		Trans->Position = Pos;
		Trans->Rotation = glm::vec3(0.f, 0.f, 0.f);
		Trans->Scale = Scale;

		Rb->Velocity = V0;
		Rb->Mass = Mass;

		Sphere->Radius = Radius;
		Sphere->Pos = Pos;
		Sphere->FrictionCoefficient = 0.15f;

		MakeMesh(Mesh, "/ScarletEngine/Content/Sphere.obj");

		return Ent;
	};


	EID MakeGround(const SharedPtr<World>& World, float Dist = 0.f, float Friction = 0.15f)
	{
		auto [Ent, Plane] = World->CreateEntity<PlaneColliderComponent>("Ground");

		Plane->Distance = Dist;
		Plane->Normal = glm::vec3(0.f, 1.f, 0.f);
		Plane->FrictionCoefficient = Friction;
		return Ent;
	};


	EID AddSpring(const SharedPtr<World>& World, EID Ent, std::vector<EID> Anchors, std::vector<float> Lengths = { 0.25f }, float Stiffness = 1000.f)
	{
		SpringCollection* SpringArray = World->AddComponent<SpringCollection>(Ent);

		for (int i = 0; i < (int)Anchors.size(); i++)
		{
			SpringArray->EmplaceBack();
			SpringArray->At(i).Anchor = Anchors.at(i);
			SpringArray->At(i).RestLength = Lengths.size() == 1 ? Lengths.at(0) : Lengths.at(i);
			SpringArray->At(i).Stiffness = Stiffness;
		}

		return Ent;
	};


	EID MakeNode(const SharedPtr<World>& World, glm::vec3 Pos, std::vector<EID> Anchors, std::vector<float> Lengths = { 0.25f })
	{
		EID Ent = MakeSphereCollider(World, Pos, glm::vec3(0.05f));
		AddSpring(World, Ent, Anchors, Lengths);
		return Ent;
	};

	void DemoSpring(const SharedPtr<World>& World)
	{
		auto [Anchor, AnchorTrans] = World->CreateEntity<TransformComponent>("Anchor");

		EID AnchorPtr = Anchor;
		AnchorTrans->Position = glm::vec3(2.f, 2.f, 0.f);
		MakeNode(World, glm::vec3(0.f, 2.f, 0.f), { AnchorPtr }, { 2.f });

		auto [Anchor2, AnchorTrans2] = World->CreateEntity<TransformComponent>("Anchor");

		EID AnchorPtr2 = Anchor2;
		AnchorTrans2->Position = glm::vec3(-2.f, 2.f, 0.f);
		MakeNode(World, glm::vec3(-2.f, -0.5f, 0.f), { AnchorPtr2 }, { 2.f });
	}


	void DemoRope(const SharedPtr<World>& World, glm::vec3 Interval)
	{
		auto [Anchor, AnchorTrans] = World->CreateEntity<TransformComponent>("Anchor");

		glm::vec3 pos = glm::vec3(0.f, 2.f, 0.f);
		EID AnchorPtr = Anchor;
		AnchorTrans->Position = pos;

		for (int i = 0; i < 15; i++)
		{
			pos -= Interval;
			AnchorPtr = MakeNode(World, pos, { AnchorPtr });
		}
	}


	void DemoSpherePlaneCollision(const SharedPtr<World>& World)
	{
		MakeSphereCollider(World, glm::vec3(5.f, 3.f, 0.f), glm::vec3(0.1f), glm::vec3(-1.5f, 0.f, 0.f), 1.0f, 0.25f);
		MakeGround(World);
	}


	void DemoSphereSphereCollision(const SharedPtr<World>& World)
	{
		MakeSphereCollider(World, glm::vec3(3.f, 3.f, 0.f), glm::vec3(0.1f), glm::vec3(-1.5f, 0.f, 0.f), 1.0f, 0.25f);
		MakeSphereCollider(World,glm::vec3(-3.f, 3.f, 0.f), glm::vec3(0.1f), glm::vec3(1.5f, 0.f, 0.f), 1.0f, 0.25f);

		MakeSphereCollider(World, glm::vec3(0.f, 2.f, -2.f), glm::vec3(0.1f), glm::vec3(0.f, 10.f, 0.f), 1.0f, 0.25f);
		MakeSphereCollider(World, glm::vec3(0.f, 4.f, -2.f), glm::vec3(0.1f), glm::vec3(0.f, 11.f, 0.f), 1.0f, 0.25f);
		MakeGround(World);
	}


	void DemoRopeCollision(const SharedPtr<World>& World)
	{
		DemoRope(World, glm::vec3(0.f, 0.25f, 0.f));
		MakeSphereCollider(World, glm::vec3(8.f, -3.f, 0.f), glm::vec3(0.1f), glm::vec3(-18.f, 0.f, 0.f), 15.f, 0.25f);
		MakeGround(World, -3.f, 0.1f);
	}


	void DemoFlag(const SharedPtr<World>& World)
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
					auto [Anchor, AnchorTrans] = World->CreateEntity<TransformComponent>("Anchor");
					AnchorTrans->Position = glm::vec3(0.f - offset, y * scale + offset * 0.5f, 0.f);
					Grid[x][y] = Anchor;
				}
				else
				{
					Grid[x][y] = MakeSphereCollider(World, glm::vec3(x * scale - offset, y * scale + offset * 0.5f, 0.f), glm::vec3(0.05f));
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
						Anchors.push_back(Grid[0][y + 1]);
						Lengths.push_back(sqrt2);
					}
				}
				if (y < height - 1)
				{
					Anchors.push_back(Grid[x][y + 1]);
					Lengths.push_back(scale);
				}
				if (x < width - 1)
				{
					Anchors.push_back(Grid[x + 1][y]);
					Lengths.push_back(scale);
				}
				if (x < width - 1 && y < height - 1)
				{
					Anchors.push_back(Grid[x + 1][y + 1]);
					Lengths.push_back(sqrt2);
				}

				AddSpring(World, Grid[x][y], Anchors, Lengths, 100.f);
			}
		}

		//MakeGround(World);
	}

	void DemoSoftBodies(const SharedPtr<World>& World, float heightOffset = 2.f)
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
					Grid[x][y][z] = MakeSphereCollider(World, glm::vec3(x * scale - offset, y * scale + heightOffset, z * scale), glm::vec3(0.05f));
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
						Anchors.push_back(Grid[x][y][z + 1]);
						Lengths.push_back(scale);
					}
					if (x < width - 1 && z < depth - 1)
					{
						Anchors.push_back(Grid[x + 1][y][z + 1]);
						Lengths.push_back(sqrt2);
					}
					if (x < width - 1 && y < height - 1 && z < depth - 1)
					{
						Anchors.push_back(Grid[x + 1][y + 1][z + 1]);
						Lengths.push_back(sqrt3);
					}

					AddSpring(World, Grid[x][y][z], Anchors, Lengths, 1000.f);
				}
			}
		}

		MakeGround(World);
	}

	EID MakePlanet(const SharedPtr<World>& World, glm::vec3 Pos, glm::vec3 V0, float Mass, float Scale, const char* Name)
	{
		auto [Ent, Trans, Mesh, Rb] = World->CreateEntity<TransformComponent, StaticMeshComponent, RigidBodyComponent>(Name);

		Trans->Position = Pos;
		Trans->Rotation = glm::vec3(0.f);
		Trans->Scale = glm::vec3(Scale * 0.05f);

		Rb->Velocity = V0;
		Rb->Mass = Mass;
		Rb->Gravity = glm::vec3(0.f);
		Rb->bUsesKeplerGravity = true;

		MakeMesh(Mesh, "/ScarletEngine/Content/Sphere.obj");

		return Ent;
	};

	void DemoKepler(const SharedPtr<World>& World)
	{
		MakePlanet(World, glm::vec3(4.3f, -1.f, 0.f), glm::vec3(0.f, 0.f, 2.f), 1., 1.2f, "Jupiter");
		MakePlanet(World, glm::vec3(3.f, 0.6f, 0.f), glm::vec3(0.f, 0.f, 2.5f), 1.f, 0.5f, "Venus");
		MakePlanet(World, glm::vec3(4.15f, -0.4f, 0.f), glm::vec3(0.f, 0.f, 2.5f), 1.f, .3f, "Moon");
		MakePlanet(World, glm::vec3(4.f, -0.4f, 0.f), glm::vec3(0.f, 0.f, 2.f), 7.f * glm::pow(10.f, 8.f), 0.6f, "Earth");
		MakePlanet(World, glm::vec3(2.f, 0.2f, 0.f), glm::vec3(0.f, 0.f, 2.5f), 1.f, 0.5f, "Mercury");
		MakePlanet(World, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 3.f * glm::pow(10.f, 11.f), 2.f, "Sun");
	}
}