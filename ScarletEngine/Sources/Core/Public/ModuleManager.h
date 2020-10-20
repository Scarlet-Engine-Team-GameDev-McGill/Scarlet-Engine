#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
	class IModule;

	class ModuleManager
	{
	public:
		static void RegisterModule(IModule* Module);

		template <typename ModuleType>
		NODISCARD static ModuleType* GetModule(const String& ModuleName)
		{
			return (Modules.find(ModuleName) != Modules.end()) ? static_cast<ModuleType*>(Modules.at(ModuleName)) : nullptr;
		}

		template <typename ModuleType>
		NODISCARD static ModuleType* GetModuleChecked(const String& ModuleName)
		{
			ModuleType* Ptr = GetModule<ModuleType>(ModuleName);
			check(Ptr != nullptr);
			return Ptr;
		}

		/** Startup all modules */
		static void Startup(); 
		/** Shutdown all modules */
		static void Shutdown();

		/** Update all modules */
		static void PreUpdate();
		static void Update();
		static void PostUpdate();

		static String GetDependencyGraphViz();
	private:
		/** Extract pointers to module dependencies based on module dependency names */
		static void FillDependencyMap();
		static void InitializeModuleAndDependencies(IModule* Module);
	private:
		/** List of modules to add. 
			Since we cannot call virtual functions from constructor, 
			we must store them here until we are ready to initialize */
		static Array<IModule*> ModulesToAdd;
		static UnorderedMap<String, IModule*> Modules;
		// In theory this doesn't need to be a class member but we will keep in here for debugging purposes
		static UnorderedMap<IModule*, Set<IModule*>> ModuleDepMap;
		static bool bStarted;
	};
}

#define DECLARE_MODULE(ModuleType) ModuleType _##ModuleType

#define IMPLEMENT_MODULE(ModuleType, ...) \
	virtual Array<const char*> GetDependencies() const override { return {#__VA_ARGS__};} \
	virtual const char* GetModuleName() const override { return #ModuleType; }
