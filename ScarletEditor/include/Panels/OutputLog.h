#pragma once

#include "Core/Core.h"
#include "UI/UIWidget.h"

namespace ScarletEngine
{
	struct Message
	{
		String Text;
		LogLevel Severity;
	};

	class OutputLogPanel : public UIWidget
	{
	public:
		OutputLogPanel();

		virtual void Initialize() override;
		virtual void Draw() override;
		virtual void Destroy() override;
	private:
		bool PassesFilter(const String& Msg) const;

		void OnMessageLogged(LogLevel Level, const char* Msg);

		void Clear();
	private:
		String FilterText;
		Array<Message> MessageBuffer;

		const uint32_t MaxBufferLength = 1024;
		uint32_t CurrentMessageIndex;
		uint32_t NumMessages;

		bool bHasNewMessage = false;
		bool bAutoscroll = true;

		bool bShowVerbose = false;
		bool bShowInfo = true;
		bool bShowWarning = true;
		bool bShowError = true;
	};
}