#include "Renderer/Renderer.h"
#include "Core/Core.h"

class TestTickable : public ScarletEngine::ITickable
{
public:
	virtual void Tick(double) override
	{
		if (Count >= 5)
		{
			ScarletEngine::Engine::Get().SignalQuit();
		}
		SCAR_LOG(LogVerbose, "Tick #%d", Count++);
	}

	int Count = 0;
};

int main()
{
	using namespace ScarletEngine;
	TestLibraries();

	Event<int, int, int> TestEvent;
	TestEvent.Bind([](int a, int b, int c)
		{
			SCAR_LOG(LogVerbose, "Event %d %d %d", a, b, c);
		});
	TestEvent.Bind([](int a, int b, int c)
		{
			SCAR_LOG(LogVerbose, "Event %d %d %d", a * 2, b * 2, c * 2);
		});

	TestEvent.Broadcast(10, 10, 10);

	Delegate<int(int)> Add;
	Add.Bind([](int a) { return a * 2; });
	SCAR_LOG(LogVerbose, "Delegate %d", Add(10));

	Delegate<void(int)> Nothing;
	Nothing.Bind([](int) {});
	Nothing(10);

	Engine& GEngine = Engine::Get();
	GEngine.Initialize();

	TestTickable Tickable;

	GEngine.Run();

	Event<void(int)> a;
	return 0;
}