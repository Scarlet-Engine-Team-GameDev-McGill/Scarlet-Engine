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

	World TestWorld;

	TestWorld.AddSystem<Position, const Velocity>([](EID EntityID, Position&, const Velocity&)
		{
			SCAR_LOG(LogVerbose, "TEST");
		});
	
	Registry& Reg = TestWorld.GetRegistry();

	Reg.CreateEntity<Position, Velocity>();
	Reg.CreateEntity<Position, Velocity, int>();
	Reg.CreateEntity<Position>();


	Engine& GEngine = Engine::Get();
	GEngine.Initialize();
	GEngine.Run();
	return 0;
}