#pragma once

#include "Core.h"
#include "TypeInfo.h"

namespace ScarletEngine
{
	class World;

	struct EntityHandle
	{
	public:
		EntityHandle(const String& InName, EID InID, World* InOwningWorld)
			: Name(InName)
			, ID(InID)
			, OwningWorld(InOwningWorld)
		{
			Name.reserve(64);
		}

		String Name;
		const EID ID;
		World* const OwningWorld;
	};
}