#pragma once

#include <functional>
#include <vector>

namespace ScarletEngine
{
	/** Allows binding multiple function callbacks to a single event */
	template <typename... Args>
	class Event
	{
	public:
		void Bind(const std::function<void(Args...)>& Callback)
		{
			Callbacks.emplace_back(Callback);
		}

		void Broadcast(Args... args) const
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
		std::vector<std::function<void(Args...)>> Callbacks;
	};
}