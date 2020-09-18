#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
	/** Allows binding multiple function callbacks to a single event */
	template <typename... Args>
	class Event
	{
	public:
		void Bind(const std::function<void(Args...)>& Callback) const
		{
			// This method is not actually const, 
			// it just allows us to prevent const references from broadcasting
			Callbacks.emplace_back(Callback);
		}

		template <typename CallerType, typename FunctionType>
		void Bind(CallerType Ptr, FunctionType Func) const
		{
			if constexpr (sizeof...(Args) == 0)
			{
				Bind(std::bind(Func, Ptr));
			}
			else  if constexpr (sizeof...(Args) == 1)
			{
				Bind(std::bind(Func, Ptr, std::placeholders::_1));
			}
			else if constexpr (sizeof...(Args) == 2)
			{
				Bind(std::bind(Func, Ptr, std::placeholders::_1, std::placeholders::_2));
			}
			else if constexpr (sizeof...(Args) == 3)
			{
				Bind(std::bind(Func, Ptr, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			}
			else if constexpr (sizeof...(Args) == 4)
			{
				Bind(std::bind(Func, Ptr, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
			}
			else
			{
				static_assert(sizeof...(Args) <= 4, "Too many arguments for event! Only supports up to 4!");
			}
		}

		void Broadcast(Args... args)
		{
			for (const auto& Callback : Callbacks)
			{
				Callback(args...);
			}
		}

		void Clear()
		{
			Callbacks.clear();
		}
	private:
		mutable std::vector<std::function<void(Args...)>> Callbacks;
	};
}