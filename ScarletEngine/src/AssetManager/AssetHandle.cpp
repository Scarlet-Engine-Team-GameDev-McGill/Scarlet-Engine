#include "AssetManager/AssetHandle.h"

#include "AssetManager/AssetManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ScarletEngine
{

	IAssetHandle::IAssetHandle(AssetType InType, const String& InFilePath)
		: Type(InType)
		, FilePath(InFilePath)
		, Leafname(FilePath)
		, Extension(Leafname)
	{
		ZoneScoped
		Leafname = Leafname.substr(Leafname.find_last_of("/") + 1);
		Extension = Extension.substr(Extension.find_last_of("."));
	}

	IAssetHandle::~IAssetHandle()
	{
		ZoneScoped
		// When nothing is referencing this asset, remove its entry in the cache
		AssetManager::UnloadAsset(FilePath);
	}

	TextureHandle::TextureHandle(const String& InFilePath)
		: IAssetHandle(AssetType::Texture, InFilePath)
		, PixelDataBuffer(nullptr)
	{
		ZoneScoped
		// potentially dangerous casting happening here
		int ImageWidth;
		int ImageHeight;
		int NumChannels;

		PixelDataBuffer = stbi_load(InFilePath.c_str(), &ImageWidth, &ImageHeight, &NumChannels, STBI_rgb_alpha);
		check(PixelDataBuffer != nullptr);

		Width = ImageWidth;
		Height = ImageHeight;
		Channels = (uint8_t)NumChannels;
	}

	TextureHandle::~TextureHandle()
	{
		ZoneScoped
		stbi_image_free(PixelDataBuffer);
	}

	// #todo: move this somewhere else
	static void LoadObj(const String& FilePath, Array<Vertex>& OutVertices, Array<uint32_t>& OutIndices)
	{
		std::vector<uint32_t> VertexIndices, UVIndices, NormalIndices;
		std::vector<glm::vec3> TempVertices;
		std::vector<glm::vec2> TempUVs;
		std::vector<glm::vec3> TempNormals;

		FILE* File = fopen(FilePath.c_str(), "r");
		if (File == nullptr)
		{
			SCAR_LOG(LogError, "Failed to load obj file from %s", FilePath.c_str());
			return;
		}

		// Parse the file into a more manageable format
		while (true)
		{
			char LineHeader[128];
			int32_t Result = fscanf(File, "%s", LineHeader);
			if (Result == EOF)
			{
				break;
			}

			if (strcmp(LineHeader, "v") == 0)
			{
				glm::vec3 Vertex;
				fscanf(File, "%f %f %f\n", &Vertex.x, &Vertex.y, &Vertex.z);
				TempVertices.push_back(Vertex);
			}
			else if (strcmp(LineHeader, "vn") == 0)
			{
				glm::vec3 Normal;
				fscanf(File, "%f %f %f\n", &Normal.x, &Normal.y, &Normal.z);
				TempNormals.push_back(Normal);
			}
			else if (strcmp(LineHeader, "vt") == 0)
			{
				glm::vec2 UV;
				fscanf(File, "%f %f\n", &UV.x, &UV.y);
				TempUVs.push_back(UV);
			}
			else if (strcmp(LineHeader, "f") == 0)
			{
				String Vertex1, Vertex2, Vertex3;
				uint32_t VertexIndex[3], UVIndex[3], NormalIndex[3];
				uint32_t NumMatches = fscanf(File, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &VertexIndex[0], &UVIndex[0], &NormalIndex[0], &VertexIndex[1], &UVIndex[1], &NormalIndex[1], &VertexIndex[2], &UVIndex[2], &NormalIndex[2]);
				if (NumMatches != 9) 
				{
					SCAR_LOG(LogError, "Can't understand OBJ file, try re-exporting with different options %s", FilePath.c_str());
					return;
				}
				VertexIndices.push_back(VertexIndex[0]);
				VertexIndices.push_back(VertexIndex[1]);
				VertexIndices.push_back(VertexIndex[2]);
				UVIndices.push_back(UVIndex[0]);
				UVIndices.push_back(UVIndex[1]);
				UVIndices.push_back(UVIndex[2]);
				NormalIndices.push_back(NormalIndex[0]);
				NormalIndices.push_back(NormalIndex[1]);
				NormalIndices.push_back(NormalIndex[2]);
			}
		}
	
		for (uint32_t Index = 0; Index < VertexIndices.size(); ++Index)
		{
			uint32_t VertexIndex = VertexIndices[Index];
			uint32_t NormalIndex = NormalIndices[Index];
			uint32_t UVIndex = UVIndices[Index];

			Vertex OutVertex;
			// we index at -1 because obj file format indices start at 1, not 0
			OutVertex.VertexPos = TempVertices[VertexIndex - 1];
			OutVertex.Normal = TempNormals[NormalIndex - 1];
			OutVertex.UV = TempUVs[UVIndex - 1];
			OutVertices.push_back(OutVertex);
			OutIndices.push_back(Index);
		}
	}

	StaticMeshHandle::StaticMeshHandle(const String& InFilePath)
		: IAssetHandle(AssetType::StaticMesh, InFilePath)
	{
		check(Extension == ".obj");
		LoadObj(InFilePath, OUT Vertices, OUT Indices);
	}

	StaticMeshHandle::~StaticMeshHandle()
	{
	}
}