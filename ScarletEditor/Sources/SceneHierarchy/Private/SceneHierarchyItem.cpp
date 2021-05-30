#include "SceneHierarchyItem.h"

namespace ScarletEngine
{
	SceneHierarchyItem::SceneHierarchyItem(const SharedPtr<Entity>& InEnt)
		: Ent(InEnt)
	{}

	const char* SceneHierarchyItem::GetDisplayString() const
	{
		return Ent.lock()->Name.c_str();
	}
}