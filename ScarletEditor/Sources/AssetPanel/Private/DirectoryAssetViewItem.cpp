#include "DirectoryAssetViewItem.h"
#include "RAL.h"
#include "AssetManager.h"

namespace ScarletEngine
{
	RALTexture2D* DirectoryAssetViewItem::DirectoryIcon = nullptr;

	DirectoryAssetViewItem::DirectoryAssetViewItem(const String& InName)
		: IAssetPanelViewItem(InName)
	{
		if (DirectoryIcon == nullptr)
		{
			DirectoryIcon = RAL::Get().CreateTexture2D(AssetManager::LoadTextureFile("/ScarletEngine/Content/Folder.png"));
		}
	}

	RALTexture2D* DirectoryAssetViewItem::GetAssetIconImage()
	{
		return DirectoryIcon;
	}
}