#pragma once

#include "CoreMinimal.h"
#include "UIWidget.h"

namespace ScarletEngine
{
	class RALTexture2D;
	class AssetView;

	class IAssetViewItem
	{
	public:
		IAssetViewItem(const String& InName, AssetView* InAssetView);
		virtual ~IAssetViewItem() {}

		void Draw();
		const String& GetName() const { return Name; }
	protected:
		virtual RALTexture2D* GetAssetIconImage() = 0;
		virtual void OnDoubleClick() {}
	protected:
		String Name;
		WeakPtr<class IAssetHandle> AssetHandle;
		AssetView* AssetViewPtr;

		bool bSelected = false;
	};
}