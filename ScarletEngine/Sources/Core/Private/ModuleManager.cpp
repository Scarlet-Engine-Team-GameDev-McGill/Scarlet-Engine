#include "ModuleManager.h"
#include "IModule.h"

namespace ScarletEngine
{
	void ModuleManager::Startup()
	{
		FillDependencyMap();

		if (ModulesToAdd.size() == 0)
		{
			SCAR_LOG(LogWarning, "No modules detected. This is probably an error!");
			return;
		}

		for (const SharedPtr<IModule>& Module : ModulesToAdd)
		{
			LoadModule_Impl(Module);
		}

#ifdef DEBUG
		SCAR_LOG(LogVerbose, "Module dependency graph: \ndigraph ModuleDependencies {\n%s\n}", GetDependencyGraphViz().c_str());
#endif
		ModulesToAdd.clear();
		bStarted = true;
	}
	
	void ModuleManager::FillDependencyMap()
	{
		for (const SharedPtr<IModule>& Module : ModulesToAdd)
		{
			ModuleDepMap[Module.get()] = Set<SharedPtr<IModule>>();
			for (const auto& DependencyName : Module->GetDependencies())
			{
				String DependencyNameString(DependencyName);
				auto It = std::find_if(ModulesToAdd.begin(), ModulesToAdd.end(), [&DependencyNameString](const SharedPtr<IModule>& PotentialDep)
					{
						return DependencyNameString == PotentialDep->GetModuleName();
					});

				if (It != ModulesToAdd.end())
				{
					ModuleDepMap[Module.get()].insert(*It);
				}
			}
		}
	}

	void ModuleManager::LoadModule_Impl(const SharedPtr<IModule>& Module)
	{
		if (Module->IsInitialized())
		{
			return;
		}

		check(ModuleDepMap.find(Module.get()) != ModuleDepMap.end());

		for (const SharedPtr<IModule>& Dependency : ModuleDepMap[Module.get()])
		{
			if (!Dependency->IsInitialized())
			{
				LoadModule_Impl(Dependency);
			}
		}

		SCAR_LOG(LogVerbose, "Initializing Module %s", Module->GetModuleName());
		Module->Initialize();
		Modules[Module->GetModuleName()] = Module;
	}

	String ModuleManager::GetDependencyGraphViz()
	{
		StringStream Stream;
		for (const auto& [Module, Deps] : ModuleDepMap)
		{
			for (const auto& Dep : Deps)
			{
				Stream << "\"" << Dep->GetModuleName() << "\"" << "->" << "\"" << Module->GetModuleName() << "\"" << std::endl;
			}
		}

		return Stream.str();
	}

	void ModuleManager::Shutdown()
	{
		for (const auto& [Name, Module] : Modules)
		{
			Module->Shutdown();
		}

		Modules.clear();
		ModuleDepMap.clear();
	}

	void ModuleManager::PreUpdate()
	{
		for (const auto& [Name, Module] : Modules)
		{
			Module->PreUpdate();
		}
	}

	void ModuleManager::Update()
	{
		for (const auto& [Name, Module] : Modules)
		{
			Module->Update();
		}
	}

	void ModuleManager::PostUpdate()
	{
		for (const auto& [Name, Module] : Modules)
		{
			Module->PostUpdate();
		}
	}
}