 #pragma once

#include "Core.h"
#include "System.h"

namespace ScarletEngine
{
    class Registry;
    
    class SystemScheduler
    {
    public:
        /** Register a new system of the templated type */
        template <typename SystemType>
        void RegisterSystem()
        {
            Systems.emplace_back(ScarNew(SystemType));
        }

        /** Run the update step on all systems for the entities in the passed registry */
        void RunUpdate(Registry* Reg) const;

        /** Run the fixed update step on all systems for the entities in the passed registry */
        void RunFixedUpdate(Registry* Reg) const;

        /** Get the scheduler singleton */
        static SystemScheduler& Get() { static SystemScheduler Instance; return Instance; }
    private:
        Array<UniquePtr<ISystem>> Systems;
    };
}
