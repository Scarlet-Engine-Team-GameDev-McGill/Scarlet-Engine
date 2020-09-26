#include "Core/ITickable.h"
#include "Core/CoreUtils.h"
#include "Core/Engine.h"

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