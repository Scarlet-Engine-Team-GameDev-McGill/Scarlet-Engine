#pragma once

#include "Core/CoreMinimal.h"
#include "Renderer/RAL/RALResources.h"

namespace ScarletEngine
{
	class OpenGLFramebuffer : public RALFramebuffer
	{
	public:
		OpenGLFramebuffer(uint32_t InWidth, uint32_t InHeight, uint32_t InSamples);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Resize(uint32_t NewWidth, uint32_t NewHeight) override;

		virtual uint64_t GetColorAttachmentID() const override { return ColorAttachment; };
	private:
		void RecreateResource();
	private:
		uint32_t FramebufferObject;
		uint32_t ColorAttachment;
		uint32_t DepthAttachment;
	};

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