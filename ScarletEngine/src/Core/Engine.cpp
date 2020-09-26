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
		ImFontConfig Config;
		Config.OversampleH = 4;
		Config.OversampleV = 4;
		Config.GlyphOffset.y -= 1.0f;
		IO.Fonts->AddFontFromFileTTF("../ScarletEngine/content/OpenSans-Regular.ttf", 18.0f, &Config);

		Config.MergeMode = true;
		Config.GlyphOffset.y -= -4.f;
		const ImWchar MDIconRanges[] = { ICON_MIN_MD, ICON_MAX_MD, 0 };
		IO.Fonts->AddFontFromFileTTF("../ScarletEngine/content/MaterialIcons-Regular.ttf", 18.0f, &Config, MDIconRanges);
		IO.Fonts->Build();

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		//style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		//style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.129f, 0.141f, 0.149f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.129f, 0.141f, 0.149f, 1.0F);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		//style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.129f, 0.141f, 0.149f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
		style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		style.GrabRounding = style.FrameRounding = 2.3f;
		style.WindowRounding = 0.f;
		style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.TabRounding = 0.f;
		style.FramePadding.x = 8.f;

		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)RAL::Get().GetWindowPtr(), true);
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