#include "Renderer/SceneProxy.h"

namespace ScarletEngine
{
	void SceneProxy::AddSMC(const Transform& Trans, const StaticMeshComponent& SMC)
	{
		SMCs.push_back(std::make_pair(&Trans, &SMC));
	}

	void SceneProxy::Reset()
	{
		SMCs.clear();
	}
}