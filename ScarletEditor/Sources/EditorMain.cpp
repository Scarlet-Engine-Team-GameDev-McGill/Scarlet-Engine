#include "Core.h"
#include "Engine.h"
#include "Editor.h"
#include "RAL.h"
#include "StaticMeshComponent.h"
#include "AssetManager.h"
#include "RenderModule.h"
#include "UIModule.h"

#include "AchillesDemo.h"

using namespace ScarletEngine;

int main()
{
	// #todo_Core: this should be loaded by a config file or something, for now default it to this.
	AssetManager::SetAssetRoot("../");

	ModuleManager::GetInstance().RegisterModule<RenderModule>();
	ModuleManager::GetInstance().RegisterModule<UIModule>();

	ScarletEngine::Achilles::RegisterSystems();

	GEngine = MakeUnique<Engine>();
	GEngine->Initialize();

	// initialize the global editor
	GEditor = MakeUnique<Editor>();
	// #todo_core: this should be handled automatically by the engine
	GEditor->Initialize();
	{
		ScarletEngine::Achilles::DemoRope(GEditor->GetActiveWorld(), glm::vec3(0.25f, 0.f, 0.f));
	}

	GEngine->Run();

	GEditor.reset();
	GEngine->Terminate();
	GEngine.reset();

	SCAR_LOG(LogInfo, "Terminating");

	return 0;
}
