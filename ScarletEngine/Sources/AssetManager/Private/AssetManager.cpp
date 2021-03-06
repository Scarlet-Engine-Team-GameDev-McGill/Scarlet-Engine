#include "AssetManager.h"
#include "Utils/StringUtils.h"

namespace ScarletEngine
{
	UnorderedMap<String, WeakPtr<IAssetHandle>> AssetManager::CachedAssets;
	String AssetManager::AssetRoot;

	void AssetManager::SetAssetRoot(const String& InAssetRoot)
	{
		AssetRoot = InAssetRoot;
	}

	SharedPtr<TextureHandle> AssetManager::LoadTextureFile(const String& FilePath)
	{
		ZoneScoped
		return std::static_pointer_cast<TextureHandle>(LoadAsset_Impl(FilePath, AssetType::Texture));
	}

	SharedPtr<StaticMeshHandle> AssetManager::LoadStaticMesh(const String& FilePath)
	{
		ZoneScoped
		return std::static_pointer_cast<StaticMeshHandle>(LoadAsset_Impl(FilePath, AssetType::StaticMesh));
	}

	SharedPtr<IAssetHandle> AssetManager::LoadAsset_Impl(const String& FilePath, AssetType Type)
	{
		ZoneScoped
		const String FullPath = ToFullPath(FilePath);

		// Check that we are loading a file which exists
		check(std::filesystem::exists(FullPath));

		// Check if the item is already in the cache and it is not invalid
		if (const auto It = CachedAssets.find(FullPath); It != CachedAssets.end())
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

		// Immediately Invoked Lambda Expression
		SharedPtr<IAssetHandle> AssetHandle = [&]() -> SharedPtr<IAssetHandle>
		{
			switch (Type)
			{
			case AssetType::Texture:
				return MakeShared<TextureHandle>(FilePath);
			case AssetType::StaticMesh:
				return MakeShared<StaticMeshHandle>(FilePath);
			default:
				// Unsupported asset type
				check(false);
				return SharedPtr<IAssetHandle>(nullptr);
			}
		}();
		
		if (AssetHandle != nullptr)
		{
			CachedAssets.emplace(FilePath, AssetHandle);
		}

		return AssetHandle;
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

	void AssetManager::UnloadAsset(const String& AssetToUnload)
	{
		CachedAssets.erase(AssetToUnload);
	}

	String AssetManager::ToFullPath(const String& AssetPath)
	{
		return StringUtils::PathConcat(AssetRoot, AssetPath);
	}
	
}