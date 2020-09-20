#pragma once

#include "Core/CoreUtils.h"
#include "Core/Logger.h"
#include "Core/CorePCH.h"

namespace ScarletEngine
{
	struct AllocationInfo
	{
		size_t Size;
	};

	class GlobalAllocator
	{
	public:
		template <typename T, typename... Args>
		static T* Alloc(Args&&... args)
		{
			T* Ptr = new T(std::forward<Args>(args)...);
			Allocs[(void*)Ptr] = AllocationInfo{ sizeof(T) };
			MemUsed += sizeof(T);
			return Ptr;
		}

		template <typename T>
		static void Free(T* Ptr)
		{
			Allocs.erase((void*)Ptr);
			MemUsed -= sizeof(T);
			delete Ptr;
		}

		static size_t GetNumAllocs() { return Allocs.size(); }
		static size_t GetMemUsed() { return MemUsed; }
	private:
		static std::unordered_map<void*, AllocationInfo> Allocs;
		static size_t MemUsed;
	};
}