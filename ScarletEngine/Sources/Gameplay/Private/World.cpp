#include "World.h"
#include "StaticMeshComponent.h"

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
		WorldSystems.RunUpdate(&Reg);
	}

	void World::FixedTick(double DeltaTime)
	{
		ZoneScoped
		WorldSystems.RunFixedUpdate(&Reg);
	}

}