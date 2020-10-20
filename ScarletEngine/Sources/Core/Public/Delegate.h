#pragma once

#include "CorePCH.h"

namespace ScarletEngine
{
	template <typename>
	class Delegate;

	/** Bind a function callback to a delegate object */
	template<typename R, typename... Args>
	class Delegate<R(Args...)>
	{
		using FunctionType = std::function<R(Args...)>;
	public:
		void Bind(const FunctionType& InCallback)
		{
			ZoneScoped
			Callback = InCallback;
		}

		bool IsBound() const
		{
			ZoneScoped
			return Callback.has_value();
		}

		R operator()(Args... args) const
		{
			ZoneScoped
			check(IsBound());
			// prevent returning void if R is void type
			if constexpr (std::is_void_v<R>)
			{
				(*Callback)(args...);
			}
			else
			{
				return (*Callback)(args...);
			}
		}
	private:
		std::optional<FunctionType> Callback;
	};
}