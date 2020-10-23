#pragma once

#include "AssetPanelViewItem.h"

namespace ScarletEngine
{
	class RALTexture2D;

	struct DirectoryAssetViewItem : public IAssetPanelViewItem
	{
	public:
		DirectoryAssetViewItem(const String& InName);

	private:
		virtual RALTexture2D* GetAssetIconImage() override;

		static RALTexture2D* DirectoryIcon;
	};
}