#include "Core/ITickable.h"
#include "Core/CoreUtils.h"
#include "Core/Engine.h"

namespace ScarletEngine
{
	ITickable::ITickable()
	{
		Engine& GEngine = Engine::Get();
		check(GEngine.IsInitialized());

		GEngine.QueueAddTickable(this);
	}

	ITickable::~ITickable()
	{
		Engine& GEngine = Engine::Get();

		GEngine.RemoveTickable(this);
	}
}