#include "Core/Core.h"
#include "Editor.h"

int main()
{
	using namespace ScarletEngine;

	Engine& GEngine = Engine::Get();
	GEngine.Initialize();

	Editor GEditor;

	GEngine.Run();

	return 0;
}