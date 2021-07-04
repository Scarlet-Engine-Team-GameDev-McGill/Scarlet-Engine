#pragma once

#include "Core.h"
#include "Entity.h"

namespace ScarletEngine
{
	class SceneHierarchyItem
	{
	public:
		SceneHierarchyItem(const SharedPtr<Entity>& InEnt);

		const String& GetDisplayString() const;

		bool bIsSelected = false;

		WeakPtr<Entity> Ent;
		Array<WeakPtr<SceneHierarchyItem>> Children;
	};
}