#include "ITickable.h"
#include "TickableList.h"

namespace ScarletEngine
{
	ITickable::ITickable()
	{
		TickableList::Get().QueueAddTickable(this);
	}

	ITickable::~ITickable()
	{
		TickableList::Get().RemoveTickable(this);
	}
}