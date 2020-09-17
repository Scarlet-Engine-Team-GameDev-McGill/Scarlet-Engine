#include "World.h"

namespace ScarletEngine
{
	World::World()
		: LastDeltaTime()
	{
	}

	void World::Tick(double DeltaTime)
	{
		LastDeltaTime = DeltaTime;
		RunSystems();
	}

}