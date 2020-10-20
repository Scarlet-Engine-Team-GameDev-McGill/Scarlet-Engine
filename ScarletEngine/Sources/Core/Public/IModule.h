#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
	class IModule
	{
	public:
		IModule();

		virtual void Startup() = 0;
		virtual void Shutdown() = 0;

		//#todo_Module: Not sure if modules should tick
		virtual void PreUpdate() {}
		virtual void Update() {}
		virtual void PostUpdate() {}

		virtual const char* GetModuleName() const = 0;

		virtual Array<const char*> GetDependencies() const = 0;

		void Initialize();
		bool IsInitialized() const { return bInitialized; }
	private:
		bool bInitialized = false;
	};
}
