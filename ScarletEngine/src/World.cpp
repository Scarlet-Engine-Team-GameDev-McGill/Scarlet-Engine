#include "World.h"

namespace ScarletEngine
{
	World::World()
		: LastDeltaTime()
	{
		ZoneScoped
	}

	void World::Initialize()
	{
		ZoneScoped
	}

	void World::Tick(double DeltaTime)
	{
		ZoneScoped
		LastDeltaTime = DeltaTime;
		RunSystems();
	}

}