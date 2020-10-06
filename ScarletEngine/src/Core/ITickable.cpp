#include "Core/ITickable.h"
#include "Core/Engine.h"

namespace ScarletEngine
{
	ITickable::ITickable()
	{
		ZoneScoped
		GEngine->QueueAddTickable(this);
	}

	ITickable::~ITickable()
	{
		ZoneScoped
		GEngine->RemoveTickable(this);
	}
}