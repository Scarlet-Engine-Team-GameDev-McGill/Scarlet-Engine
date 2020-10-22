#include "Core.h"
#include "World.h"
#include "ECS.h"

int main()
{
	using namespace ScarletEngine;
	GEngine = MakeUnique<Engine>();
	GEngine->Initialize();

	{
		// If you need to test any runtime code outside of the editor environment,
		// this is where you can put it.
	}

	GEngine->Terminate();
	GEngine.reset();

	return 0;
}