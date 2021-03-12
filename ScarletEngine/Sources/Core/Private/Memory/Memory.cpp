#include "Memory/Memory.h"

namespace ScarletEngine
{
	NODISCARD void* ScarMalloc(size_t Size)
	{
		void* Ptr = std::malloc(Size);
		check(Ptr != nullptr);
		MemoryTracker::Get().MarkAlloc(Ptr, Size);
		TracyAlloc(Ptr, Size);
		return Ptr;
	}

	void ScarFree(void* Ptr)
	{
		MemoryTracker::Get().RemoveAlloc(Ptr);
		TracyFree(Ptr);
		std::free(Ptr);
	}

	void _ScarDelete(void* Ptr)
	{
		check(Ptr != nullptr);
		MemoryTracker::Get().RemoveAlloc(Ptr);
		TracyFree(Ptr);
		delete Ptr;
	}

}