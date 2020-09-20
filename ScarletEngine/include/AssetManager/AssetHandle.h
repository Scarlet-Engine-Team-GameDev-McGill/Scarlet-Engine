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
		String FilePath;
		std::string_view Leafname;
		std::string_view Extension;
	protected:
		// Constructor is protected to prevent instantiation of this abstract class
		IAssetHandle(AssetType InType, const String& InFilePath);
	};

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
}