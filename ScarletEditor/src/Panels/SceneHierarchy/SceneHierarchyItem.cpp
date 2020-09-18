#include "Panels/SceneHierarchy/SceneHierarchyItem.h"

namespace ScarletEngine
{
	SceneHierarchyItem::SceneHierarchyItem(const std::shared_ptr<Entity>& InEnt)
		: Ent(InEnt)
	{}

	const char* SceneHierarchyItem::GetDisplayString() const
	{
		return Ent.lock()->Name.c_str();
	}
}