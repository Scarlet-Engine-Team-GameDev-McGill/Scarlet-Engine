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