#pragma once

#include "CoreMinimal.h"
#include "IModule.h"

namespace ScarletEngine
{
	class ModuleManager
	{
	public:
		/** Registers a new module of `ModuleType` */
		template <typename ModuleType>
		static void RegisterModule()
		{
			ModuleManager& Instance = GetInstance();
			check(!Instance.bStarted);

			const SharedPtr<IModule> Module(ScarNew(ModuleType));
			Instance.Modules.push_back(Module);
		}

		/** Returns a pointer to the specified module, casted to type */
		template <typename ModuleType>
		NODISCARD static ModuleType* GetModule(StringView ModuleName)
		{
			ModuleManager& Instance = GetInstance();
			auto It = std::find_if(Instance.Modules.begin(), Instance.Modules.end(), [ModuleName](const SharedPtr<IModule>& PotentialMatch)
			{
				return StringView(PotentialMatch->GetModuleName()) == ModuleName;
			});
			return (It != Instance.Modules.end()) ? static_cast<ModuleType*>(It->get()) : nullptr;
		}

		/** Returns a pointer to the specified module, checked to ensure it exists */
		template <typename ModuleType>
		NODISCARD static ModuleType* GetModuleChecked(StringView ModuleName)
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
		
		static ModuleManager& GetInstance() { static ModuleManager Instance; return Instance; }
	private:
		/** Extract pointers to module dependencies based on module dependency names */
		void ResolveModuleOrder();
	private:
		Array<SharedPtr<IModule>> Modules;

		bool bStarted = false;
	};
}

#define IMPLEMENT_MODULE(ModuleType, ...) \
	virtual Array<const char*> GetDependencies() const override { return {#__VA_ARGS__};} \
	virtual const char* GetModuleName() const override { return #ModuleType; }
