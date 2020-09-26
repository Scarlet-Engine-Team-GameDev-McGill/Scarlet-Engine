#pragma once

#include "MemoryTracker.h"
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

		[[nodiscard]] T* allocate(size_t Number) 
		{
			T* Ptr = static_cast<T*>(std::malloc(Number * sizeof(T)));
			check(Ptr != nullptr);
			MemoryTracker::MarkAlloc(Ptr, Number * sizeof(T));
			return Ptr;
		}

		void deallocate(T* Ptr, size_t Number)
		{
			check(Ptr != nullptr);
			MemoryTracker::RemoveAlloc(Ptr, Number * sizeof(T));
			std::free(Ptr);
		}

		template <typename... Args>
		[[nodiscard]] static T* New(Args&&... args)
		{
			T* Ptr = new T(std::forward<Args>(args)...);
			check(Ptr != nullptr);
			MemoryTracker::MarkAlloc(Ptr, sizeof(T));
			return Ptr;
		}
	
		inline bool operator==(const GlobalAllocator&) { return true; }
		inline bool operator!=(const GlobalAllocator&) { return true; }

		struct Delete
		{
			void operator()(T* Ptr)
			{
				check(Ptr != nullptr);
				MemoryTracker::RemoveAlloc(Ptr, sizeof(T));
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