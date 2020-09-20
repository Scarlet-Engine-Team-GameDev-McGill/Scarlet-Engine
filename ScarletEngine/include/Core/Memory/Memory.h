#pragma once

#include "GlobalAllocator.h"

namespace ScarletEngine
{
	template <typename T>
	struct GlobalDeleter
	{
		void operator()(T* Ptr)
		{
			GlobalAllocator::Free(Ptr);
		}
	};

	template <typename T>
	using UniquePtr = std::unique_ptr<T, GlobalDeleter<T>>;

	template <typename T>
	using SharedPtr = std::shared_ptr<T>;

	template <typename T>
	using WeakPtr = std::weak_ptr<T>;

	template <typename T, typename... Args>
	SharedPtr<T> MakeShared(Args&&... args)
	{
		return SharedPtr<T>(GlobalAllocator::Alloc<T>(std::forward<Args>(args)...), GlobalDeleter<T>());
	}
}