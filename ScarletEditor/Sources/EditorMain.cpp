#include "Core.h"
#include "Engine.h"
#include "Editor.h"
#include "AssetManager.h"
#include "RenderModule.h"
#include "UIModule.h"

#include "AchillesDemo.h"

using namespace ScarletEngine;

int main()
{
	// #todo_core: this should be loaded by a config file or something, for now default it to this.
	AssetManager::SetAssetRoot("../");

	// Register default modules.
	// #todo_core: should be detected and initialized automatically by the engine
	ModuleManager::GetInstance().RegisterModule<RenderModule>();
	ModuleManager::GetInstance().RegisterModule<UIModule>();

	GEngine = MakeUnique<Engine>();
	GEngine->Initialize();

	// Initialize the global editor object
	GEditor = MakeUnique<Editor>();
	// #todo_core: this should be handled automatically by the engine
	GEditor->Initialize();

	// Initialize the default editor world
	{
		// Register gameplay systems with the ECS
		Achilles::RegisterSystems();

		Achilles::DemoKepler(GEditor->GetActiveWorld());
	}

	// Run the engine
	GEngine->Run();

	// Cleanup
	GEditor.reset();
	GEngine->Terminate();
	GEngine.reset();

	return 0;
}
