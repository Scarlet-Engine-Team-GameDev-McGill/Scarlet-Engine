#pragma once

#include "TypeInfo.h"
#include <string>

namespace ScarletEngine
{
	class World;

	struct Entity
	{
	public:
		Entity(const std::string& InName)
			: Name(InName)
		{}

		EID ID;
		std::string Name;
		
		World* OwningWorld;
	};
}