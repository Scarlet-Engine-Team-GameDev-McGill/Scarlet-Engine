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
        uint8_t Data = (Val ? 1 : 0);
        Write(&Data, sizeof(uint8_t));
    }

    void BinaryArchive::WriteChar(unsigned char Val)
    {
        Write(&Val, sizeof(uint8_t));
    }

    void BinaryArchive::WriteInt(uint64_t Val)
    {
        Write(reinterpret_cast<const byte_t*>(&Val), sizeof(uint64_t));
    }

    void BinaryArchive::WriteFloat(double Val)
    {
         Write(reinterpret_cast<const byte_t*>(&Val), sizeof(double));
    }

    void BinaryArchive::WriteString(const char* Str, size_t Length)
    {
        Write(reinterpret_cast<const byte_t*>(Str), sizeof(char) * Length);
    }

    void BinaryArchive::ReadBool(bool& OutVal)
    {
        uint8_t Temp;
        Read(&Temp, sizeof(uint8_t));
        OutVal = Temp == 1;
    }

    void BinaryArchive::ReadChar(unsigned char& OutVal)
    {
        Read(&OutVal, sizeof(uint8_t));
    }

    void BinaryArchive::ReadInt(uint64_t& OutVal)
    {
        Read(reinterpret_cast<byte_t*>(&OutVal), sizeof(uint64_t));
    }

    void BinaryArchive::ReadFloat(double& OutVal)
    {
        Read(reinterpret_cast<byte_t*>(&OutVal), sizeof(double));
    }

    void BinaryArchive::ReadString(char* Str, size_t Length)
    {
        Read(reinterpret_cast<byte_t*>(Str), sizeof(char) * Length);
    }

    void BinaryArchive::Write(const byte_t* Data, size_t Size)
    {
        check(Mode == ArchiveMode::Write);
        const size_t NewPos = Pos + Size;
        if (NewPos > DataArray.size())
        {
            DataArray.resize(NewPos * 2);
        }
        memcpy(reinterpret_cast<void*>((uint64_t)DataArray.data() + (uint64_t)Pos), Data, Size);
        Pos = NewPos;
    }

    void BinaryArchive::Read(byte_t* OutData, size_t Size)
    {
        check(Mode == ArchiveMode::Read);
        check(Pos + Size <= DataArray.size()); // Attempting to read past the end of the archive.
        memcpy(OutData, reinterpret_cast<void*>((uint64_t)DataArray.data() + (uint64_t)Pos), Size);
        Pos += Size;
    }
}