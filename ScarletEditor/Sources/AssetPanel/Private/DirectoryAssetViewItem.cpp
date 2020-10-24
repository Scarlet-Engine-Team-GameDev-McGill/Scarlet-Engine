#include "DirectoryAssetViewItem.h"
#include "RAL.h"
#include "AssetManager.h"
#include "AssetPanel.h"

namespace ScarletEngine
{
	RALTexture2D* DirectoryAssetViewItem::DirectoryIcon = nullptr;

	DirectoryAssetViewItem::DirectoryAssetViewItem(const String& InName, AssetPanel* InAssetView)
		: IAssetPanelViewItem(InName, InAssetView)
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

	void DirectoryAssetViewItem::OnDoubleClick()
	{
		String NewDirectory = AssetView->GetCurrentDirectory();
		StringUtils::PathConcat(NewDirectory, GetName());
		AssetView->SetCurrentDirectory(NewDirectory);
	}
}