#include "Core/Memory/GlobalAllocator.h"

namespace ScarletEngine
{
	std::unordered_map<void*, AllocationInfo> GlobalAllocator::Allocs;
	size_t GlobalAllocator::MemUsed = 0;
}