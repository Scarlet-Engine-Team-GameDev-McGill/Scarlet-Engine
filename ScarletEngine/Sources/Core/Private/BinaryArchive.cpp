#include "BinaryArchive.h"

#include <fstream>

namespace ScarletEngine
{
	BinaryArchive::BinaryArchive()
		: Filename()
		, DataArray()
		, Pos(0)
		, Mode(ArchiveMode::Write)
	{
		DataArray.resize(128);
	}

	BinaryArchive::BinaryArchive(const String& InFilename, ArchiveMode InMode)
		: Filename(InFilename)
		, DataArray()
		, Pos(0)
		, Mode(InMode)
	{
		if (!Filename.empty())
		{
			if (InMode == ArchiveMode::Read)
			{
				std::ifstream File(Filename.c_str(), std::ios::binary | std::ios::ate);
				if (File.is_open())
				{
					size_t Size = (size_t)File.tellg();
					File.seekg(0);
					DataArray.resize(Size);
					File.read((char*)DataArray.data(), Size);
					File.close();
				}
			}
		}
	}

	void BinaryArchive::Close()
	{
		if (Mode == ArchiveMode::Write && !Filename.empty())
		{
			SaveToFile();
		}
		DataArray.clear();
	}

	bool BinaryArchive::SaveToFile(const char* OverrideFile)
	{
		if (Pos > 0)
		{
			// trunc to immediately remove the old file contents
			std::ofstream File(OverrideFile != nullptr ? OverrideFile : Filename.c_str(), std::ios::binary | std::ios::trunc | std::ios::out);
			if (File.is_open())
			{
				File.write(reinterpret_cast<const char*>(DataArray.data()), static_cast<std::streamsize>(Pos));
				File.close();
				return true;
			}
		}
		
		return false;
	}

}