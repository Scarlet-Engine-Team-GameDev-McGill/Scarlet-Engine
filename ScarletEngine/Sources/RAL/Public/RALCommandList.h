#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
    class RALCommandList
    {
    public:
        void QueueRenderCommand(const Function<void(RALCommandList&)>& Func)
        {
            Commands.emplace_back(Func);
        }

        void ExecuteAll()
        {
            for (const auto& Command : Commands)
            {
                Command(*this);
            }
        }

    private:
        Array<Function<void(RALCommandList&)>> Commands;
    };
}
