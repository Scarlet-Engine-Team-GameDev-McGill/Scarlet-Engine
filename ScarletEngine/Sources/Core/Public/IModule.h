#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
	/**
	 * Base class for all Engine Modules.
	 * Modules are Engine subunits which each have their own dependencies.
	 * The Engine ensures that each dependency is properly initialized before initializing a given module.
	 * They also allow global access to each module through the ModuleManager class.
	 */
	class IModule
	{
	public:
		virtual ~IModule() {}

		/** Runs any module startup code. This will be called before the engine begins ticking objects */
		virtual void Startup() = 0;
		/** Runs any module shutdown code. This will be called after the engine is finished ticking objects */
		virtual void Shutdown() = 0;

		//#todo_Module: Not sure if modules should tick
		virtual void PreUpdate() {}
		virtual void Update() {}
		virtual void PostUpdate() {}

		/** Returns the name of this module */
		virtual const char* GetModuleName() const = 0;

	protected:
		friend class ModuleManager;
		/** Returns a list of names of modules this module depends on */
		virtual Array<const char*> GetDependencies() const = 0;
	};
}
