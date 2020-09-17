#pragma once

#include "Core/Core.h"
#include "ECS/ECS.h"

#include <memory>
#include <vector>

namespace ScarletEngine
{
	class World : public ITickable
	{
	public:
		World();

		void Tick(double DeltaTime);

		//inline double GetDeltaTime() const { return LastDeltaTime; }
	public:
		/** Register a new system with the world */
		template <typename... SystemSig>
		void AddSystem(const typename System<SystemSig...>::ForEachFunctionType ForEach)
		{
			Systems.emplace_back(new System<SystemSig...>(&Reg, ForEach));
		}

		Registry& GetRegistry() { return Reg; }
	private:
		void RunSystems()
		{
			for (const auto& Sys : Systems)
			{
				for (EID EntityID : Reg.GetEntities())
				{
					Sys->Run(EntityID);
				}
			}
		}
	private:
		double LastDeltaTime;
		Registry Reg;
		std::vector<std::unique_ptr<ISystem>> Systems;
	};
}
