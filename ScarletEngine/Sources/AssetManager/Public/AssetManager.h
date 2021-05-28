#pragma once

#include "CoreMinimal.h"
#include "AssetHandle.h"

namespace ScarletEngine
{
	/** Manages asset loading and caching */
	class AssetManager
	{
	public:
		/** Set the root path for content */
		static void SetAssetRoot(const String& InAssetRoot);

		/** Load a texture from a file */
		static SharedPtr<TextureHandle> LoadTextureFile(const String& FilePath);

		/** Load a static mesh from a file */
		static SharedPtr<StaticMeshHandle> LoadStaticMesh(const String& FilePath);

		/** Unload an asset by path */
		static void UnloadAsset(const String& AssetToUnload);

		static void ForEachLoadedAsset(const std::function<bool(IAssetHandle&)>& Func);

		/** Convert an asset path to a full operating system path */
		static String ToFullPath(const String& AssetPath);
	private:
		static SharedPtr<IAssetHandle> LoadAsset_Impl(const String& FilePath, AssetType Type);
	private:
		// Asset Manager is a static class so delete constructors as there should never be an instance of this class.
		AssetManager() = default;
		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;
	private:
		static UnorderedMap<String, WeakPtr<IAssetHandle>> CachedAssets;

		static String AssetRoot;
	};
}
