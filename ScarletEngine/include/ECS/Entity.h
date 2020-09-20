#pragma once

#include "TypeInfo.h"
#include <string>

namespace ScarletEngine
{
	class World;

	struct Entity
	{
	public:
		Entity(const String& InName)
			: Name(InName)
			, OwningWorld(nullptr)
		{
			Name.reserve(64);
		}

		String Name;
		EID ID;
		World* OwningWorld;
	};
}