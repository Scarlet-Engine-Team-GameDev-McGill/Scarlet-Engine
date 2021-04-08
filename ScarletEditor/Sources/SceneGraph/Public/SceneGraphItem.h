#pragma once

#include "Core.h"
#include "ECS.h"

namespace ScarletEngine
{
	class SceneHierarchyItem
	{
	public:
		SceneHierarchyItem(const SharedPtr<EntityHandle>& InEnt);
		
		const char* GetDisplayString() const;

		bool bIsSelected = false;

		WeakPtr<EntityHandle> Ent;
		Array<WeakPtr<SceneHierarchyItem>> Children;
	};
}