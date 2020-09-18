#include "AssetManager/AssetHandle.h"

#include "AssetManager/AssetManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ScarletEngine
{

	IAssetHandle::IAssetHandle(AssetType InType, const std::string& InFilePath)
		: Type(InType)
		, FilePath(InFilePath)
	{}

	IAssetHandle::~IAssetHandle()
	{
		// When nothing is referencing this asset, remove its entry in the cache
		AssetManager::UnloadAsset(FilePath);
	}

	TextureHandle::TextureHandle(const std::string& InFilePath)
		: IAssetHandle(AssetType::Texture, InFilePath)
	{
		// potentially dangerous casting happening here
		int ImageWidth;
		int ImageHeight;
		int NumChannels;

		PixelDataBuffer = stbi_load(InFilePath.c_str(), &ImageWidth, &ImageHeight, &NumChannels, STBI_rgb_alpha);

		Width = ImageWidth;
		Height = ImageHeight;
		Channels = NumChannels;
	}

	TextureHandle::~TextureHandle()
	{
		stbi_image_free(PixelDataBuffer);
	}

}