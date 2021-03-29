#include "SystemScheduler.h"

namespace ScarletEngine
{
    void SystemScheduler::RunUpdate(Registry* Reg) const
    {
        for (const UniquePtr<ISystem>& System : Systems)
        {
            System->Reg = Reg;
            System->Update();
            System->Reg = nullptr;
        }
    }

    void SystemScheduler::RunFixedUpdate(Registry* Reg) const
    {
        for (const UniquePtr<ISystem>& System : Systems)
        {
            System->Reg = Reg;
            System->FixedUpdate();
            System->Reg = nullptr;
        }
    }
}
