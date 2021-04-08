#pragma once

#include "TypeInfo.h"
#include <string>

namespace ScarletEngine
{
	class World;

	struct Entity
	{
	public:
		Entity(const String& InName, EID InID, World* InOwningWorld)
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