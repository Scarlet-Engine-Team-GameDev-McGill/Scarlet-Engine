#pragma once

#include "Core/CoreMinimal.h"
#include "AssetManager/AssetHandle.h"

namespace ScarletEngine
{
	class AssetManager
	{
	public:
		static SharedPtr<TextureHandle> LoadTextureFile(const std::string& FilePath);

		static void UnloadAsset(const std::string& AssetToUnload);

		static void ForEachLoadedAsset(const std::function<bool(IAssetHandle&)>& Func);
	private:
		static SharedPtr<IAssetHandle> LoadAsset(const std::string& FilePath, AssetType Type);
	private:
		AssetManager() = default;
		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;
	private:
		static std::unordered_map<std::string, WeakPtr<IAssetHandle>> CachedAssets;
	};
}
