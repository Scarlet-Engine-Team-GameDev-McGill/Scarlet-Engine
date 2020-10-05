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
		void MarkAlloc(void* Ptr, size_t Size)
		{
			Allocs[Ptr] = AllocationInfo{ Size };
			MemUsed += Size;
		}

		void RemoveAlloc(void* Ptr, size_t Size)
		{
			ZoneScoped
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

		size_t GetNumAllocs() const { return Allocs.size(); }
		size_t GetMemUsed() const { return MemUsed; }

		static MemoryTracker& Get() { static MemoryTracker Instance; return Instance; }
	private:
		// Using a regular std::unordered map to prevent tracking memory used by this
		std::unordered_map<void*, AllocationInfo> Allocs;
		size_t MemUsed;
	};
}