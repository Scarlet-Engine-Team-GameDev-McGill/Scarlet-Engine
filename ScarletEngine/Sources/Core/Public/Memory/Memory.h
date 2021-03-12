#pragma once

#include "MemoryTracker.h"

namespace ScarletEngine
{
	NODISCARD void* ScarMalloc(size_t Size);

	void ScarFree(void* Ptr);

	template <typename T, typename ... Args>
	NODISCARD inline T* _ScarNew(Args&&... args)
	{
		T* Ptr = new T(std::forward<Args>(args)...);
		check(Ptr != nullptr);
		MemoryTracker::Get().MarkAlloc(Ptr, sizeof(T));
		TracyAlloc(Ptr, sizeof(T));
		return Ptr;
	}

	void _ScarDelete(void* Ptr);
}

#define ScarNew(TypeName, ...) ScarletEngine::_ScarNew<TypeName>(__VA_ARGS__)
#define ScarDelete(Ptr) ScarletEngine::_ScarDelete(Ptr)

namespace ScarletEngine
{
	template <class T>
	struct GlobalAllocator
	{
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using value_type = T;

		inline GlobalAllocator() = default;
		inline GlobalAllocator(const GlobalAllocator&) {}
		template <class U> inline GlobalAllocator(const GlobalAllocator<U>&) {}

		NODISCARD inline T* allocate(size_t Number)
		{
			T* Ptr = static_cast<T*>(ScarMalloc(Number * sizeof(T)));
			return Ptr;
		}

		inline void deallocate(T* Ptr, size_t)
		{
			ScarFree(Ptr);
		}

		inline bool operator==(const GlobalAllocator&) const{ return true; }
		inline bool operator==(GlobalAllocator&) const { return true; }

		struct Delete
		{
			void operator()(T* Ptr)
			{
				ScarDelete(Ptr);
			}
		};
	};

	template <typename T>
	using UniquePtr = std::unique_ptr<T, typename GlobalAllocator<T>::Delete>;

	template <typename T>
	using SharedPtr = std::shared_ptr<T>;

	template <typename T>
	using WeakPtr = std::weak_ptr<T>;

	template <typename T, typename... Args>
	NODISCARD SharedPtr<T> MakeShared(Args&&... args)
	{
		return SharedPtr<T>(ScarNew(T, std::forward<Args>(args)...), typename GlobalAllocator<T>::Delete());
	}

	template <typename T, typename... Args>
	NODISCARD UniquePtr<T> MakeUnique(Args&&... args)
	{
		return UniquePtr<T>(ScarNew(T, std::forward<Args>(args)...));
	}
}