#pragma once

#include "Renderer/RAL/RALResources.h"

namespace ScarletEngine
{
	class OpenGLVertexBuffer : public RALVertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t InSize, uint32_t InUsage);

		virtual void UploadData(void* DataPtr, size_t InSize) const override;
		virtual void Release() override;
	private:
		uint32_t BufferObject;
	};

	class OpenGLShader : public RALShader
	{
	public:
		OpenGLShader(RALShaderStage Stage, const std::vector<uint8_t>& ShaderCode);
	private:
		friend class OpenGLShaderProgram;
		uint32_t ShaderObject;
	};

	class OpenGLShaderProgram : public RALShaderProgram
	{
	public:
		OpenGLShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader);
	private:
		uint32_t ProgramObject;
	};
}