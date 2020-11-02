#include "ITickable.h"
#include "Engine.h"

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