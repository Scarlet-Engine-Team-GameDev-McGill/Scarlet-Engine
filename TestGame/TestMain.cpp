#include "Engine.h"
#include "AssetManager.h"
#include "World.h"
#include "RAL.h"
#include "ModuleManager.h"
#include "RenderModule.h"

#include "VulkanRAL.h" // temp for testing

int main()
{
	using namespace ScarletEngine;

	// #todo_Core: this should be loaded by a config file or something, for now default it to this.
	AssetManager::SetAssetRoot("../");
	
	ModuleManager::RegisterModule<RenderModule>();

	GEngine = MakeUnique<Engine>();	
	GEngine->Initialize();

	{
		SharedPtr<World> TestWorld = MakeShared<World>();
		TestWorld->Initialize();

		GEngine->Run();
	}


	GEngine->Terminate();
	GEngine.reset();

	return 0;
}