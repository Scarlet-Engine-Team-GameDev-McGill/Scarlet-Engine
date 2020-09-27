#pragma once

#include "Core/CoreMinimal.h"
#include "AssetManager/AssetHandle.h"

namespace ScarletEngine
{
	class AssetManager
	{
	public:
		static SharedPtr<TextureHandle> LoadTextureFile(const String& FilePath);

		static SharedPtr<StaticMeshHandle> LoadStaticMesh(const String& FilePath);

		static void UnloadAsset(const String& AssetToUnload);

		static void ForEachLoadedAsset(const std::function<bool(IAssetHandle&)>& Func);
	private:
		static SharedPtr<IAssetHandle> LoadAsset(const String& FilePath, AssetType Type);
	private:
		AssetManager() = default;
		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;
	private:
		static UnorderedMap<String, WeakPtr<IAssetHandle>> CachedAssets;
	};
}
