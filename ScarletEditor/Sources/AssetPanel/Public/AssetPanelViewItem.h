#pragma once

#include "CoreMinimal.h"
#include "UIWidget.h"

namespace ScarletEngine
{
	class RALTexture2D;
	class AssetPanel;

	class IAssetPanelViewItem
	{
	public:
		IAssetPanelViewItem(const String& InName, AssetPanel* InAssetView);
		virtual ~IAssetPanelViewItem() {}

		void Draw();
		const String& GetName() const { return Name; }
	protected:
		virtual RALTexture2D* GetAssetIconImage() = 0;
		virtual void OnDoubleClick() {}
	protected:
		String Name;
		WeakPtr<class IAssetHandle> AssetHandle;
		AssetPanel* AssetView;

		bool bSelected = false;
	};
}