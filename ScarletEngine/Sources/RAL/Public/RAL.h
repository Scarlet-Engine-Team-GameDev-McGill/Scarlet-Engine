#pragma once

#include "CoreMinimal.h"
#include "RALResources.h"

namespace ScarletEngine
{
	class Window;

	enum class RenderAPI
	{
		OpenGL,
		Vulkan,
		Invalid
	};

	struct GPUInfo
	{
		const char* Vendor;
		const char* Renderer;
		const char* Version;
	};

	class RAL
	{
	public:
		virtual ~RAL() {}

		virtual void Initialize() = 0;

		virtual void Terminate() = 0;

		virtual const char* GetBackendName() const = 0;

		virtual GPUInfo GetGPUInfo() const = 0;

		// Functions which correspond to rendering commands
		virtual void SetClearColorCommand(const glm::vec4& ClearColor) const = 0;
		virtual void ClearCommand(bool bColor, bool bDepth, bool bStencil) const = 0;
		virtual void DrawVertexArray(const RALVertexArray* VA) const = 0;

		// Functions for device resource management and creation

		virtual RALFramebuffer* CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples) const = 0;

		virtual RALTexture2D* CreateTexture2D(const WeakPtr<TextureHandle>& AssetHandle) const = 0;

		virtual RALGpuBuffer* CreateBuffer(uint32_t Size, RALBufferUsage Usage) const = 0;
		virtual RALVertexArray* CreateVertexArray(const RALGpuBuffer* VB, const RALGpuBuffer* IB) const = 0;
		
		virtual RALShader* CreateShader(RALShaderStage Stage, const String& ShaderPath) const = 0;
		virtual RALShaderProgram* CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) const = 0;
	
		static RAL& Get() { return *Instance; }
		static RenderAPI GetAPI() { return API; }
	private:
		friend class RenderModule;
		static UniquePtr<RAL> Instance;
		static RenderAPI API;
	};
}