#include "Renderer/Renderer.h"
#include "Core/Core.h"

int main()
{
	using namespace ScarletEngine;
	TestLibraries();

	Engine& GEngine = Engine::Get();

	GEngine.Initialize();
	GEngine.Run();

	SCAR_LOG(LogInfo, "Hello world from GameDev McGill");
	return 0;
}