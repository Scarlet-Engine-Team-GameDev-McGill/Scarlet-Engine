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

		bool bIsSelected = false;

		std::weak_ptr<Entity> Ent;
		std::vector<std::weak_ptr<SceneHierarchyItem>> Children;
	};
}