#pragma once

#include "Core/CoreMinimal.h"
#include "AssetManager/AssetHandle.h"

namespace ScarletEngine
{
	class AssetManager
	{
	public:
		static std::shared_ptr<TextureHandle> LoadTexture(const std::string& FilePath);

		static void UnloadAsset(const std::string& AssetToUnload);

	private:
		static std::shared_ptr<IAssetHandle> LoadAsset(const std::string& FilePath, AssetType Type);
	private:
		AssetManager() = default;
		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;
	private:
		static AssetManager Instance;

		static std::unordered_map<std::string, std::weak_ptr<IAssetHandle>> CachedAssets;
	};
}
