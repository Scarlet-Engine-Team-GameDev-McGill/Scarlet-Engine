#include "AssetPanelViewItem.h"
#include "AssetHandle.h"
#include "UIWidget.h"
#include <imgui_internal.h>

#include "RALResources.h"

namespace ScarletEngine
{
	IAssetPanelViewItem::IAssetPanelViewItem(const String& InName)
		: Name(InName)
		, AssetHandle()
	{}

	void IAssetPanelViewItem::Draw()
	{
		const float CellWidth = ImGui::TableGetCellBgRect().GetWidth();
		const auto CursorPos = ImGui::GetCursorPos();
		ImGui::Selectable("", &bSelected, 0, { CellWidth, CellWidth });
		ImGui::SetCursorPos(CursorPos);

		RALTexture2D* AssetIcon = GetAssetIconImage();
		if (AssetIcon != nullptr)
		{
			ImGui::Image(reinterpret_cast<void*>(AssetIcon->GetTextureResource()), { 128.f, 128.f });
		}
		
		// Center the text label
		const float CellTextPadding = CellWidth / 8.f;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + CellTextPadding);
		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + CellWidth - 2.f * CellTextPadding);
		ImGui::TextWrapped(Name.c_str());
		ImGui::PopTextWrapPos();
	}
}