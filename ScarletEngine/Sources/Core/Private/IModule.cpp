#include "IModule.h"

namespace ScarletEngine
{
	void IModule::Initialize()
	{
		Startup();
		bInitialized = true;
	}
}