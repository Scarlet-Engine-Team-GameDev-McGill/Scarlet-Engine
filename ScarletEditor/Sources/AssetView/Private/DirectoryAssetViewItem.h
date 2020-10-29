#pragma once

#include "AssetViewItem.h"

namespace ScarletEngine
{
	class RALTexture2D;

	class DirectoryAssetViewItem : public IAssetViewItem
	{
	public:
		DirectoryAssetViewItem(const String& InName, AssetView* InAssetView);

	private:
		virtual RALTexture2D* GetAssetIconImage() override;
		virtual void OnDoubleClick() override;
	private:
		static RALTexture2D* DirectoryIcon;
	};
}