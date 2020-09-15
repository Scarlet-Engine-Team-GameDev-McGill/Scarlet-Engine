#pragma once

#include <cstdint>
#include <vector>
#include "Core/CoreUtils.h"

namespace ScarletEngine
{
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