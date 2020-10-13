#include "Registry.h"

namespace ScarletEngine
{
	void Registry::DestroyEntity(EID EntityID)
	{
		ZoneScoped
		for (auto& Pair : ComponentContainers)
		{
			Pair.second->Remove(EntityID);
		}
	}
}