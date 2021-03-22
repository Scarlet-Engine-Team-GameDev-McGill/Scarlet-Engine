#pragma once

#include "CoreMinimal.h"
#include "IModule.h"

namespace ScarletEngine
{
	class ModuleManager
	{
	public:
		template <typename ModuleType>
		static void RegisterModule()
		{
			ModuleManager& Instance = GetInstance();
			check(!Instance.bStarted);

			SharedPtr<IModule> Module((IModule*)ScarNew(ModuleType));
			Instance.ModulesToAdd.push_back(Module);
		}

		template <typename ModuleType>
		NODISCARD static ModuleType* GetModule(const String& ModuleName)
		{
			ModuleManager& Instance = GetInstance();
			return (Instance.Modules.find(ModuleName) != Instance.Modules.end()) ? static_cast<ModuleType*>(Instance.Modules.at(ModuleName).get()) : nullptr;
		}

		template <typename ModuleType>
		NODISCARD static ModuleType* GetModuleChecked(const String& ModuleName)
		{
			ModuleManager& Instance = GetInstance();
			ModuleType* Ptr = Instance.GetModule<ModuleType>(ModuleName);
			check(Ptr != nullptr);
			return Ptr;
		}

		/** Startup all modules */
		void Startup(); 
		/** Shutdown all modules */
		void Shutdown();

		/** Update all modules */
		void PreUpdate();
		void Update();
		void PostUpdate();

		String GetDependencyGraphViz();

		static ModuleManager& GetInstance() { static ModuleManager Instance; return Instance; }
	private:
		/** Extract pointers to module dependencies based on module dependency names */
		void FillDependencyMap();
		/** Load a module and all it's dependencies */
		void LoadModule_Impl(const SharedPtr<IModule>& Module);
	private:
		UnorderedMap<String, SharedPtr<IModule>> Modules;
		// In theory this doesn't need to be a class member but we will keep in here for debugging purposes
		UnorderedMap<IModule*, Set<SharedPtr<IModule>>> ModuleDepMap;

		Array<SharedPtr<IModule>> ModulesToAdd;
		bool bStarted;
	};
}

#define IMPLEMENT_MODULE(ModuleType, ...) \
	virtual Array<const char*> GetDependencies() const override { return {#__VA_ARGS__};} \
	virtual const char* GetModuleName() const override { return #ModuleType; }
