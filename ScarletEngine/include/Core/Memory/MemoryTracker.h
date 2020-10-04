#pragma once

#include "Core/CoreUtils.h"
#include "Core/CorePCH.h"

namespace ScarletEngine
{
	struct AllocationInfo
	{
		size_t Size;
	};

	class MemoryTracker
	{
	public:
		static void MarkAlloc(void* Ptr, size_t Size)
		{
			Allocs[Ptr] = AllocationInfo{ Size };
			MemUsed += Size;
		}

		static void RemoveAlloc(void* Ptr, size_t Size)
		{
			if (Allocs.find(Ptr) != Allocs.end())
			{
				Allocs.erase(Ptr);
				MemUsed -= Size;
			}
			else
			{
				// Attempting to delete a pointer which is not tracked by the application
				check(false);
			}
		}

		static size_t GetNumAllocs() { return Allocs.size(); }
		static size_t GetMemUsed() { return MemUsed; }
	private:
		// Using a regular std::unordered map to prevent tracking memory used by this
		static std::unordered_map<void*, AllocationInfo> Allocs;
		static size_t MemUsed;
	};
}