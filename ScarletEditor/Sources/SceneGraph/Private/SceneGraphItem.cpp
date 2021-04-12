#include "SceneGraphItem.h"

namespace ScarletEngine
{
	SceneHierarchyItem::SceneHierarchyItem(const SharedPtr<EntityHandle>& InEnt)
		: Ent(InEnt)
	{}

	const char* SceneHierarchyItem::GetDisplayString() const
	{
		ZoneScoped
		return Ent.lock()->Name.c_str();
	}
}