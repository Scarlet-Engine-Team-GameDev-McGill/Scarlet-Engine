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
		Array<WeakPtr<SceneHierarchyItem>> Children;
	};
}