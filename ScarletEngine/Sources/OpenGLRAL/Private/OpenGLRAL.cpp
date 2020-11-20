#include "OpenGLRAL.h"

#include "Core.h"
#include "Engine.h"
#include "OpenGLResources.h"
#include "AssetManager.h"

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

	void FramebufferResizeCallback(GLFWwindow*, int Width, int Height)
	{
		ZoneScoped
		glViewport(0, 0, Width, Height);
	}

	void WindowCloseCallback(GLFWwindow*)
	{
		ZoneScoped
		GEngine->SignalQuit();
	}

	void OpenGLRAL::Initialize()
	{
		ZoneScoped
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

		// #todo: remove hard coded window title
		Window = glfwCreateWindow(800, 600, "Scarlet Editor", nullptr, nullptr);
		if (Window == nullptr)
		{
			glfwTerminate();
			check(false);
		}

		// Set the window icon;
		SharedPtr<TextureHandle> LogoTex = AssetManager::LoadTextureFile("/ScarletEngine/Content/scarlet_logo.png");
		GLFWimage Image;
		Image.pixels = LogoTex->PixelDataBuffer;
		Image.width = LogoTex->Width;
		Image.height = LogoTex->Height;

		glfwSetWindowIcon(Window, 1, &Image);

		glfwMakeContextCurrent(Window);
		glfwSwapInterval(0);

		check(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
		glViewport(0, 0, 800, 600);
		glfwSetFramebufferSizeCallback(Window, FramebufferResizeCallback);
		glfwSetWindowCloseCallback(Window, WindowCloseCallback);

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

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
	}

	void OpenGLRAL::SetWindowCtx(void* WindowPtr)
	{
		ZoneScoped
		glfwMakeContextCurrent((GLFWwindow*)WindowPtr);
	}

	void OpenGLRAL::Terminate()
	{
		ZoneScoped
		glfwTerminate();
	}

	void OpenGLRAL::SwapWindowBuffers() const
	{
		ZoneScoped
		glfwSwapBuffers(Window);
		//TracyGpuCollect
	}

	void OpenGLRAL::PollWindowEvents() const
	{
		ZoneScoped
		glfwPollEvents();
	}

	GPUInfo OpenGLRAL::GetGPUInfo() const
	{
		GPUInfo Info{};
		Info.Vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		Info.Renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		Info.Version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		return Info;
	}

	void OpenGLRAL::SetClearColorCommand(const glm::vec4& ClearColor) const
	{
		ZoneScoped
		glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
	}

	void OpenGLRAL::ClearCommand(bool bColor, bool bDepth, bool bStencil) const
	{
		ZoneScoped
		GLbitfield ClearField = 0;
		if (bColor) ClearField |= GL_COLOR_BUFFER_BIT;
		if (bDepth) ClearField |= GL_DEPTH_BUFFER_BIT;
		if (bStencil) ClearField |= GL_STENCIL_BUFFER_BIT;

		{
			//TracyGpuZone("Clear");
			glClear(ClearField);
		}
	}

	void OpenGLRAL::DrawVertexArray(const RALVertexArray* VA) const
	{
		ZoneScoped
		VA->Bind();
		glDrawElements(GL_TRIANGLES, VA->IB->Size / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
		VA->Unbind();
	}

	RALFramebuffer* OpenGLRAL::CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples) const
	{
		ZoneScoped
		return GlobalAllocator<OpenGLFramebuffer>::New(Width, Height, Samples);
	}

	ScarletEngine::RALTexture2D* OpenGLRAL::CreateTexture2D(const WeakPtr<TextureHandle>& AssetHandle) const
	{
		ZoneScoped
		return GlobalAllocator<OpenGLTexture2D>::New(AssetHandle);
	}

	RALGpuBuffer* OpenGLRAL::CreateBuffer(uint32_t Size, RALBufferUsage Usage) const
	{
		ZoneScoped
		return GlobalAllocator<OpenGLGpuBuffer>::New(Size, Usage);
	}

	RALVertexArray* OpenGLRAL::CreateVertexArray(const RALGpuBuffer* VB, const RALGpuBuffer* IB) const
	{
		ZoneScoped
		return GlobalAllocator<OpenGLVertexArray>::New(VB, IB);
	}

	RALShader* OpenGLRAL::CreateShader(RALShaderStage Stage, const String& ShaderPath) const
	{
		ZoneScoped
		return GlobalAllocator<OpenGLShader>::New(Stage, ShaderPath);
	}

	RALShaderProgram* OpenGLRAL::CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) const
	{
		ZoneScoped
		return GlobalAllocator<OpenGLShaderProgram>::New(InVertexShader, InPixelShader, InGeometryShader, InComputeShader);
	}
}