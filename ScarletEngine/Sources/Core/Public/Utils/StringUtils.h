#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
	namespace StringUtils
	{
		/** Create a string representation of a byte-size. Will round to the nearest thousand and append the appropriate unit. */
		String CreateBytesString(size_t Size);

		/** Concatenates two paths ensuring that there is exactly one slash between them */
		String PathConcat(StringView First, StringView Second);
	}
}