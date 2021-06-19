#pragma once

#include "Core.h"
#include "AssetHandle.h"
#include "RALResources.h"

namespace ScarletEngine
{
	class StaticMeshComponent final : public IComponent
	{
		COMPONENT_DEFINITION(StaticMeshComponent);

		SharedPtr<StaticMeshHandle> MeshHandle;
		RALGpuBuffer* IndexBuff = nullptr;
		RALGpuBuffer* VertexBuff = nullptr;
		RALVertexArray* VertexArray = nullptr;

		// #todo: for now its just a shader, later will be a "material" ptr
		RALShaderProgram* Shader = nullptr;
	};
}