#include "ITickable.h"
#include "Engine.h"

namespace ScarletEngine
{
	ITickable::ITickable()
	{
		GEngine->QueueAddTickable(this);
	}

	ITickable::~ITickable()
	{
		GEngine->RemoveTickable(this);
	}
}