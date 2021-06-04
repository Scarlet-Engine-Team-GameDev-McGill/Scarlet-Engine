 #pragma once

#include "Core.h"
#include "System.h"

namespace ScarletEngine
{
    class Registry;
    
    class SystemScheduler
    {
    public:
        /** Register a new system */
        template <typename SystemType>
        void RegisterSystem()
        {
            SharedPtr<ISystem> System(ScarNew(SystemType));
            check(System);

            SCAR_LOG(LogInfo, "Activating systems: %s", System->Name.c_str());

            if (System->IsGameplayOnly())
            {
                GameplayOnlySystems.emplace_back(System);
                if (bRunningGameplaySystems)
                {
                    ActiveSystems.emplace_back(System);
                }
            }
            else
            {
                ActiveSystems.emplace_back(System);
            }
        }

        /** Run the update step on all systems for the entities in the passed registry */
        void RunUpdate(Registry* Reg) const;

        /** Run the fixed update step on all systems for the entities in the passed registry */
        void RunFixedUpdate(Registry* Reg) const;

        void EnableGameplaySystems();
        void DisableGameplaySystems();

        /** Get the scheduler singleton */
        static SystemScheduler& Get() { static SystemScheduler Instance; return Instance; }
    private:
        Array<SharedPtr<ISystem>> ActiveSystems;
        Array<SharedPtr<ISystem>> GameplayOnlySystems;

        bool bRunningGameplaySystems = false;
    };
}
