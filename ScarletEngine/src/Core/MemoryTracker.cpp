#include "Core/Memory/MemoryTracker.h"

namespace ScarletEngine
{
	std::unordered_map<void*, AllocationInfo> MemoryTracker::Allocs;
	size_t MemoryTracker::MemUsed = 0;
}