#include "Core/Core.h"
#include "Editor.h"
#include "EditorStyle.h"

int main()
{
	using namespace ScarletEngine;

	EditorStyle::Initialize();

	GEngine = MakeUnique<Engine>();
	GEngine->Initialize();

	// initialize the global editor
	GEditor = MakeUnique<Editor>();

	GEngine->Run();

	GEditor.reset();
	GEngine->Terminate();
	GEngine.reset();

	SCAR_LOG(LogInfo, "Terminating");

	return 0;
}