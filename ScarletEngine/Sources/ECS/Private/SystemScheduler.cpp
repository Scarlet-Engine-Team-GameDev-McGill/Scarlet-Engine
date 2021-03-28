#include "SystemScheduler.h"

namespace ScarletEngine
{
    void SystemScheduler::RunUpdate() const
    {
        for (const UniquePtr<ISystem>& System : Systems)
        {
            System->Update();
        }
    }

    void SystemScheduler::RunFixedUpdate() const
    {
        for (const UniquePtr<ISystem>& System : Systems)
        {
            System->FixedUpdate();
        }
    }
}
