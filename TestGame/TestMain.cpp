#include "Core/Core.h"

int main()
{
	using namespace ScarletEngine;

	Engine& GEngine = Engine::Get();
	GEngine.Initialize();

	GEngine.Run();

	return 0;
}