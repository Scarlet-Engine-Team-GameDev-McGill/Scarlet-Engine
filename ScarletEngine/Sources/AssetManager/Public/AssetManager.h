#pragma once

#include "CoreMinimal.h"
#include "AssetHandle.h"

namespace ScarletEngine
{
	/** Manages asset loading and caching */
	class AssetManager
	{
	public:
		static void SetAssetRoot(const String& InAssetsSource);

		static SharedPtr<TextureHandle> LoadTextureFile(const String& FilePath);

		static SharedPtr<StaticMeshHandle> LoadStaticMesh(const String& FilePath);

		static void UnloadAsset(const String& AssetToUnload);

		static void ForEachLoadedAsset(const std::function<bool(IAssetHandle&)>& Func);

		static String ToFullPath(const String& AssetPath);
	private:
		static SharedPtr<IAssetHandle> LoadAsset_Impl(const String& FilePath, AssetType Type);
	private:
		AssetManager() = default;
		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;
	private:
		static UnorderedMap<String, WeakPtr<IAssetHandle>> CachedAssets;

		static String AssetRoot;
	};
}
