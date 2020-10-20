#include "SceneProxy.h"

namespace ScarletEngine
{
	void SceneProxy::DrawSMC(const Transform& Trans, const StaticMeshComponent& SMC)
	{
		SMCs.push_back(std::make_pair(&Trans, &SMC));
	}

	void SceneProxy::Reset()
	{
		SMCs.clear();
	}
}