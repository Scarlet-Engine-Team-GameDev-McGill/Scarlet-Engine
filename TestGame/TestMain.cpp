#include "Engine.h"
#include "AssetManager.h"
#include "World.h"
#include "ModuleManager.h"
#include "RenderModule.h"

int main()
{
	using namespace ScarletEngine;

	// #todo_Core: this should be loaded by a config file or something, for now default it to this.
	AssetManager::SetAssetRoot("../");
	
	ModuleManager::RegisterModule<RenderModule>();

	GEngine = ScarNew(Engine);
	GEngine->Initialize();
 
	{
		//const SharedPtr<World>& ActiveWorld = GEngine->GetActiveWorld();

		// initialize anything you want in the world above this commment
		GEngine->Run();
	}

	ScarDelete(GEngine);

	return 0;
}