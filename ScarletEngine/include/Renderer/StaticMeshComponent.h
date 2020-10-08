#pragma once

#include "AssetManager/AssetHandle.h"

#include "RAL/RALResources.h"
namespace ScarletEngine
{
	struct StaticMeshComponent
	{
		SharedPtr<StaticMeshHandle> MeshHandle;
		RALGpuBuffer* IndexBuff;
		RALGpuBuffer* VertexBuff;
		RALVertexArray* VertexArray;

		// #todo: for now its just a shader, later will be a "material" ptr
		RALShaderProgram* Shader;
	};
}