#include "ECS/Registry.h"

namespace ScarletEngine
{
	void Registry::DestroyEntity(EID EntityID)
	{
		for (auto& Pair : ComponentContainers)
		{
			Pair.second->Remove(EntityID);
		}
	}
}