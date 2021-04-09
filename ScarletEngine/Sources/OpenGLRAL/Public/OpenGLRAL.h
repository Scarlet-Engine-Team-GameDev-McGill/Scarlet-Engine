#pragma once

#include "CoreMinimal.h"
#include "RAL.h"

namespace ScarletEngine
{
	class OpenGLRAL : public RAL
	{
	public:
		virtual void Initialize() override;
		virtual void Terminate() override;
		virtual void Submit() override;

		virtual const char* GetBackendName() const override { return "OpenGL"; }

		virtual GPUInfo GetGPUInfo() const override;

		virtual void SetClearColorCmd(const glm::vec4& ClearColor) override;
		virtual void ClearCmd(bool bColor, bool bDepth, bool bStencil) override;
		virtual void DrawVertexArrayCmd(const RALVertexArray* VA) override;

		virtual RALFramebuffer* CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples) override;
		virtual RALTexture2D* CreateTexture2D(const WeakPtr<TextureHandle>& AssetHandle) override;
		virtual RALGpuBuffer* CreateBuffer(uint32_t Size, RALBufferUsage Usage) override;
		virtual RALVertexArray* CreateVertexArray(const RALGpuBuffer* VB, const RALGpuBuffer* IB) override;
		virtual RALShader* CreateShader(RALShaderStage Stage, const String& ShaderPath) override;
		virtual RALShaderProgram* CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) override;
	private:
		virtual RALCommandList* CreateCommandList() const override;
	};
}