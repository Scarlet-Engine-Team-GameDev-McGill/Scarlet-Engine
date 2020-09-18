#include "Renderer/OpenGLRAL/OpenGLRAL.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Core/Core.h"
#include "Renderer/OpenGLRAL/OpenGLResources.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace ScarletEngine
{
	void FramebufferResizeCallback(GLFWwindow*, int Width, int Height)
	{
		glViewport(0, 0, Width, Height);
	}

	void WindowCloseCallback(GLFWwindow*)
	{
		Engine::Get().SignalQuit();
	}

	void OpenGLRAL::Initialize()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// #todo: remove hard coded window title
		Window = glfwCreateWindow(800, 600, "Scarlet Editor", nullptr, nullptr);
		if (Window == nullptr)
		{
			glfwTerminate();
			check(false);
		}

		// Set the window icon;
		GLFWimage Images[1]; 
		// #todo: use a global asset loader/manager to load from content
		Images[0].pixels = stbi_load("../ScarletEngine/content/scarlet_logo.png", &Images[0].width, &Images[0].height, 0, 4);
		glfwSetWindowIcon(Window, 1, Images);
		stbi_image_free(Images[0].pixels);

		glfwMakeContextCurrent(Window);
		glfwSwapInterval(1);

		check(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
		glViewport(0, 0, 800, 600);
		glfwSetFramebufferSizeCallback(Window, FramebufferResizeCallback);
		glfwSetWindowCloseCallback(Window, WindowCloseCallback);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	}

	void OpenGLRAL::SetWindowCtx(void* WindowPtr)
	{
		glfwMakeContextCurrent((GLFWwindow*)WindowPtr);
	}

	void OpenGLRAL::Terminate()
	{
		glfwTerminate();
	}

	void OpenGLRAL::SwapWindowBuffers() const
	{
		glfwSwapBuffers(Window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRAL::PollWindowEvents() const
	{
		glfwPollEvents();
	}

	void OpenGLRAL::SetClearColorCommand(const glm::vec4& ClearColor) const
	{
		glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
	}

	void OpenGLRAL::ClearCommand(bool bColor, bool bDepth, bool bStencil) const
	{
		GLbitfield ClearField = 0;
		if (bColor) ClearField |= GL_COLOR_BUFFER_BIT;
		if (bDepth) ClearField |= GL_DEPTH_BUFFER_BIT;
		if (bStencil) ClearField |= GL_STENCIL_BUFFER_BIT;

		glClear(ClearField);
	}

	RALFramebuffer* OpenGLRAL::CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples) const
	{
		return new OpenGLFramebuffer(Width, Height, Samples);
	}

	RALVertexBuffer* OpenGLRAL::CreateVertexBuffer(uint32_t Size, uint32_t Usage) const
	{
		return new OpenGLVertexBuffer(Size, Usage);
	}

	RALShader* OpenGLRAL::CreateShader(RALShaderStage Stage, const std::vector<uint8_t>& ShaderCode) const
	{
		return new OpenGLShader(Stage, ShaderCode);
	}

	RALShaderProgram* OpenGLRAL::CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) const
	{
		return new OpenGLShaderProgram(InVertexShader, InPixelShader, InGeometryShader, InComputeShader);
	}
}