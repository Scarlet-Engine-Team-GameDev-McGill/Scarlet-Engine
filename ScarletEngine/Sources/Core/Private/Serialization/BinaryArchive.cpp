#include "Serialization/BinaryArchive.h"

#include <fstream>

namespace ScarletEngine
{
	BinaryArchive::BinaryArchive(ArchiveMode InMode)
		: Archive(InMode)
		, DataArray()
		, Pos(0)
	{
		DataArray.resize(128);
	}

	BinaryArchive::BinaryArchive(const String& InFilename, ArchiveMode InMode)
		: Archive(InFilename, InMode)
		, DataArray()
		, Pos(0)
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
		DataArray.clear();
	}

	bool BinaryArchive::SaveToFile(const char* OverrideFile) const
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

	void BinaryArchive::WriteBool(bool Val)
	{

	}

	void BinaryArchive::WriteChar(unsigned char Val)
	{

	}

	void BinaryArchive::WriteInt(uint64_t Data, bool bIsSigned)
	{

	}

	void BinaryArchive::WriteFloat(double Data)
	{

	}

	void BinaryArchive::WriteString(const char* Str, size_t Length)
	{

	}	

	void BinaryArchive::ReadBool(bool& OutVal)
	{

	}

	void BinaryArchive::ReadChar(unsigned char& OutVal)
	{

	}

	void BinaryArchive::ReadInt(uint64_t& Data, bool bIsSigned)
	{

	}

	void BinaryArchive::ReadFloat(double& Data)
	{

	}

	void BinaryArchive::ReadString(char* Str, size_t Length)
	{

	}
}