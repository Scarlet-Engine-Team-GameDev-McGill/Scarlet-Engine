#pragma once

#include "CorePCH.h"

namespace ScarletEngine
{
	template <typename>
	class Delegate;

	/** Delegate object which can be used to bind a single function callback to. */
	template<typename R, typename... Args>
	class Delegate<R(Args...)>
	{
		using FunctionType = std::function<R(Args...)>;
	public:
		/** Bind a callback function to this delegate */
		void Bind(const FunctionType& InCallback)
		{
			Callback = InCallback;
		}

		/** Check if this delegate has a bound callback */
		bool IsBound() const
		{
			return Callback.has_value();
		}

		R operator()(Args... args) const
		{
			return Execute(std::forward<Args>(args)...);
		}

		/** Execute the delegate. Checks if delegate is bound */
		R Execute(Args... args) const
		{
			check(IsBound());
			return (*Callback)(std::forward<Args>(args)...);
		}

	private:
		std::optional<FunctionType> Callback;
	};
}