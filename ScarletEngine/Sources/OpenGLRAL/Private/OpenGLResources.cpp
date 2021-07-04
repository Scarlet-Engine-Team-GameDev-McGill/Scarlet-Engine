#include "OpenGLResources.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

#include "AssetManager.h"
#include "RAL.h"

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

		glGenFramebuffers(1, &FramebufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferObject);

		// Create the color attachment texture for the framebuffer
		glGenTextures(1, &ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Create the depth attachment texture for the framebuffer
		glGenTextures(1, &DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, DepthAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, Width, Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

		// Bind the textures to the framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachment, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment, 0);
		

		// Ensure the framebuffer is complete
		check(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind() const
	{
		RAL::Get().QueueCommand([this](RALCommandList&)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, FramebufferObject);
			glViewport(0, 0, Width, Height);
		}, "BindFramebuffer");
	}

	void OpenGLFramebuffer::Unbind() const
	{
		RAL::Get().QueueCommand([](RALCommandList&)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}, "UnbindFramebuffer");
	}

	void OpenGLFramebuffer::Resize(uint32_t NewWidth, uint32_t NewHeight)
	{
		Width = NewWidth;
		Height = NewHeight;
		
		RecreateResource();
	}

	OpenGLTexture2D::OpenGLTexture2D(const WeakPtr<TextureHandle>& InAssetHandle)
		: RALTexture2D(InAssetHandle)
		, TextureObject(0)
	{
		check(!InAssetHandle.expired());
		const SharedPtr<TextureHandle> TexHandle = AssetHandle.lock();
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

	OpenGLGpuBuffer::OpenGLGpuBuffer(uint32_t InSize, RALBufferType InType, RALBufferUsage InUsage,
									 RALBufferPropertyFlags InProperties)
		: RALGpuBuffer(InSize, InType, InUsage, InProperties)
		, BufferObject(0)
	{
		glGenBuffers(1, &BufferObject);
	}

	void OpenGLGpuBuffer::UploadData(void* DataPtr, size_t InSize) const
	{
		check(InSize <= Size);
		(void)InSize;
		glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
		
		uint32_t GLUsage;
		switch (Usage)
		{
		case RALBufferUsage::STATIC_DRAW:
			GLUsage = GL_STATIC_DRAW;
			break;
		default:
			check(false); // unsupported usage
			GLUsage = 0;
			break;
		}
		glBufferData(GL_ARRAY_BUFFER, Size, DataPtr, GLUsage);
	}

	void OpenGLGpuBuffer::Release()
	{
		glDeleteBuffers(1, &BufferObject);
		BufferObject = 0;
		Usage = RALBufferUsage::INVALID;
	}

	OpenGLVertexArray::OpenGLVertexArray(const RALGpuBuffer* VB, const RALGpuBuffer* IB)
		: RALVertexArray(VB, IB)
	{
		auto OpenGLVB = static_cast<const OpenGLGpuBuffer*>(VB);
		auto OpenGLIB = static_cast<const OpenGLGpuBuffer*>(IB);

		glGenVertexArrays(1, &VAObject);
		glBindVertexArray(VAObject);
		glBindBuffer(GL_ARRAY_BUFFER, OpenGLVB->BufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OpenGLIB->BufferObject);

		// #todo_rendering: support more complex vertex attributes
		// for now the layout is:
		// vec3 vertex_pos
		// vec3 normals
		// vec2 uvs
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, VertexPos));
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(VAObject);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::Release()
	{
		glDeleteVertexArrays(1, &VAObject);
	}

	static String GetShaderCode(const String& ShaderFilePath)
	{
		std::ifstream ShaderFile(AssetManager::ToFullPath(ShaderFilePath).c_str());
		if (!ShaderFile.is_open())
		{
			SCAR_LOG(LogError, "Could not open shader file: %s", ShaderFilePath.c_str());
			return "";
		}

		std::basic_stringstream<char, std::char_traits<char>, GlobalAllocator<char>> ShaderStream;
		ShaderStream << ShaderFile.rdbuf();
		ShaderFile.close();

		return String(ShaderStream.str().c_str());
	}

	OpenGLShader::OpenGLShader(RALShaderStage Stage, const String& ShaderPath)
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

		const String Code = GetShaderCode(ShaderPath);
		const char* CodePtr = Code.c_str();

		glShaderSource(ShaderObject, 1, &CodePtr, nullptr);
		glCompileShader(ShaderObject);

		int Success;
		glGetShaderiv(ShaderObject, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			char InfoBuffer[512];
			glGetShaderInfoLog(ShaderObject, 512, nullptr, InfoBuffer);
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
		glGetProgramiv(ProgramObject, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			char InfoBuffer[512];
			glGetProgramInfoLog(ProgramObject, 512, nullptr, InfoBuffer);
			SCAR_LOG(LogError, "%s", InfoBuffer);
			check(false);
		}
	}

	void OpenGLShaderProgram::Bind() const
	{
		RAL::Get().QueueCommand([this](RALCommandList&)
		{
			glUseProgram(ProgramObject);
		}, "BindShaderProgram");
	}

	void OpenGLShaderProgram::Unbind() const
	{
		RAL::Get().QueueCommand([](RALCommandList&)
		{
			glUseProgram(0);
		}, "UnbindShaderProgram");
	}

	void OpenGLShaderProgram::SetUniformMat4(const glm::mat4& Mat, const char* Binding) const
	{
		RAL::Get().QueueCommand([this, Binding, Mat](RALCommandList&)
		{
			glUniformMatrix4fv(glGetUniformLocation(ProgramObject, Binding), 1, GL_FALSE, &Mat[0][0]);
		}, "SetUniformMat4");
	}

	void OpenGLShaderProgram::SetUniformVec3(const glm::vec3& Vec, const char* Binding) const
	{
		RAL::Get().QueueCommand([this, Binding, Vec](RALCommandList&)
		{
			glUniform3fv(glGetUniformLocation(ProgramObject, Binding), 1, &Vec.x);
		}, "SetUniformVec3");
	}
}
