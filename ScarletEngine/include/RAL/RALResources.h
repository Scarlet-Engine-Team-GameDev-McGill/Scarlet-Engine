#pragma once

#include "Core/CoreMinimal.h"
#include "AssetManager/AssetHandle.h"

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

	class RALVertexBuffer
	{
	public:
		RALVertexBuffer(uint32_t InSize, uint32_t InUsage)
			: Size(InSize)
			, Usage(InUsage)
		{}
		
		virtual void UploadData(void* DataPtr, size_t Size) const = 0;
		virtual void Release() = 0;
	protected:
		uint32_t Size;
		uint32_t Usage;
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

		RALShader* GetShader(RALShaderStage Stage) const
		{
			switch (Stage)
			{
			case RALShaderStage::Vertex: return VertexShader;
			case RALShaderStage::Pixel: return PixelShader;
			case RALShaderStage::Geometry: return GeometryShader;
			case RALShaderStage::Compute: return ComputeShader;
			default: check(false); return nullptr;
			}
		}
	protected:
		RALShader* VertexShader;
		RALShader* PixelShader;
		RALShader* GeometryShader;
		RALShader* ComputeShader;
	};
}