#include "ModuleManager.h"
#include "IModule.h"

namespace ScarletEngine
{
	Array<IModule*> ModuleManager::ModulesToAdd;
	UnorderedMap<String, IModule*> ModuleManager::Modules;
	UnorderedMap<IModule*, Set<IModule*>> ModuleManager::ModuleDepMap;

	bool ModuleManager::bStarted = false;

	void ModuleManager::RegisterModule(IModule* Module)
	{
		check(!bStarted);
		ModulesToAdd.push_back(Module);
	}

	void ModuleManager::Startup()
	{
		FillDependencyMap();

		for (const auto& Module : ModulesToAdd)
		{
			InitializeModuleAndDependencies(Module);
		}

#ifdef DEBUG
		SCAR_LOG(LogVerbose, "Module dependency graph: \ndigraph ModuleDependencies {\n%s\n}", GetDependencyGraphViz().c_str());
#endif

		ModulesToAdd.clear();
		bStarted = true;
	}
	
	void ModuleManager::FillDependencyMap()
	{
		for (const auto& Module : ModulesToAdd)
		{
			ModuleDepMap[Module] = Set<IModule*>();
			for (const auto& DependencyName : Module->GetDependencies())
			{
				String DependencyNameString(DependencyName);
				auto It = std::find_if(ModulesToAdd.begin(), ModulesToAdd.end(), [&DependencyNameString](IModule* PotentialDep)
					{
						return DependencyNameString == PotentialDep->GetModuleName();
					});

				if (It != ModulesToAdd.end())
				{
					ModuleDepMap[Module].insert(*It);
				}
			}
		}
	}

	void ModuleManager::InitializeModuleAndDependencies(IModule* Module)
	{
		if (Module->IsInitialized())
		{
			return;
		}

		check(ModuleDepMap.find(Module) != ModuleDepMap.end());

		for (const auto& Dependency : ModuleDepMap[Module])
		{
			if (!Dependency->IsInitialized())
			{
				InitializeModuleAndDependencies(Dependency);
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