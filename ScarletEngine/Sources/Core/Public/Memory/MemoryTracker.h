#pragma once

#include "CoreUtils.h"
#include "CorePCH.h"

namespace ScarletEngine
{
	struct AllocationInfo
	{
		size_t Size;
	};

	class MemoryTracker
	{
	public:
		/** Mark an allocation event */
		void MarkAlloc(void* Ptr, size_t Size)
		{
			Allocs[Ptr] = AllocationInfo{ Size };
			MemUsed += Size;
		}

		/** Remove a tracked allocation */
		void RemoveAlloc(void* Ptr)
		{
			if (auto It = Allocs.find(Ptr); It != Allocs.end())
			{
				MemUsed -= It->second.Size;
				Allocs.erase(It);
			}
			else if (Ptr != nullptr)
			{
				// Attempting to delete a pointer which is not tracked by the application
				check(false);
			}
		}

		/** Returns the total number of active allocations */
		size_t GetNumAllocs() const { return Allocs.size(); }
		/** Returns the amount of memory currently used in bytes */
		size_t GetMemUsed() const { return MemUsed; }

		static MemoryTracker& Get() { static MemoryTracker Instance; return Instance; }
	private:
		// We are using a regular STL unordered map to prevent self-referencing since all Scarlet containers are tracked by default.
		std::unordered_map<void*, AllocationInfo> Allocs;
		size_t MemUsed = 0;
	};
}