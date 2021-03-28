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
        SystemScheduler(Registry* InReg) : Reg(InReg) {}
        
        template <ECSSystem SystemType>
        void RegisterSystem()
        {
            Systems.push_back(UniquePtr<ISystem>(ScarNew(SystemType, Reg)));
        }

        void RunUpdate() const;

        void RunFixedUpdate() const;

    private:
        Array<UniquePtr<ISystem>> Systems;
        Registry* Reg;
    };
}
