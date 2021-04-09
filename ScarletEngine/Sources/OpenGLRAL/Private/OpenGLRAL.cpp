#include "OpenGLRAL.h"

#include "Core.h"
#include "Engine.h"
#include "OpenGLResources.h"
#include "AssetManager.h"
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <TracyOpenGL.hpp>
#include <sstream>

namespace ScarletEngine
{
#ifdef DEBUG
	// Debug output taken from https://learnopengl.com
	static void APIENTRY glDebugOutput(GLenum source,
		GLenum type,
		unsigned int id,
		GLenum severity,
		GLsizei,
		const char* message,
		const void*)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::stringstream Message;
		Message << "---------------" << std::endl;
		Message << "Debug message (" << id << "): " << message << std::endl;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             Message << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   Message << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: Message << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     Message << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     Message << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           Message << "Source: Other"; break;
		} Message << std::endl;

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               Message << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: Message << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  Message << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         Message << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         Message << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              Message << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          Message << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           Message << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               Message << "Type: Other"; break;
		} Message << std::endl;

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         Message << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       Message << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          Message << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: Message << "Severity: notification"; break;
		} Message << std::endl;
		Message << std::endl;

		SCAR_LOG(LogError, "OGL Error: %s", Message.str().c_str());
	}
#endif

	static void FramebufferResizeCallback(uint32_t NewWidth, uint32_t NewHeight)
	{
		glViewport(0, 0, NewWidth, NewHeight);
	}

	void OpenGLRAL::Initialize()
	{
		ZoneScoped
		RAL::Initialize();

		ApplicationWindow* AppWindow = GEngine->GetApplicationWindow();
		GLFWwindow* WindowHandle = (GLFWwindow*)AppWindow->GetWindowHandle();

		glfwMakeContextCurrent(WindowHandle);
		glfwSwapInterval(0);

		check(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
		glViewport(0, 0, AppWindow->GetWidth(), AppWindow->GetHeight());

		AppWindow->OnWindowResize.Bind(FramebufferResizeCallback);

#ifdef DEBUG
		int Flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &Flags);
		if (Flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_TRUE);
		}
#endif

		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
	}

	void OpenGLRAL::Terminate()
	{
		ZoneScoped
		glfwTerminate();
	}

	void OpenGLRAL::Submit()
	{
		CommandListQueue.front().ExecuteAll();
		CommandListQueue.pop();

		CommandListQueue.push(RALCommandList{});
	}

	GPUInfo OpenGLRAL::GetGPUInfo() const
	{
		GPUInfo Info{};
		Info.Vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		Info.Renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		Info.Version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		return Info;
	}

	void OpenGLRAL::SetClearColorCmd(const glm::vec4& ClearColor)
	{
		ZoneScoped
		QueueCommand([ClearColor](RALCommandList&)
		{
			glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
		});
	}

	void OpenGLRAL::ClearCmd(bool bColor, bool bDepth, bool bStencil)
	{
		ZoneScoped
		QueueCommand([bColor, bDepth, bStencil](RALCommandList&)
		{
			GLbitfield ClearField = 0;
			if (bColor) ClearField |= GL_COLOR_BUFFER_BIT;
			if (bDepth) ClearField |= GL_DEPTH_BUFFER_BIT;
			if (bStencil) ClearField |= GL_STENCIL_BUFFER_BIT;

			glClear(ClearField);
		});
	}

	void OpenGLRAL::DrawVertexArrayCmd(const RALVertexArray* VA)
	{
		ZoneScoped
		VA->Bind();
		glDrawElements(GL_TRIANGLES, VA->IB->Size / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
		VA->Unbind();
	}

	RALFramebuffer* OpenGLRAL::CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples)
	{
		ZoneScoped
		return ScarNew(OpenGLFramebuffer, Width, Height, Samples);
	}

	ScarletEngine::RALTexture2D* OpenGLRAL::CreateTexture2D(const WeakPtr<TextureHandle>& AssetHandle)
	{
		ZoneScoped
		return ScarNew(OpenGLTexture2D, AssetHandle);
	}

	RALGpuBuffer* OpenGLRAL::CreateBuffer(uint32_t Size, RALBufferUsage Usage)
	{
		ZoneScoped
		return ScarNew(OpenGLGpuBuffer, Size, Usage);
	}

	RALVertexArray* OpenGLRAL::CreateVertexArray(const RALGpuBuffer* VB, const RALGpuBuffer* IB)
	{
		ZoneScoped
		return ScarNew(OpenGLVertexArray, VB, IB);
	}

	RALShader* OpenGLRAL::CreateShader(RALShaderStage Stage, const String& ShaderPath)
	{
		ZoneScoped
		return ScarNew(OpenGLShader, Stage, ShaderPath);
	}

	RALShaderProgram* OpenGLRAL::CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader)
	{
		ZoneScoped
		return ScarNew(OpenGLShaderProgram, InVertexShader, InPixelShader, InGeometryShader, InComputeShader);
	}
}