#pragma once

#include "CoreMinimal.h"
#include "RALResources.h"

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

	class OpenGLTexture2D : public RALTexture2D
	{
	public:
		OpenGLTexture2D(const WeakPtr<TextureHandle>& InAssetHandle);

		virtual ~OpenGLTexture2D();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint64_t GetTextureResource() const override { return TextureObject; }
	private:
		uint32_t TextureObject;
	};

	class OpenGLGpuBuffer : public RALGpuBuffer
	{
	public:
		OpenGLGpuBuffer(uint32_t InSize, RALBufferUsage InUsage);

		virtual void UploadData(void* DataPtr, size_t InSize) const override;
		virtual void Release() override;

		uint32_t BufferObject;
	};

	class OpenGLVertexArray : public RALVertexArray
	{
	public:
		OpenGLVertexArray(const RALGpuBuffer* VB, const RALGpuBuffer* IB);
		
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Release() override;

		uint32_t VAObject;
	};

	class OpenGLShader : public RALShader
	{
	public:
		OpenGLShader(RALShaderStage Stage, const String& ShaderPath);
	private:
		friend class OpenGLShaderProgram;
		uint32_t ShaderObject;
	};

	class OpenGLShaderProgram : public RALShaderProgram
	{
	public:
		OpenGLShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniformMat4(const glm::mat4& Mat, const char* Binding) const override;
		virtual void SetUniformVec3(const glm::vec3& Vec, const char* Binding) const override;
	private:
		uint32_t ProgramObject;
	};
}