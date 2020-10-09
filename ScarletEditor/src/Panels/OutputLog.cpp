#include "Panels/OutputLog.h"

namespace ScarletEngine
{
	OutputLogPanel::OutputLogPanel()
		: UIWidget("Output Log")
		, FilterText()
		, CurrentMessageIndex(0)
		, NumMessages(0)
	{
	}

	void OutputLogPanel::Construct()
	{
		ZoneScoped
		MessageBuffer.resize(MaxBufferLength);
		FilterText.reserve(32);

		Logger::Get().GetOnMessageLogged().Bind(this, &OutputLogPanel::OnMessageLogged);
	}

	void OutputLogPanel::Destroy()
	{
		ZoneScoped
		Logger::Get().GetOnMessageLogged().Unbind(this);
	}

	void OutputLogPanel::Draw()
	{
		ZoneScoped
		ImGui::TextUnformatted(ICON_MD_FILTER_LIST " Filter:");
		ImGui::SameLine();
		ImGui::InputText("###Filter", (char*)FilterText.c_str(), FilterText.capacity(), ImGuiInputTextFlags_EnterReturnsTrue);
			
		ImGui::SameLine();

		if (ImGui::Button("Clear"))
		{
			Clear();
		}
		ImGui::SameLine();

		if (ImGui::Button(ICON_MD_SETTINGS))
		{
			ImGui::OpenPopup("###OutputSetting");
		}

		if (ImGui::BeginPopup("###OutputSetting"))
		{
			ImGui::MenuItem("Show Verbose", "", &bShowVerbose);
			ImGui::MenuItem("Show Info", "", &bShowInfo);
			ImGui::MenuItem("Show Warning", "", &bShowWarning);
			ImGui::MenuItem("Show Error", "", &bShowError);
			ImGui::Separator();
			ImGui::MenuItem("Autoscroll", "", &bAutoscroll);
			ImGui::EndPopup();
		}

		if (ImGui::BeginChild("Log"))
		{
			uint32_t i = (CurrentMessageIndex) % ((NumMessages < MaxBufferLength) && (NumMessages != 0) ? NumMessages : MaxBufferLength);
			uint32_t UpdateCount = 0;
			while (UpdateCount < NumMessages)
			{
				Message& Msg = MessageBuffer[i];
				if (((Msg.Severity == LogLevel::LogVerbose && bShowVerbose) || 
					(Msg.Severity == LogLevel::LogInfo && bShowInfo) ||
					(Msg.Severity == LogLevel::LogWarning && bShowWarning) ||
					(Msg.Severity == LogLevel::LogError && bShowError)) &&
					PassesFilter(Msg.Text))
				{
					switch (Msg.Severity)
					{
					case LogLevel::LogInfo:
						ImGui::TextUnformatted(Msg.Text.c_str());
						break;
					case LogLevel::LogVerbose:
						ImGui::TextDisabled("%s", Msg.Text.c_str());
						break;
					case LogLevel::LogWarning:
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),"%s", Msg.Text.c_str());
						break;
					case LogLevel::LogError:
						ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "%s", Msg.Text.c_str());
						break;
					}
				}

				i = (i + 1) % MaxBufferLength;
				UpdateCount++;
			}

			// If there was a new message, scroll to the end of the child window
			if (bHasNewMessage && bAutoscroll)
			{
				ImGui::SetScrollHereY(1.0f);
				bHasNewMessage = false;
			}
		}
			
		ImGui::EndChild();
	}

	bool OutputLogPanel::PassesFilter(const String& Msg) const
	{
		ZoneScoped
		return Msg.find(FilterText.c_str()) != std::string::npos;
	}

	void OutputLogPanel::OnMessageLogged(LogLevel Level, const char* Msg)
	{
		ZoneScoped
		MessageBuffer[CurrentMessageIndex] = { Msg, Level };
		CurrentMessageIndex = (CurrentMessageIndex + 1) % MaxBufferLength;
		if (NumMessages != MaxBufferLength)
		{
			++NumMessages;
		}
		bHasNewMessage = true;
	}

	void OutputLogPanel::Clear()
	{
		ZoneScoped
		CurrentMessageIndex = 0;
		NumMessages = 0;
	}
}