#include "RAL.h"

namespace ScarletEngine
{
	UniquePtr<RAL> RAL::Instance = nullptr;
	RenderAPI RAL::API = RenderAPI::Invalid;

	void RAL::Initialize()
	{
		CommandListQueue.push(UniquePtr<RALCommandList>(CreateCommandList()));
	}

	void RAL::Submit()
	{
		CommandListQueue.front()->ExecuteAll();
		CommandListQueue.pop();

		CommandListQueue.push(UniquePtr<RALCommandList>(CreateCommandList()));
	}

	void RAL::QueueCommand(const Function<void(RALCommandList&)>& Cmd, const char* CommandLabel)
	{
		CommandListQueue.back()->QueueRenderCommand(Cmd, CommandLabel);
	}
}
