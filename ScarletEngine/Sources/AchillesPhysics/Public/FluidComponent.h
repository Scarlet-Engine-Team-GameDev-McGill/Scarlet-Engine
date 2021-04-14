#pragma once
#include "Core.h"

namespace ScarletEngine::Achilles
{
	struct FluidComponent
	{
		float Density = 1.f;
		float CurrentDensity;
		float DistanceView = 10.f;
		float Lambda;
		float H = 0.5f;
		float Epsilon = 0.0001f;
	};
}