#pragma once

#include "Core.h"
#include "UIWindow.h"

namespace ScarletEngine
{
	struct Message
	{
		String Text;
		LogLevel Severity;
	};

	class OutputLogPanel : public UIWindow
	{
	public:
		OutputLogPanel();

		virtual void Construct() override;
		virtual void DrawWindowContent() override;
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