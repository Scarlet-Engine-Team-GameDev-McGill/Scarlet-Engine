#pragma once
#include "AssetPanelViewItem.h"

//#todo_AssetView: REMOVE THIS CLASS

namespace ScarletEngine
{
	struct FileAssetViewItem : public IAssetPanelViewItem
	{
	public:
		FileAssetViewItem(const String& InName);

	private:
		virtual RALTexture2D* GetAssetIconImage() override;

		static RALTexture2D* FileIcon;
	};
}