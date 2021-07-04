#pragma once
#include "Core.h"

namespace ScarletEngine::Achilles
{
	class SpringComponent final : public IComponent
	{
		COMPONENT_DEFINITION(Spring);

		float RestLength = 0.25f;
		float Stiffness = 1000.f;
		float Damping = 0.4f;
		EID Anchor;
	};

	class SpringCollection final : public IComponent
	{
		COMPONENT_DEFINITION(Spring Collection);

		size_t Size() const { return SpringComponents.size(); }

		SpringComponent& At(size_t Index) { return SpringComponents.at(Index); }

		template <class... Args>
		decltype(auto) EmplaceBack(Args&&... args)
		{
			return SpringComponents.emplace_back(std::forward(args)...);
		}

		Array<SpringComponent> SpringComponents;
	};
}
