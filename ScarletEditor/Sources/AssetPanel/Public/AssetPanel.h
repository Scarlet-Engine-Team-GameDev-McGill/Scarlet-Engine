#pragma once
#include "Core.h"
#include "UIWindow.h"

namespace ScarletEngine
{
	using AssetPanelViewItemPtr = SharedPtr<struct IAssetPanelViewItem>;

	class AssetPanel : public UIWindow
	{
	public:
		AssetPanel();

		/** Generates a list of available assets on construction. */
		virtual void Construct() override;
		virtual void DrawWindowContent() override;

		/** Refreshes view */
		void Refresh();
	private:
		String CurrentDirectory;
		Array<AssetPanelViewItemPtr> ViewItems;
	};
}