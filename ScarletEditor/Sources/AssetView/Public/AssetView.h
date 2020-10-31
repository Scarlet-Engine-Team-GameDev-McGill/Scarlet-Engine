#pragma once
#include "Core.h"
#include "UIWindow.h"

namespace ScarletEngine
{
	using AssetViewItemPtr = SharedPtr<class IAssetViewItem>;

	class AssetView : public UIWindow
	{
	public:
		AssetView();

		/** Generates a list of available assets on construction. */
		virtual void Construct() override;
		virtual void DrawWindowContent() override;

		/** Refreshes view */
		void Refresh();
		
		/** Get the current directory */
		const String& GetCurrentDirectory() const { return CurrentDirectory; }
		/** Sets the current directory */
		void SetCurrentDirectory(const String& NewCurrentDirectory);
	private:
		String CurrentDirectory;
		Array<AssetViewItemPtr> ViewItems;

		bool bNeedsRefresh;
	};
}