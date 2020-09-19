#include "AssetManager/AssetManager.h"

#include <filesystem>

namespace ScarletEngine
{
	std::unordered_map<std::string, std::weak_ptr<IAssetHandle>> AssetManager::CachedAssets;

	std::shared_ptr<TextureHandle> AssetManager::LoadTexture(const std::string& FilePath)
	{
		return std::static_pointer_cast<TextureHandle>(LoadAsset(FilePath, AssetType::Texture));
	}

	std::shared_ptr<IAssetHandle> AssetManager::LoadAsset(const std::string& FilePath, AssetType Type)
	{
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

		switch (Type)
		{
		case AssetType::Texture:
		{
			std::shared_ptr<IAssetHandle> Ret = std::make_shared<TextureHandle>(FilePath);
			CachedAssets.emplace(FilePath, Ret);
			return Ret;
		}
		case AssetType::Font:
		case AssetType::StaticMesh:
		default:
			// Unsupported asset type
			check(false)
				break;
		}

		return nullptr;
	}

	void AssetManager::ForEachLoadedAsset(const std::function<bool(IAssetHandle&)>& Func)
	{
		for (const auto& Pair : CachedAssets)
		{
			if (!Func(*(Pair.second.lock())))
			{
				break;
			}
		}
	}

	void AssetManager::UnloadAsset(const std::string& AssetToUnload)
	{
		CachedAssets.erase(AssetToUnload);
	}
	
}