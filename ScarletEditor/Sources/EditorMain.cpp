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
		//ScarletEngine::Achilles::DemoKepler(GEditor->GetActiveWorld());
		//ScarletEngine::Achilles::DemoSpherePlaneCollision(GEditor->GetActiveWorld());
		//ScarletEngine::Achilles::DemoSphereSphereCollision(GEditor->GetActiveWorld());
		//ScarletEngine::Achilles::DemoSpring(GEditor->GetActiveWorld());
		//ScarletEngine::Achilles::DemoRope(GEditor->GetActiveWorld(), glm::vec3(0.25f, 0.f, 0.f));
		//ScarletEngine::Achilles::DemoRopeCollision(GEditor->GetActiveWorld());
		ScarletEngine::Achilles::DemoFlag(GEditor->GetActiveWorld());
		//ScarletEngine::Achilles::DemoSoftBodies(GEditor->GetActiveWorld());
	}

	GEngine->Run();

	GEditor.reset();
	GEngine->Terminate();
	GEngine.reset();

	SCAR_LOG(LogInfo, "Terminating");

	return 0;
}
