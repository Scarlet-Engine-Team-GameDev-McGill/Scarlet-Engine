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
		SharedPtr<World> TestWorld = MakeShared<World>();

		GEngine->Run();
	}

	return 0;
}