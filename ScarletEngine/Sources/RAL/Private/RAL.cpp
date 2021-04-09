#include "RAL.h"

namespace ScarletEngine
{
	UniquePtr<RAL> RAL::Instance = nullptr;
	RenderAPI RAL::API = RenderAPI::Invalid;

	void RAL::Initialize()
	{
		CommandListQueue.push(RALCommandList{});
		SetClearColorCmd({ 0.1f, 0.1f, 0.1f, 1.f });
	}

	void RAL::QueueCommand(const Function<void(RALCommandList&)>& Cmd)
	{
		CommandListQueue.back().QueueRenderCommand(Cmd);
	}
}
