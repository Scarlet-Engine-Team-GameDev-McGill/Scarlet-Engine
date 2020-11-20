#include "ModuleManager.h"
#include "IModule.h"

namespace ScarletEngine
{
	UnorderedMap<String, IModule*> ModuleManager::Modules;
	UnorderedMap<IModule*, Set<IModule*>> ModuleManager::ModuleDepMap;

	bool ModuleManager::bStarted = false;

	/** 
	 * List of modules to add.
	 * Since we cannot call virtual functions from constructor,
	 * we must store them here until we are ready to initialize.
	 * This is a pointer to an array to prevent static reinitialization 
	 * due to linking to multiple copies of the same library
	 */
	Array<IModule*>* ModulesToAdd = nullptr;

	void ModuleManager::RegisterModule(IModule* Module)
	{
		check(!bStarted);
		if (ModulesToAdd == nullptr)
		{
			ModulesToAdd = new Array<IModule*>;
		}
		ModulesToAdd->push_back(Module);
	}

	void ModuleManager::Startup()
	{
		FillDependencyMap();

		if (ModulesToAdd == nullptr)
		{
			SCAR_LOG(LogWarning, "No modules detected. This is probably an error!");
			return;
		}

		for (const auto& Module : *ModulesToAdd)
		{
			InitializeModuleAndDependencies(Module);
		}

#ifdef DEBUG
		SCAR_LOG(LogVerbose, "Module dependency graph: \ndigraph ModuleDependencies {\n%s\n}", GetDependencyGraphViz().c_str());
#endif

		delete ModulesToAdd;
		bStarted = true;
	}
	
	void ModuleManager::FillDependencyMap()
	{
		if (ModulesToAdd == nullptr)
		{
			return;
		}

		for (const auto& Module : *ModulesToAdd)
		{
			ModuleDepMap[Module] = Set<IModule*>();
			for (const auto& DependencyName : Module->GetDependencies())
			{
				String DependencyNameString(DependencyName);
				auto It = std::find_if(ModulesToAdd->begin(), ModulesToAdd->end(), [&DependencyNameString](IModule* PotentialDep)
					{
						return DependencyNameString == PotentialDep->GetModuleName();
					});

				if (It != ModulesToAdd->end())
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