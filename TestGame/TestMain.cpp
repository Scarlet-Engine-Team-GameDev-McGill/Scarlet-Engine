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
		SCAR_LOG(LogInfo, "Tick #%d", Count++);
	}

	int Count = 0;
};

int main()
{
	using namespace ScarletEngine;
	TestLibraries();

	Engine& GEngine = Engine::Get();
	GEngine.Initialize();

	TestTickable Tickable;

	GEngine.Run();

	return 0;
}