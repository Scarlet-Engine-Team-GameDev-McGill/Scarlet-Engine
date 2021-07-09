#include "Engine.h"

#include "CoreUtils.h"

#include <chrono>
#include <common/TracySystem.hpp>

#include "InputManager.h"
#include "ModuleManager.h"
#include "TickableList.h"
#include "Window.h"
#include "EngineDelegates.h"
#include "Serialization/BinaryArchive.h"

namespace ScarletEngine
{
    Engine* GEngine = nullptr;

    // -----------------------------------------------------------------------------------------------------------------

    Engine::Engine()
    {
    }

    void Engine::Initialize()
    {
        ZoneScoped
        Logger::Get().SetLogFile("Log.txt");

        AppWindow = ScarNew(ApplicationWindow, 800, 600, "Scarlet Engine");
        check(AppWindow);

        AppWindow->OnWindowClose.BindMember(this, &Engine::SignalQuit);

        ModuleManager::GetInstance().Startup();

        // Create a default world
        ActiveWorld = MakeShared<World>();
        ActiveWorld->SetWorldName("Editor World");

        if (bStartGameplaySystemsOnLoad)
        {
            SystemScheduler::Get().EnableGameplaySystems();
        }

        bIsInitialized = true;
    }

    void Engine::Run()
    {
        ZoneScoped
        tracy::SetThreadName("Engine Thread");

        check(bIsInitialized);

        std::chrono::high_resolution_clock Clock;

        using seconds = std::chrono::duration<double, std::ratio<1>>;
        auto LastTime = Clock.now();
        double Lag = 0.0;

        bIsRunning = true;
        while (bIsRunning)
        {
            FrameMark

            const double DeltaTime = std::chrono::duration_cast<seconds>(Clock.now() - LastTime).count();
            LastTime = Clock.now();
            Lag += DeltaTime;

            PreUpdate();
            // In this scope we are ticking objects
            {
                bTickingObjects = true;
                // If it has been more than FIXED_UPDATE since our last fixed timestep update,
                // we should run a fixed update before running a regular update
                while (Lag >= FIXED_UPDATE_TIME)
                {
                    Lag -= FIXED_UPDATE_TIME;
                    FixedUpdate(FIXED_UPDATE_TIME);
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
        ZoneScoped

        AppWindow->PollEvents();

        TickableList::Get().AddQueuedTickables();

        ModuleManager::GetInstance().PreUpdate();
    }

    void Engine::PostUpdate()
    {
        ZoneScoped
        ModuleManager::GetInstance().PostUpdate();

        InputManager::Get().PostUpdate();

        if (QueuedWorldToLoad)
        {
            LoadWorld();
        }

        TickableList::Get().Cleanup();

        AppWindow->SwapBuffer();
    }

    void Engine::Terminate()
    {
        ZoneScoped
        SCAR_LOG(LogInfo, "Terminating engine");
        ModuleManager::GetInstance().Shutdown();

        ScarDelete(AppWindow);

        TickableList::Get().Reset();
        bIsInitialized = false;
        bIsTerminated = true;
    }

    void Engine::Update(double DeltaTime)
    {
        ZoneScoped
        ModuleManager::GetInstance().Update();

        TickableList::Get().Update(DeltaTime);
    }

    void Engine::FixedUpdate(double DeltaTime)
    {
        ZoneScoped
        TickableList::Get().FixedUpdate(DeltaTime);
    }

    void Engine::SaveWorld(const String& WorldPath) const
    {
        // #todo_core: change this to a json archive when ready
        BinaryArchive WorldArchive(WorldPath, ArchiveMode::Write);
        //WorldArchive << *ActiveWorld;
    }

    void Engine::LoadWorldFromFile(const String& WorldPath)
    {
        // #todo_core: change this to a json archive when ready
        BinaryArchive WorldArchive(WorldPath, ArchiveMode::Read);
        QueuedWorldToLoad = MakeShared<World>();
        QueuedWorldToLoad->SetWorldName(WorldPath);

        // #todo_core: for now copy the worlds registry to prevent having to re-register component types some other way
        QueuedWorldToLoad->GetRegistry() = std::move(ActiveWorld->GetRegistry());

        //WorldArchive >> *QueuedWorldToLoad;
    }

    void Engine::LoadWorld()
    {
        ActiveWorld.reset();
        ActiveWorld = QueuedWorldToLoad;

        EngineDelegates::OnWorldChanged.Broadcast(ActiveWorld);

        QueuedWorldToLoad = nullptr;
    }
}
