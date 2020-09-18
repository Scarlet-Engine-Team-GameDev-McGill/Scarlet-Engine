#include "Core/Core.h"
#include "Editor.h"

int main()
{
	using namespace ScarletEngine;

	Engine& GEngine = Engine::Get();
	GEngine.Initialize();

	// initialize the global editor
	Editor::Get();
	GEngine.Run();

	return 0;
}