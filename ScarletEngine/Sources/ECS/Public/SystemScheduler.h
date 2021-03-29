#pragma once

#include "Core.h"
#include "System.h"

namespace ScarletEngine
{
    class Registry;
    
    template <typename Type>
    concept ECSSystem = std::derived_from<Type, ISystem>;
    
    class SystemScheduler
    {
    public:        
        template <ECSSystem SystemType>
        void RegisterSystem()
        {
            Systems.push_back(UniquePtr<ISystem>(ScarNew(SystemType)));
        }

        void RunUpdate(Registry* Reg) const;

        void RunFixedUpdate(Registry* Reg) const;
    private:
        Array<UniquePtr<ISystem>> Systems;
    };
}
