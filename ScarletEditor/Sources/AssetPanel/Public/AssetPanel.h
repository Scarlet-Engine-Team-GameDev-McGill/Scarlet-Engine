#pragma once
#include "Core.h"
#include "UIWindow.h"

namespace ScarletEngine
{
	using AssetPanelViewItemPtr = SharedPtr<class IAssetPanelViewItem>;

	class AssetPanel : public UIWindow
	{
	public:
		AssetPanel();

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
		Array<AssetPanelViewItemPtr> ViewItems;

		bool bNeedsRefresh;
	};
}