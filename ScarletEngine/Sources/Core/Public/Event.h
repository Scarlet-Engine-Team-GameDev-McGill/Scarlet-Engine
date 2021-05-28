#pragma once

#include "CoreSTL.h"

namespace ScarletEngine
{
	/** Bind multiple function callbacks to a single event */
	template <typename... Args>
	class Event
	{
		using FunctionType = std::function<void(Args...)>;
		struct CallbackData
		{
			void* CallbackOwner;
			FunctionType Func;
		};

	public:
		/**
		 * Bind a free function or lambda to the event 
		 * note: this function is not actually const. Marking it as such is just so that we can prevent const refs from broadcasting
		 */
		void Bind(const FunctionType& Callback, void* OwnerPtr = nullptr) const
		{
			Callbacks.emplace_back(OwnerPtr, Callback);
		}

		/**
		 * Bind a member function to the event
		 * note: this function is not actually const. Marking it as such is just so that we can prevent const refs from broadcasting
		 */
		template <typename CallerType, typename MemberFuncType>
		void BindMember(CallerType Ptr, MemberFuncType Func) const
		{
			if constexpr (sizeof...(Args) == 0)
			{
				Callbacks.emplace_back(Ptr, std::bind(Func, Ptr));
			}
			else  if constexpr (sizeof...(Args) == 1)
			{
				Callbacks.emplace_back(Ptr, std::bind(Func, Ptr, std::placeholders::_1));
			}
			else if constexpr (sizeof...(Args) == 2)
			{
				Callbacks.emplace_back(Ptr, std::bind(Func, Ptr, std::placeholders::_1, std::placeholders::_2));
			}
			else if constexpr (sizeof...(Args) == 3)
			{
				Callbacks.emplace_back(Ptr, std::bind(Func, Ptr, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			}
			else if constexpr (sizeof...(Args) == 4)
			{
				Callbacks.emplace_back(Ptr, std::bind(Func, Ptr, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
			}
			else
			{
				static_assert(sizeof...(Args) <= 4, "Too many arguments for event! Only supports up to 4!");
			}
		}

		void Unbind(void* Ptr) const
		{
			auto It = std::find_if(Callbacks.begin(), Callbacks.end(), [Ptr](const CallbackData& Data)
				{
					return Data.CallbackOwner == Ptr;
				});

			if (It != Callbacks.end())
			{
				Callbacks.erase(It);
			}
		}

		void Broadcast(Args... args)
		{
			for (const auto& CallbackInfo : Callbacks)
			{
				CallbackInfo.Func(args...);
			}
		}

		void Clear()
		{
			Callbacks.clear();
		}
	private:
		// #todo: This can be optimized to use a flat array of pure Functions
		//		  and store the metadata elsewhere
		mutable Array<CallbackData> Callbacks;
	};
}