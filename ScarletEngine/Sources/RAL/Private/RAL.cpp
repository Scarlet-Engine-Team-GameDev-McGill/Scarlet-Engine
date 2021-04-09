#include "RAL.h"

namespace ScarletEngine
{
	UniquePtr<RAL> RAL::Instance = nullptr;
	RenderAPI RAL::API = RenderAPI::Invalid;

	void RAL::Initialize()
	{
		CommandListQueue.push(RALCommandList{});
	}

	void RAL::QueueCommand(const Function<void(RALCommandList&)>& Cmd, const char* CommandLabel)
	{
		CommandListQueue.back().QueueRenderCommand(Cmd, CommandLabel);
	}
}
