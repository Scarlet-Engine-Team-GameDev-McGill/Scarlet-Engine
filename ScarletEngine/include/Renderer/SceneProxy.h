#pragma once

#include "Core/Core.h"
#include "StaticMeshComponent.h"

namespace ScarletEngine
{
	class SceneProxy
	{
	public:
		void AddSMC(const Transform& Trans, const StaticMeshComponent& SMC);
		void Reset();
	private:
		friend class Renderer;
		// #todo: for now cache the drawable objects per frame.
		// later we would simply record a command list and submit it to the rendering thread
		// at the end of the frame
		Array<std::pair<const Transform*, const StaticMeshComponent*>> SMCs;
	};
}