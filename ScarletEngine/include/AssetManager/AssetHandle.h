#pragma once

#include "Core/CoreMinimal.h"
#include "AssetTypes.h"

namespace ScarletEngine
{
	class IAssetHandle
	{
	public:
		virtual ~IAssetHandle();

		AssetType Type;
		std::string FilePath;
	protected:
		// Constructor is protected to prevent instantiation of this abstract class
		IAssetHandle(AssetType InType, const std::string& InFilePath);
	};

	class TextureHandle final : public IAssetHandle
	{
	public:
		TextureHandle(const std::string& InFilePath);

		virtual ~TextureHandle();

		byte_t* PixelDataBuffer;
		uint32_t Width;
		uint32_t Height;
		uint8_t Channels;
	};
}