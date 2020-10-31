#include "Utils/StringUtils.h"

namespace ScarletEngine
{
	static String UnitsArr[] = { "B", "kB", "MB", "GB", "TB" };

	namespace StringUtils
	{
		String CreateBytesString(size_t Size)
		{
			ZoneScoped
			if (Size == 0)
			{
				return "0 B";
			}
			
			// Take the log base 1024 of the Size (using change of base for logarithm)
			uint32_t Units = (uint32_t)(std::log10(Size) / std::log10(1024));

			// Always at most 8 characters, + 1 for null byte
			// "xx.xx yy"
			char Buffer[9];
			snprintf(Buffer, 9, "%.2f %s", (Size / std::pow(1024, Units)), UnitsArr[Units].c_str());
			return String(Buffer);
		}

		void PathConcat(String& OutResult, const String& Other)
		{
			if (OutResult.back() == '/' && Other.front() == '/')
			{
				OutResult.replace(OutResult.size() - 1, Other.size(), Other);
			}
			else if (OutResult.back() != '/' && Other.front() != '/')
			{
				OutResult.append("/").append(Other);
			}
			else
			{
				OutResult.append(Other);
			}
		}
	}
}