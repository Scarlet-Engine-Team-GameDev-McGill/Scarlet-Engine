#pragma once
#include "AssetViewItem.h"

namespace ScarletEngine
{
	// #todo_AssetView: Create individual items for each item type
	class FileAssetViewItem : public IAssetViewItem
	{
	public:
		FileAssetViewItem(const String& InName, AssetView* InAssetView);

	private:
		virtual RALTexture2D* GetAssetIconImage() const override;

		static RALTexture2D* FileIcon;
	};
}