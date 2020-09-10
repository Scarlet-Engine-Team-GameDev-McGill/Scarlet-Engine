#include "Renderer/Renderer.h"
#include "Core/Core.h"

int main()
{
	ScarletEngine::TestLibraries();

	ScarletEngine::Logger::Get().SetLogFile("Log.txt");
	SCAR_LOG(LogInfo, "Hello world from GameDev McGill");
	return 0;
}