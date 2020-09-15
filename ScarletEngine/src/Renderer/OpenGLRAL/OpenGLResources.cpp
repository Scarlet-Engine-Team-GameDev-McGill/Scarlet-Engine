#include "Renderer/OpenGLRAL/OpenGLResources.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ScarletEngine
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t InSize, uint32_t InUsage)
		: RALVertexBuffer(InSize, InUsage)
	{
		glGenBuffers(1, &BufferObject);
	}

	void OpenGLVertexBuffer::UploadData(void* DataPtr, size_t InSize) const
	{
		check(InSize <= Size);
		glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
		glBufferData(GL_ARRAY_BUFFER, Size, DataPtr, Usage);
	}

	void OpenGLVertexBuffer::Release()
	{
		glDeleteBuffers(1, &BufferObject);
		BufferObject = 0;
		Usage = 0;
	}


	OpenGLShader::OpenGLShader(RALShaderStage Stage, const std::vector<uint8_t>& ShaderCode)
		: RALShader(Stage)
		, ShaderObject(0)
	{
		switch (Stage)
		{
		case RALShaderStage::Vertex:
			ShaderObject = glCreateShader(GL_VERTEX_SHADER);
			break;
		case RALShaderStage::Pixel:
			ShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case RALShaderStage::Geometry:
			ShaderObject = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case RALShaderStage::Compute:
			ShaderObject = glCreateShader(GL_COMPUTE_SHADER);
			break;
		}
		glShaderBinary(1, &ShaderObject, GL_SHADER_BINARY_FORMAT_SPIR_V, ShaderCode.data(), NULL);
		glSpecializeShader(ShaderObject, "main", 0, NULL, NULL);

		int Success;
		char InfoBuffer[512];
		glGetShaderiv(ShaderObject, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(ShaderObject, 512, NULL, InfoBuffer);
			SCAR_LOG(LogError, "%s", InfoBuffer);
			check(false);
		}
	}

	OpenGLShaderProgram::OpenGLShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader)
		: RALShaderProgram(InVertexShader, InPixelShader, InGeometryShader, InComputeShader)
		, ProgramObject(0)
	{
		ProgramObject = glCreateProgram();
		if (VertexShader != nullptr)
		{
			glAttachShader(ProgramObject, static_cast<OpenGLShader*>(VertexShader)->ShaderObject);
		}
		if (PixelShader != nullptr)
		{
			glAttachShader(ProgramObject, static_cast<OpenGLShader*>(PixelShader)->ShaderObject);
		}
		if (GeometryShader != nullptr)
		{
			glAttachShader(ProgramObject, static_cast<OpenGLShader*>(GeometryShader)->ShaderObject);
		}
		if (ComputeShader != nullptr)
		{
			glAttachShader(ProgramObject, static_cast<OpenGLShader*>(ComputeShader)->ShaderObject);
		}
		
		glLinkProgram(ProgramObject);

		int Success;
		char InfoBuffer[512];
		glGetProgramiv(ProgramObject, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			glGetProgramInfoLog(ProgramObject, 512, NULL, InfoBuffer);
			SCAR_LOG(LogError, "%s", InfoBuffer);
			check(false);
		}
	}
}