#pragma once

#include "CoreMinimal.h"
#include "AssetHandle.h"

namespace ScarletEngine
{
	class RALFramebuffer
	{
	public:
		RALFramebuffer(uint32_t InWidth, uint32_t InHeight, uint32_t InSamples)
			: Width(InWidth)
			, Height(InHeight)
			, Samples(InSamples)
		{}
		virtual ~RALFramebuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(uint32_t NewWidth, uint32_t NewHeight) = 0;

		virtual uint64_t GetColorAttachmentID() const = 0;

		glm::ivec2 GetSize() const { return { Width, Height }; }
	protected:
		uint32_t Width;
		uint32_t Height;
		uint32_t Samples;
	};

	class RALTexture2D
	{
	public:
		RALTexture2D(const WeakPtr<TextureHandle>& InAssetHandle)
			: AssetHandle(InAssetHandle)
		{}
		virtual ~RALTexture2D() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint64_t GetTextureResource() const = 0;
	protected:
		WeakPtr<TextureHandle> AssetHandle;
	};

	enum class RALBufferUsage
	{
		STATIC_DRAW,
		STATIC_READ,
		STATIC_COPY,
		DYNAMIC_DRAW,
		DYNAMIC_READ,
		DYNAMIC_COPY,
		INVALID
	};

	// TODO This enum is used similarly to how the above one is used, but
	// vulkan requires us to be more specific about the type in the usage. Should
	// probably merge these somehow in the future
	enum class RALBufferType 
	{
		TRANSFER_SRC,
		TRANSFER_DST,
		VERTEX_BUFFER,
		INDEX_BUFFER,
	};
	
	enum RALBufferPropertyFlagBits {
		DEVICE_LOCAL_BIT = 0x00000001,
		HOST_VISIBLE_BIT = 0x00000002,
		HOST_COHERENT_BIT = 0x00000004,
		HOST_CACHED_BIT = 0x00000008,
	};
	typedef uint32_t RALBufferPropertyFlags;

	class RALGpuBuffer
	{
	public:
		RALGpuBuffer(uint32_t InSize, RALBufferType InType, RALBufferUsage InUsage, 
		RALBufferPropertyFlags InProperties)
			: Size(InSize)
			, Type(InType)
			, Usage(InUsage)
			, Properties(InProperties)
		{}

		virtual ~RALGpuBuffer() {}

		virtual void UploadData(void* DataPtr, size_t Size) const = 0;
		virtual void Release() = 0;

		uint32_t Size;
		RALBufferType Type;
		RALBufferUsage Usage;
		RALBufferPropertyFlags Properties;
	};

	class RALVertexArray
	{
	public:
		RALVertexArray(const RALGpuBuffer* InVB, const RALGpuBuffer* InIB)
			: VB(InVB)
			, IB(InIB)
			, VertexCount(IB->Size / sizeof(uint32_t)) // index buffer is always uint32_t, so count = buff_size / elem_size
		{}

		virtual ~RALVertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Release() = 0;
		inline uint32_t GetCount() const { return VertexCount; }

		const RALGpuBuffer* VB;
		const RALGpuBuffer* IB;
		uint32_t VertexCount;
	};

	enum class RALShaderStage
	{
		Vertex,
		Pixel,
		Geometry,
		Compute
	};

	class RALShader
	{
	public:
		RALShader(RALShaderStage InStage) : Stage(InStage) {}
	protected:
		RALShaderStage Stage;
	};

	class RALShaderProgram
	{
	public:
		RALShaderProgram(
			RALShader* InVertexShader,
			RALShader* InPixelShader,
			RALShader* InGeometryShader,
			RALShader* InComputeShader)
			: VertexShader(InVertexShader)
			, PixelShader(InPixelShader)
			, GeometryShader(InGeometryShader)
			, ComputeShader(InComputeShader)
		{}

		virtual ~RALShaderProgram() {}
		
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniformMat4(const glm::mat4& Mat, const char* Binding) const = 0;
		virtual void SetUniformVec3(const glm::vec3& Vec, const char* Binding) const = 0;
	protected:
		RALShader* VertexShader;
		RALShader* PixelShader;
		RALShader* GeometryShader;
		RALShader* ComputeShader;
	};
}