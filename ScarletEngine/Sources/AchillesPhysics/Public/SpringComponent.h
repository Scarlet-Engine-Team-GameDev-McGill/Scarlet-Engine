#pragma once
#include "Core.h"
#include "Transform.h"

namespace ScarletEngine::Achilles
{
	struct SpringComponent
	{
		EID Anchor;
		float RestLength = 0.25f;
		float Stiffness = 1000.f;
		float Damping = 0.4f;
	};

	using SpringCollection = Array<SpringComponent>;
}
