#pragma once

#include "Core/CoreMinimal.h"
#include "RAL/RAL.h"

struct GLFWwindow;

namespace ScarletEngine
{
	class OpenGLRAL : public RAL
	{
	public:
		virtual void Initialize() override;
		virtual void Terminate() override;

		// #todo: move window functions out of RAL
		virtual void SwapWindowBuffers() const override;
		virtual void PollWindowEvents() const override;

		virtual const char* GetBackendName() const override { return "OpenGL"; }

		virtual void* GetWindowPtr() const override { return Window; }
		virtual void SetWindowCtx(void* WindowPtr) override;

		virtual void SetClearColorCommand(const glm::vec4& ClearColor) const override;
		virtual void ClearCommand(bool bColor, bool bDepth, bool bStencil) const override;

		virtual RALFramebuffer* CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples) const override;
		virtual RALTexture2D* CreateTexture2D(const WeakPtr<TextureHandle>& AssetHandle) const override;
		virtual RALVertexBuffer* CreateVertexBuffer(uint32_t Size, uint32_t Usage) const override;
		virtual RALShader* CreateShader(RALShaderStage Stage, const std::vector<uint8_t>& ShaderCode) const override;
		virtual RALShaderProgram* CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) const override;
	private:
		// #todo: this shouldn't really be part of the RAL but rather some other windowing abstraction layer
		GLFWwindow* Window = nullptr;
	};
}