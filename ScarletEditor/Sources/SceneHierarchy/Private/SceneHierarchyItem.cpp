#include "SceneHierarchyItem.h"

namespace ScarletEngine
{
	SceneHierarchyItem::SceneHierarchyItem(const SharedPtr<Entity>& InEnt)
		: Ent(InEnt)
	{}

	const String& SceneHierarchyItem::GetDisplayString() const
	{
		return Ent.lock()->GetName();
	}
}