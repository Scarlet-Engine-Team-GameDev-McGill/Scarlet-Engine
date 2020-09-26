#include "Core/Core.h"
#include "Editor.h"

int main()
{
	using namespace ScarletEngine;

	GEngine = MakeUnique<Engine>();
	GEngine->Initialize();

	// initialize the global editor
	GEditor = MakeUnique<Editor>();

	GEngine->Run();

	GEditor.reset();
	GEngine->Terminate();
	GEngine.reset();

	return 0;
}