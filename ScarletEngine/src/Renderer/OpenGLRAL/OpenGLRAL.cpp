#include "Renderer/OpenGLRAL/OpenGLRAL.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Core/Core.h"
#include "Renderer/OpenGLRAL/OpenGLResources.h"

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

		Window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
		if (Window == nullptr)
		{
			glfwTerminate();
			check(false);
		}
		glfwMakeContextCurrent(Window);

		check(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
		glViewport(0, 0, 800, 600);
		glfwSetFramebufferSizeCallback(Window, FramebufferResizeCallback);
		glfwSetWindowCloseCallback(Window, WindowCloseCallback);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
		return nullptr;
	}
}