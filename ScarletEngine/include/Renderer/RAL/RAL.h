#pragma once

#include "RALResources.h"

namespace ScarletEngine
{
	class IRAL
	{
	public:
		virtual ~IRAL() {}

		virtual void Initialize() = 0;

		virtual void Terminate() = 0;

		virtual const char* GetBackendName() const = 0;

		// #todo: move window functions out of RAL
		virtual void SwapWindowBuffers() const = 0;
		virtual void PollWindowEvents() const = 0;

		virtual RALVertexBuffer* CreateVertexBuffer(uint32_t Size, uint32_t Usage) const = 0;
		virtual RALShader* CreateShader(RALShaderStage Stage, const std::vector<uint8_t>& ShaderCode) const = 0;
		virtual RALShaderProgram* CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) const = 0;
	};
}