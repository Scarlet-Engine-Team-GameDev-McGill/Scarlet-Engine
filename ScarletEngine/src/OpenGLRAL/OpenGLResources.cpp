#include "OpenGLRAL/OpenGLResources.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ScarletEngine
{
	OpenGLFramebuffer::OpenGLFramebuffer(uint32_t InWidth, uint32_t InHeight, uint32_t InSamples)
		: RALFramebuffer(InWidth, InHeight, InSamples)
		, FramebufferObject(0)
		, ColorAttachment(0)
		, DepthAttachment(0)
	{
		RecreateResource();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &FramebufferObject);
		uint32_t Textures[] = { ColorAttachment, DepthAttachment };
		glDeleteTextures(2, Textures);
	}

	void OpenGLFramebuffer::RecreateResource()
	{
		if (FramebufferObject)
		{
			glDeleteFramebuffers(1, &FramebufferObject);
			uint32_t Textures[] = { ColorAttachment, DepthAttachment };
			glDeleteTextures(2, Textures);
		}

		glCreateFramebuffers(1, &FramebufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferObject);

		// Create the color attachment texture for the framebuffer
		glCreateTextures(GL_TEXTURE_2D, 1, &ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Create the depth attachment texture for the framebuffer
		glCreateTextures(GL_TEXTURE_2D, 1, &DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, Width, Height);

		// Bind the textures to the framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachment, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment, 0);

		// Ensure the framebuffer is complete
		check(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferObject);
		glViewport(0, 0, Width, Height);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t NewWidth, uint32_t NewHeight)
	{
		Width = NewWidth;
		Height = NewHeight;
	}

	OpenGLTexture2D::OpenGLTexture2D(const WeakPtr<TextureHandle>& InAssetHandle)
		: RALTexture2D(InAssetHandle)
		, TextureObject(0)
	{
		check(!InAssetHandle.expired());
		SharedPtr<TextureHandle> TexHandle = AssetHandle.lock();
		check(TexHandle->PixelDataBuffer != nullptr);
		
		glGenTextures(1, &TextureObject);
		glBindTexture(GL_TEXTURE_2D, TextureObject);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TexHandle->Width, TexHandle->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, AssetHandle.lock()->PixelDataBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &TextureObject);
	}

	void OpenGLTexture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, TextureObject);
	}

	void OpenGLTexture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t InSize, uint32_t InUsage)
		: RALVertexBuffer(InSize, InUsage)
		, BufferObject(0)
	{
		glGenBuffers(1, &BufferObject);
	}

	void OpenGLVertexBuffer::UploadData(void* DataPtr, size_t InSize) const
	{
		check(InSize <= Size);
		(void)InSize;
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

		glShaderBinary(1, &ShaderObject, GL_SHADER_BINARY_FORMAT_SPIR_V, ShaderCode.data(), static_cast<uint32_t>(ShaderCode.size()));
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