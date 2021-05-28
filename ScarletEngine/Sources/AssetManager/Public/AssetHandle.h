#pragma once

#include "CoreMinimal.h"
#include "AssetTypes.h"

namespace ScarletEngine
{
	/** Abstract interface which handles all file-based assets. */
	class IAssetHandle
	{
	public:
		virtual ~IAssetHandle();

		/** The type of the asset */
		AssetType Type;
		/** The path of the asset relative to the engine content directory */
		String FilePath;
		/** the leaf name of the asset file */
		StringView Leafname;
		/** The file extension of the asset */
		StringView Extension;
	protected:
		// Constructor is protected to prevent instantiation of this abstract class
		IAssetHandle(AssetType InType, const String& InFilePath);
	};

	/** Represents a texture asset file */
	class TextureHandle final : public IAssetHandle
	{
	public:
		TextureHandle(const String& InFilePath);

		virtual ~TextureHandle();

		byte_t* PixelDataBuffer;
		uint32_t Width;
		uint32_t Height;
		uint8_t Channels;
	};

	// #todo_rendering: move this somewhere else
	struct Vertex
	{
		glm::vec3 VertexPos;
		glm::vec3 Normal;
		glm::vec2 UV;
	};

	/** Represents a static mesh asset file */
	class StaticMeshHandle final : public IAssetHandle
	{
	public:
		StaticMeshHandle(const String& InFilePath);
		virtual ~StaticMeshHandle();

		Array<Vertex> Vertices;
		Array<uint32_t> Indices;
	};
}