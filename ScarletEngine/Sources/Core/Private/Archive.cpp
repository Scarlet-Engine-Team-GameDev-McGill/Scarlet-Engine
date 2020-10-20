#include "Archive.h"

#include <fstream>

namespace ScarletEngine
{
	Archive::Archive()
		: Filename()
		, DataArray()
		, Pos(0)
		, Mode(ArchiveMode::Write)
	{
		DataArray.resize(128);
	}

	Archive::Archive(const String& InFilename, ArchiveMode InMode)
		: Filename(InFilename)
		, DataArray()
		, Pos(0)
		, Mode(InMode)
	{
		ZoneScoped
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

	void Archive::Close()
	{
		ZoneScoped
		if (Mode == ArchiveMode::Write && !Filename.empty())
		{
			SaveToFile();
		}
		DataArray.clear();
	}

	bool Archive::SaveToFile(const char* OverrideFile)
	{
		ZoneScoped
		if (Pos > 0)
		{
			// trunc to immediately remove the old file contents
			std::ofstream File(OverrideFile != nullptr ? OverrideFile : Filename.c_str(), std::ios::binary | std::ios::trunc | std::ios::out);
			if (File.is_open())
			{
				File.write((const char*)DataArray.data(), (std::streamsize)Pos);
				File.close();
				return true;
			}
		}
		
		return false;
	}

}