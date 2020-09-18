#include "Core/Engine.h"

#include "Core/CoreUtils.h"
#include "Core/ITickable.h"
#include "Renderer/Renderer.h"

#include <chrono>
#include <algorithm>
#ifdef RAL_USE_OPENGL
// #todo: move imgui out
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#endif

#define FIXED_UPDATE_MS 20.0

namespace ScarletEngine
{
	Engine::Engine()
		: VariableUpdateTickables()
		, FixedUpdateTickables()
		, TickableQueue()
		, bIsInitialized(false)
		, bIsRunning(false)
		, bIsTerminated(false)
		, bTickingObjects(false)
	{

	}

	void Engine::Initialize()
	{
		Logger::Get().SetLogFile("Log.txt");

		Renderer::Get().Initialize();

		// #todo: move ImGUI out of Engine initialization
#ifdef RAL_USE_OPENGL
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& IO = ImGui::GetIO();
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		IO.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
		IO.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
		// Enable floating windows
		//IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		IO.Fonts->AddFontFromFileTTF("../ScarletEngine/content/OpenSans-Regular.ttf", 18.f);

		ImGui::StyleColorsDark();

		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
		colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
		colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
		colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
		colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

		style->ChildRounding = 4.0f;
		style->FrameBorderSize = 1.0f;
		style->FrameRounding = 2.0f;
		style->GrabMinSize = 7.0f;
		style->PopupRounding = 2.0f;
		style->ScrollbarRounding = 12.0f;
		style->ScrollbarSize = 13.0f;
		style->TabBorderSize = 1.0f;
		style->TabRounding = 0.0f;
		style->WindowRounding = 4.0f;

		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Renderer::Get().GetWindowPtr(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
#endif

		bIsInitialized = true;
	}

	void Engine::Run()
	{
		check(bIsInitialized);

		std::chrono::high_resolution_clock Clock;

		using ms = std::chrono::duration<double, std::milli>;
		auto LastTime = Clock.now();
		double Lag = 0.0;

		bIsRunning = true;
		while (bIsRunning)
		{
			const double DeltaTime = std::chrono::duration_cast<ms>(Clock.now() - LastTime).count();
			LastTime = Clock.now();
			Lag += DeltaTime;

			PreUpdate();
			// In this scope we are ticking objects
			{
				bTickingObjects = true;
				// If it has been more than FIXED_UPDATE_MS since our last fixed timestep update,
				// we should run a fixed update before running a regular update
				while (Lag >= FIXED_UPDATE_MS)
				{
					Lag -= FIXED_UPDATE_MS;
					FixedUpdate(FIXED_UPDATE_MS);
				}

				Update(DeltaTime);
				bTickingObjects = false;
			}

			// Process anything that should happen before the next update
			PostUpdate();
		}

		Terminate();
	}

	void Engine::PreUpdate()
	{
		AddQueuedTickables();

		// #todo: move ImGUI out of Engine initialization
#ifdef RAL_USE_OPENGL
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
#endif
	}

	void Engine::PostUpdate()
	{
		// #todo: move ImGUI out of Engine initialization
#ifdef RAL_USE_OPENGL
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			void* BackupCurrentContext = Renderer::Get().GetWindowPtr();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			Renderer::Get().SetWindowCtx(BackupCurrentContext);
		}
#endif
		Renderer::Get().EndFrame();
	}

	void Engine::Terminate()
	{
		bIsInitialized = false;
		VariableUpdateTickables.clear();
		FixedUpdateTickables.clear();

		bIsTerminated = true;
	}

	void Engine::Update(double DeltaTime)
	{
		for (const auto Tickable : VariableUpdateTickables)
		{
			Tickable->Tick(DeltaTime);
		}
	}

	void Engine::FixedUpdate(double DeltaTime)
	{
		for (const auto Tickable : FixedUpdateTickables)
		{
			Tickable->Tick(DeltaTime);
		}
	}

	void Engine::QueueAddTickable(ITickable* TickableObject)
	{
		//std::lock_guard<std::mutex> Lock(TickableQueueMutex);

		TickableQueue.push_back(TickableObject);
	}

	void Engine::RemoveTickable(ITickable* TickableObject)
	{
		// Tickables cannot be removed while objects are being ticked as this would invalidate the iterator
		check(!bTickingObjects);

		if (TickableObject->WantsFixedTimestep())
		{
			auto It = std::remove(FixedUpdateTickables.begin(), FixedUpdateTickables.end(), TickableObject);
			if (It != FixedUpdateTickables.end())
			{
				FixedUpdateTickables.erase(It, FixedUpdateTickables.end());
			}
		}
		else
		{
			auto It = std::remove(VariableUpdateTickables.begin(), VariableUpdateTickables.end(), TickableObject);
			if (It != VariableUpdateTickables.end())
			{
				VariableUpdateTickables.erase(It, VariableUpdateTickables.end());
			}
		}
	}

	void Engine::AddQueuedTickables()
	{
		//std::lock_guard<std::mutex> Lock(TickableQueueMutex);
		// Add any new tickables which may have been enqueued during the last frame
		for (const auto TickableToAdd : TickableQueue)
		{
			AddTickable(TickableToAdd);
		}
		TickableQueue.clear();
	}

	void Engine::AddTickable(ITickable* TickableObject)
	{
		check(bIsInitialized);
		check(!bTickingObjects);

		TickableObject->Initialize();

		if (TickableObject->WantsFixedTimestep())
		{
			FixedUpdateTickables.push_back(TickableObject);
		}
		else
		{
			VariableUpdateTickables.push_back(TickableObject);
		}
	}
}