#pragma once

#include "CoreMinimal.h"
#include "UIWidget.h"

namespace ScarletEngine
{
	class RALTexture2D;

	struct IAssetPanelViewItem
	{
	public:
		virtual ~IAssetPanelViewItem() {}

		IAssetPanelViewItem(const String& InName);

		void Draw();

		String Name;
		WeakPtr<class IAssetHandle> AssetHandle;
	private:
		virtual RALTexture2D* GetAssetIconImage() = 0;
		bool bSelected = false;
	};
}