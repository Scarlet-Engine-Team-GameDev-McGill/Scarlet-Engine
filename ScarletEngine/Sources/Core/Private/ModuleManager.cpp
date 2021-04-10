#include "ModuleManager.h"

#include "IModule.h"

namespace ScarletEngine
{
    void ModuleManager::ResolveModuleOrder()
    {
        Array<SharedPtr<IModule>> InitOrder;
        InitOrder.reserve(Modules.size());

        auto FindModuleByString = [](StringView String, const Array<SharedPtr<IModule>>& Modules)
        {
            const auto It = std::ranges::find_if(Modules, [String](const SharedPtr<IModule>& PotentialDep)
            {
                return String == StringView(PotentialDep->GetModuleName());
            });
            return It != Modules.end() ? std::optional<SharedPtr<IModule>>(*It) : std::optional<SharedPtr<IModule>> {};
        };

        Function<void(const SharedPtr<IModule>&)> AddModuleToInitOrder = [this, &InitOrder, &AddModuleToInitOrder, &
                FindModuleByString](const SharedPtr<IModule>& ModuleToAdd)
        {
            const bool bModuleToAddExists = std::ranges::find(InitOrder, ModuleToAdd) != InitOrder.end();
            if (bModuleToAddExists) return;
            
            // Find this modules dependencies by name
            for (const char* const DependencyName : ModuleToAdd->GetDependencies())
            {
                if (const std::optional<SharedPtr<IModule>> Dependency = FindModuleByString(DependencyName, Modules))
                {
                    // Ensure the dep module doesn't already exist in the map
                    const bool bDependencyExists = FindModuleByString(DependencyName, InitOrder).has_value();
                    if (!bDependencyExists) AddModuleToInitOrder(Dependency.value());
                }
            }

            InitOrder.emplace_back(ModuleToAdd);
        };

        for (SharedPtr<IModule>& Module : Modules)
        {
            AddModuleToInitOrder(Module);
        }

        Modules = std::move(InitOrder);
    }
    
    void ModuleManager::Startup()
    {
        if (Modules.size() == 0)
        {
            SCAR_LOG(LogWarning, "No modules detected. This is probably an error!");
            return;
        }

        ResolveModuleOrder();

        for (const SharedPtr<IModule>& Module : Modules)
        {
            Module->Startup();

            SCAR_LOG(LogVerbose, "Initialized module: %s", Module->GetModuleName());
        }

        bStarted = true;
    }

    void ModuleManager::Shutdown()
    {
        for (const auto& Module : std::ranges::reverse_view(Modules))
        {
            Module->Shutdown();

            SCAR_LOG(LogVerbose, "Shutdown module: %s", Module->GetModuleName());
        }

        Modules.clear();
    }

    void ModuleManager::PreUpdate()
    {
        check(bStarted);

        for (const auto& Module : Modules)
        {
            Module->PreUpdate();
        }
    }

    void ModuleManager::Update()
    {
        check(bStarted);

        for (const auto& Module : Modules)
        {
            Module->Update();
        }
    }

    void ModuleManager::PostUpdate()
    {
        check(bStarted);

        for (const auto& Module : std::ranges::reverse_view(Modules))
        {
            Module->PostUpdate();
        }
    }
}
