#include "SystemScheduler.h"

namespace ScarletEngine
{
    void SystemScheduler::RunUpdate(Registry* Reg) const
    {
        for (const SharedPtr<ISystem>& System : ActiveSystems)
        {
            System->Reg = Reg;
            System->Update();
            System->Reg = nullptr;
        }
    }

    void SystemScheduler::RunFixedUpdate(Registry* Reg) const
    {
        for (const SharedPtr<ISystem>& System : ActiveSystems)
        {
            System->Reg = Reg;
            System->FixedUpdate();
            System->Reg = nullptr;
        }
    }

    void SystemScheduler::EnableGameplaySystems()
    {
        bRunningGameplaySystems = true;
        ActiveSystems.insert(ActiveSystems.end(), GameplayOnlySystems.begin(), GameplayOnlySystems.end());
    }

    void SystemScheduler::DisableGameplaySystems()
    {
        bRunningGameplaySystems = false;
        ActiveSystems.erase(std::remove_if(ActiveSystems.begin(), ActiveSystems.end(), [](const SharedPtr<ISystem>& System)
        {
            return System->IsGameplayOnly();
        }),
        ActiveSystems.end());
    }
}
