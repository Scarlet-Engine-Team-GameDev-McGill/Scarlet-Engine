#pragma once

#include "CoreMinimal.h"

#define RAL_USING_COMMAND_LISTS 1

namespace ScarletEngine
{
    class RALCommandList
    {
    public:
        void QueueRenderCommand(const Function<void(RALCommandList&)>& Cmd, const char* CommandLabel)
        {
#if RAL_USING_COMMAND_LISTS
            Commands.emplace_back(std::make_pair(Cmd, CommandLabel));
#else
            Cmd(*this);
#endif
        }

        void ExecuteAll()
        {
            for (const auto& [Command, _] : Commands)
            {
                Command(*this);
            }
        }

    private:
        Array<std::pair<Function<void(RALCommandList&)>, const char*>> Commands;
    };
}
