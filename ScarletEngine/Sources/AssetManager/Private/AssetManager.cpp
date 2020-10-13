#include "AssetManager.h"

namespace ScarletEngine
{
	UnorderedMap<String, WeakPtr<IAssetHandle>> AssetManager::CachedAssets;

	SharedPtr<TextureHandle> AssetManager::LoadTextureFile(const String& FilePath)
	{
		ZoneScoped
		return std::static_pointer_cast<TextureHandle>(LoadAsset(FilePath, AssetType::Texture));
	}

	SharedPtr<StaticMeshHandle> AssetManager::LoadStaticMesh(const String& FilePath)
	{
		ZoneScoped
		return std::static_pointer_cast<StaticMeshHandle>(LoadAsset(FilePath, AssetType::StaticMesh));
	}

	SharedPtr<IAssetHandle> AssetManager::LoadAsset(const String& FilePath, AssetType Type)
	{
		ZoneScoped
		// Check that we are loading a file which exists
		check(std::filesystem::exists(FilePath));

		// Check if the item is already in the cache and it is not invalid
		if (auto It = CachedAssets.find(FilePath); It != CachedAssets.end())
		{
			if (!It->second.expired())
			{
				return It->second.lock();
			}
			else
			{
				// Item is expired, it should be removed from the cache
				CachedAssets.erase(It);
			}
		}

		SharedPtr<IAssetHandle> Ret(nullptr);

		switch (Type)
		{
		case AssetType::Texture:
			Ret = std::make_shared<TextureHandle>(FilePath);
			break;
		case AssetType::Font:
		case AssetType::StaticMesh:
			Ret = std::make_shared<StaticMeshHandle>(FilePath);
			break;
		default:
			// Unsupported asset type
			check(false);
			break;
		}
		
		if (Ret != nullptr)
		{
			CachedAssets.emplace(FilePath, Ret);
		}
		return Ret;
	}

	void AssetManager::ForEachLoadedAsset(const std::function<bool(IAssetHandle&)>& Func)
	{
		ZoneScoped
		for (const auto& Pair : CachedAssets)
		{
			if (!Func(*(Pair.second.lock())))
			{
				break;
			}
		}
	}

	void AssetManager::UnloadAsset(const String& AssetToUnload)
	{
		ZoneScoped
		CachedAssets.erase(AssetToUnload);
	}
	
}