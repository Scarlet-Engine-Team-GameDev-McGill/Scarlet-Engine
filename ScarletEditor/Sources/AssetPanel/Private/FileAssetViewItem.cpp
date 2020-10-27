#include "FileAssetViewItem.h"
#include "RAL.h"
#include "AssetManager.h"

namespace ScarletEngine
{
	RALTexture2D* FileAssetViewItem::FileIcon = nullptr;

	FileAssetViewItem::FileAssetViewItem(const String& InName, AssetPanel* InAssetView)
		: IAssetPanelViewItem(InName, InAssetView)
	{
		if (FileIcon == nullptr)
		{
			FileIcon = RAL::Get().CreateTexture2D(AssetManager::LoadTextureFile("/ScarletEngine/Content/GenericAsset.tga"));
		}
	}

	RALTexture2D* FileAssetViewItem::GetAssetIconImage()
	{
		return FileIcon;
	}
}