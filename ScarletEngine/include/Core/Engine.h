#pragma once

namespace ScarletEngine
{
	class Engine
	{
	public:
		void Initialize();
		void Run();

		void SignalQuit() { bIsRunning = false; }

		void Update(double DeltaTime) const;
		void FixedUpdate(double DeltaTime) const;

		/** Return a reference to the global Engine object */
		static Engine& Get() { static Engine Instance; return Instance; }
	private:
        void Terminate();

	private:
		// Prevent instantiations of this class and disable copy/move constructors
		Engine() = default;
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;

		bool bIsInitialized = false;
		bool bIsRunning = false;
	};
}