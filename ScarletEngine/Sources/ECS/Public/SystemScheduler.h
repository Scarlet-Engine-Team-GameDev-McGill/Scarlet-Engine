#pragma once

#include "Core.h"
#include "System.h"

namespace ScarletEngine
{
    class Registry;
    
    class SystemScheduler
    {
    public:        
        template <typename SystemType>
        void RegisterSystem()
        {
            Systems.emplace_back(ScarNew(SystemType));
        }

        void RunUpdate(Registry* Reg) const;

        void RunFixedUpdate(Registry* Reg) const;
        
        static SystemScheduler& Get() { static SystemScheduler Instance; return Instance; }
    private:
        Array<UniquePtr<ISystem>> Systems;
    };
}
