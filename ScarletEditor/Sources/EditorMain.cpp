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

    GEngine = GEditor = ScarNew(Editor);
    GEngine->Initialize();

    // Initialize the default editor world
    {
        // Register gameplay systems with the ECS
        Achilles::RegisterSystems();

        Achilles::DemoKepler(GEngine->GetActiveWorld());
    }

    // Run the engine
    GEngine->Run();

    // Cleanup
    ScarDelete(GEngine);
    GEngine = nullptr;

    return 0;
}
