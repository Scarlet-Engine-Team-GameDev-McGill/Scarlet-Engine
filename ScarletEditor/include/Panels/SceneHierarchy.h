#pragma once

#include <memory>
#include "ECS/ECS.h"

namespace ScarletEngine
{
	class World;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(const std::shared_ptr<World>& InRepresentingWorld);
		void Draw();
	private:
		 std::weak_ptr<World> RepresentingWorld;
	};
}