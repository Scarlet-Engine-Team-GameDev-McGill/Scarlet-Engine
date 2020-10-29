#include "AssetViewItem.h"
#include "AssetHandle.h"
#include "UIWidget.h"
#include <imgui_internal.h>

#include "RALResources.h"

namespace ScarletEngine
{
	IAssetViewItem::IAssetViewItem(const String& InName, AssetView* InAssetView)
		: Name(InName)
		, AssetHandle()
		, AssetViewPtr(InAssetView)
	{}

	void IAssetViewItem::Draw()
	{
		const float CellWidth = ImGui::TableGetCellBgRect().GetWidth();
		const auto CursorPos = ImGui::GetCursorPos();
		ImGui::Selectable("", &bSelected, 0, { CellWidth, CellWidth });
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			// #todo_AssetView: double click should open an asset editor or preview for that asset type 
			OnDoubleClick();
		}

		ImGui::SetCursorPos(CursorPos);

		const RALTexture2D* AssetIcon = GetAssetIconImage();
		if (AssetIcon != nullptr)
		{
			ImGui::Image(reinterpret_cast<void*>(AssetIcon->GetTextureResource()), { CellWidth, CellWidth });
		}

		// Center the text label
		const float CellTextPadding = CellWidth / 8.f;
		const float SingleLineTextWidth = CellWidth - 2.f * CellTextPadding;
		const float TextWidth = ImGui::CalcTextSize(Name.c_str()).x;

		if (TextWidth > SingleLineTextWidth)
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + CellTextPadding);
			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + SingleLineTextWidth);
			ImGui::TextWrapped("%s", Name.c_str());
			ImGui::PopTextWrapPos();
		}
		else
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 0.5f * CellWidth - 0.5f * TextWidth);
			ImGui::Text("%s", Name.c_str());
		}
	}
}