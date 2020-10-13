#pragma once

#include "MemoryTracker.h"

namespace ScarletEngine
{
	[[nodiscard]] void* ScarMalloc(size_t Size);

	void ScarFree(void* Ptr);

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

		[[nodiscard]] inline T* allocate(size_t Number) 
		{
			T* Ptr = static_cast<T*>(ScarMalloc(Number * sizeof(T)));
			return Ptr;
		}

		inline void deallocate(T* Ptr, size_t)
		{
			ScarFree(Ptr);
		}

		template <typename... Args>
		[[nodiscard]] static inline T* New(Args&&... args)
		{
			T* Ptr = new T(std::forward<Args>(args)...);
			check(Ptr != nullptr);
			MemoryTracker::Get().MarkAlloc(Ptr, sizeof(T));
			TracyAlloc(Ptr, sizeof(T));
			return Ptr;
		}

		static inline void Free(T* Ptr)
		{
			check (Ptr != nullptr)
			MemoryTracker::Get().RemoveAlloc(Ptr, sizeof(T));
			TracyFree(Ptr);
			delete Ptr;
		}

		inline bool operator==(const GlobalAllocator&) { return true; }
		inline bool operator!=(const GlobalAllocator&) { return true; }

		struct Delete
		{
			void operator()(T* Ptr)
			{
				check(Ptr != nullptr);
				MemoryTracker::Get().RemoveAlloc(Ptr, sizeof(T));
				TracyFree(Ptr);
				delete Ptr;
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
	[[nodiscard]] SharedPtr<T> MakeShared(Args&&... args)
	{
		return SharedPtr<T>(GlobalAllocator<T>::New(std::forward<Args>(args)...), typename GlobalAllocator<T>::Delete());
	}

	template <typename T, typename... Args>
	[[nodiscard]] UniquePtr<T> MakeUnique(Args&&... args)
	{
		return UniquePtr<T>(GlobalAllocator<T>::New(std::forward<Args>(args)...));
	}
}