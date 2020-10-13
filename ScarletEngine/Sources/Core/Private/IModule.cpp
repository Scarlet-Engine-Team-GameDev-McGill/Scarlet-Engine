#include "IModule.h"

#include "ModuleManager.h"

namespace ScarletEngine
{
	IModule::IModule()
	{
		ModuleManager::RegisterModule(this);
	}

	void IModule::Initialize()
	{
		Startup();
		bInitialized = true;
	}
}