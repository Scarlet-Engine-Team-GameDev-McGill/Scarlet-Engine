#pragma once

#include "Core/Core.h"
#include "ECS/ECS.h"

namespace ScarletEngine
{
	class SceneHierarchyItem
	{
	public:
		SceneHierarchyItem(const std::shared_ptr<Entity>& InEnt);
		
		const char* GetDisplayString() const;

		std::weak_ptr<Entity> Ent;
	};
}