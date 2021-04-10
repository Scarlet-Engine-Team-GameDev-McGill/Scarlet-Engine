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

		virtual void Startup() = 0;
		virtual void Shutdown() = 0;

		//#todo_Module: Not sure if modules should tick
		virtual void PreUpdate() {}
		virtual void Update() {}
		virtual void PostUpdate() {}

		virtual const char* GetModuleName() const = 0;

		virtual Array<const char*> GetDependencies() const = 0;
	};
}
