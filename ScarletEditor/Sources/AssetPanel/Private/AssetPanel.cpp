#include "AssetPanel.h"
#include "AssetPanelViewItem.h"
#include "AssetManager.h"

#include "DirectoryAssetViewItem.h"
#include "FileAssetViewItem.h"

namespace ScarletEngine
{
	AssetPanel::AssetPanel()
		: UIWindow("Assets")
		, CurrentDirectory("/ScarletEngine/Content")
		, ViewItems()
	{}

	void AssetPanel::Construct()
	{
		bNeedsRefresh = true;
	}

	void AssetPanel::Refresh()
	{
		ViewItems.clear();

		for (const auto& File : std::filesystem::directory_iterator(AssetManager::ToFullPath(CurrentDirectory)))
		{
			std::filesystem::path Path = File.path();
			if (File.is_regular_file())
			{
				// #todo_AssetView: Support other file types
				ViewItems.push_back(MakeShared<FileAssetViewItem>(Path.filename().string().c_str(), this));
			}
			else if (File.is_directory())
			{
				ViewItems.push_back(MakeShared<DirectoryAssetViewItem>(Path.filename().string().c_str(), this));
			}
		}

		bNeedsRefresh = false;
	}

	void AssetPanel::SetCurrentDirectory(const String& NewCurrentDirectory)
	{
		CurrentDirectory = NewCurrentDirectory;
		bNeedsRefresh = true;
	}

	void AssetPanel::DrawWindowContent()
	{
		if (ImGui::Button(ICON_MD_KEYBOARD_BACKSPACE))
		{
			if (CurrentDirectory.size() > 1)
			{
				const size_t Index = CurrentDirectory.find_last_of('/');
				if (Index != 0)
				{
					CurrentDirectory = CurrentDirectory.substr(0, Index);
				}
				else
				{
					CurrentDirectory = "/";
				}
				bNeedsRefresh = true;
			}
		}

		if (bNeedsRefresh)
		{
			Refresh();
		}

		ImGui::SameLine();
		ImGui::Text("%s", CurrentDirectory.c_str());

		const float AvailWidth = ImGui::GetContentRegionAvailWidth();
		const float CellSize = 128.f * ImGui::GetWindowDpiScale();
		// ImGui requires the number of columns in a table to be in [1, 64]
		const uint32_t NumColumns = std::min(std::max((uint32_t)(AvailWidth / CellSize), (uint32_t)1), (uint32_t)64);

		uint32_t TableFlags = ImGuiTableColumnFlags_WidthFixed;
		if (ImGui::BeginChild("##TableView"))
		{
			if (ImGui::BeginTable("##AssetViewContent", NumColumns, TableFlags))
			{
				uint32_t Index = 0;
				for (uint32_t i = 0; i < NumColumns; ++i)
				{
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, CellSize);
				}
				for (const auto& ViewItem : ViewItems)
				{
					ImGui::PushID(ViewItem.get());

					if (Index++ % NumColumns == 0)
					{
						ImGui::TableNextRow(ImGuiTableRowFlags_None, CellSize);
					}
					ImGui::TableNextColumn();
					ViewItem->Draw();

					ImGui::PopID();
				}
				ImGui::EndTable();
			}
		}
		ImGui::EndChild();
		
	}
}