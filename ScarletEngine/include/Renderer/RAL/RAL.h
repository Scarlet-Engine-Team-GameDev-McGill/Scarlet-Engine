#pragma once

#include "Core/CoreMinimal.h"
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

		virtual void* GetWindowPtr() const = 0;
		virtual void SetWindowCtx(void* WindowPtr) = 0;

		// #todo: move window functions out of RAL
		virtual void SwapWindowBuffers() const = 0;
		virtual void PollWindowEvents() const = 0;

		// Functions which correspond to rendering commands
		virtual void SetClearColorCommand(const glm::vec4& ClearColor) const = 0;
		virtual void ClearCommand(bool bColor, bool bDepth, bool bStencil) const = 0;

		// Functions for device resource management and creation

		virtual RALFramebuffer* CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples) const = 0;

		virtual RALTexture2D* CreateTexture2D(const std::weak_ptr<TextureHandle>& AssetHandle) const = 0;

		virtual RALVertexBuffer* CreateVertexBuffer(uint32_t Size, uint32_t Usage) const = 0;
		
		virtual RALShader* CreateShader(RALShaderStage Stage, const std::vector<uint8_t>& ShaderCode) const = 0;
		virtual RALShaderProgram* CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) const = 0;
	};
}