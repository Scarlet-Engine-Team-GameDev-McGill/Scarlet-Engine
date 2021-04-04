#pragma once
#include "Core.h"
#include "Transform.h"

namespace Achilles
{
	struct SpringComponent
	{
		ScarletEngine::EID Anchor;
		float RestLength = 0.2f;
		float Stiffness = 1000.f;
		float Damping = 0.4f;
	};
};