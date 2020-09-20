#pragma once

#include "Core/Core.h"
#include "ECS/ECS.h"

namespace ScarletEngine
{
	class SceneHierarchyItem
	{
	public:
		SceneHierarchyItem(const SharedPtr<Entity>& InEnt);
		
		const char* GetDisplayString() const;

		bool bIsSelected = false;

		WeakPtr<Entity> Ent;
		std::vector<std::weak_ptr<SceneHierarchyItem>> Children;
	};
}