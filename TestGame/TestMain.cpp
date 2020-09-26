#include "Core/Core.h"
#include "World.h"
#include "ECS/ECS.h"

struct Position
{
	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
};

struct Velocity
{
	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
};

int main()
{
	using namespace ScarletEngine;

	Engine& GEngine = Engine::Get();
	GEngine.Initialize();
	
	World TestWorld;

	TestWorld.AddSystem<Position, const Velocity>("Test System")
		.Each([](EID, Position&, const Velocity&)
			{
				SCAR_LOG(LogVerbose, "TEST");
			});

	TestWorld.CreateEntity<Position, Velocity>("Pos/Vel");
	TestWorld.CreateEntity<Position, Velocity, int>("Pos/Vel/Int");
	TestWorld.CreateEntity<Position>("Pos");
	
	GEngine.Run();
	return 0;
}