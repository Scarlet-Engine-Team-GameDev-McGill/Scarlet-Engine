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

}