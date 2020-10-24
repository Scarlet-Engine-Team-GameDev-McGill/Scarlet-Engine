#pragma once

#include "AssetPanelViewItem.h"

namespace ScarletEngine
{
	class RALTexture2D;

	class DirectoryAssetViewItem : public IAssetPanelViewItem
	{
	public:
		DirectoryAssetViewItem(const String& InName, AssetPanel* InAssetView);

	private:
		virtual RALTexture2D* GetAssetIconImage() override;
		virtual void OnDoubleClick() override;
	private:
		static RALTexture2D* DirectoryIcon;
	};
}