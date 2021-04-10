#pragma once

#include "CoreMinimal.h"
#include "RALResources.h"
#include "RALCommandList.h"

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

		virtual void Initialize();
		virtual void Terminate() = 0;
		virtual void Submit();

		virtual const char* GetBackendName() const = 0;

		virtual GPUInfo GetGPUInfo() const = 0;

		// Functions which correspond to rendering commands
		virtual void SetClearColorCmd(const glm::vec4& ClearColor) = 0;
		virtual void ClearCmd(bool bColor, bool bDepth, bool bStencil) = 0;
		virtual void DrawVertexArrayCmd(const RALVertexArray* VA) = 0;

		// Functions for device resource management and creation

		virtual RALFramebuffer* CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples) = 0;

		virtual RALTexture2D* CreateTexture2D(const WeakPtr<TextureHandle>& AssetHandle) = 0;

		virtual RALGpuBuffer* CreateBuffer(uint32_t Size, RALBufferUsage Usage) = 0;
		virtual RALVertexArray* CreateVertexArray(const RALGpuBuffer* VB, const RALGpuBuffer* IB) = 0;
		
		virtual RALShader* CreateShader(RALShaderStage Stage, const String& ShaderPath) = 0;
		virtual RALShaderProgram* CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) = 0;
	
		void QueueCommand(const Function<void(RALCommandList&)>& Cmd, const char* CommandLabel = "");
		
		static RAL& Get() { return *Instance; }
		static RenderAPI GetAPI() { return API; }
	protected:
		virtual RALCommandList* CreateCommandList() const = 0;
	protected:
		Queue<UniquePtr<RALCommandList>> CommandListQueue;
	private:
		friend class RenderModule;
		static UniquePtr<RAL> Instance;
		static RenderAPI API;
	};
}
