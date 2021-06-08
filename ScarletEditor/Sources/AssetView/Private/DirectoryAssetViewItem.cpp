#include "DirectoryAssetViewItem.h"
#include "RAL.h"
#include "AssetManager.h"
#include "AssetView.h"

namespace ScarletEngine
{
	RALTexture2D* DirectoryAssetViewItem::DirectoryIcon = nullptr;

	DirectoryAssetViewItem::DirectoryAssetViewItem(const String& InName, AssetView* InAssetView)
		: IAssetViewItem(InName, InAssetView)
	{
		if (DirectoryIcon == nullptr)
		{
			DirectoryIcon = RAL::Get().CreateTexture2D(AssetManager::LoadTextureFile("/ScarletEngine/Content/Folder.png"));
		}
	}

	RALTexture2D* DirectoryAssetViewItem::GetAssetIconImage() const
	{
		return DirectoryIcon;
	}

	void DirectoryAssetViewItem::OnDoubleClick()
	{
		const String NewDirectory = StringUtils::PathConcat(AssetViewPtr->GetCurrentDirectory(), GetName());
		AssetViewPtr->SetCurrentDirectory(NewDirectory);
	}
}