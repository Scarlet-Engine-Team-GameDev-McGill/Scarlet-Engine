#include "World.h"

namespace ScarletEngine
{
	World::World()
		: LastDeltaTime()
	{
	}

	void World::Initialize()
	{
	}

	void World::Tick(double)
	{
		ZoneScoped
		SystemScheduler::Get().RunUpdate(&Reg);
	}

	void World::FixedTick(double)
	{
		ZoneScoped
		SystemScheduler::Get().RunFixedUpdate(&Reg);
	}
}