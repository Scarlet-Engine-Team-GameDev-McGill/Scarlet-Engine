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
			Callback = InCallback;
		}

		bool IsBound() const
		{
			return Callback.has_value();
		}

		R operator()(Args... args) const
		{
			check(IsBound());

			return (*Callback)(args...);
		}
	private:
		std::optional<FunctionType> Callback;
	};
}