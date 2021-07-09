#include "Serialization/JsonArchive.h"

#include <fstream>

namespace ScarletEngine
{
    JsonArchive::JsonArchive(ArchiveMode InMode)
        : Archive(InMode)
        , Data()
    {
    }

    JsonArchive::JsonArchive(const String& InFilename, ArchiveMode InMode)
        : Archive(InFilename, InMode)
        , Data()
    {
        if (!Filename.empty())
        {
            if (InMode == ArchiveMode::Read)
            {
                std::ifstream File(Filename.c_str(), std::ios::binary | std::ios::ate);
                if (File.is_open())
                {
                    File >> Data;
                    File.close();
                }
            }
        }
    }

    void JsonArchive::Close()
    {
        Data.clear();
    }

    bool JsonArchive::SaveToFile(const char* OverrideFile) const
    {
        std::ofstream File(OverrideFile != nullptr ? OverrideFile : Filename.c_str(), std::ios::trunc | std::ios::out);
        if (File.is_open())
        {
            File << Data.dump();
            File.close();
            return true;
        }

        return false;
    }

    void JsonArchive::WriteBool(bool Val)
    {
        Data.push_back(Val);
        ++Pos;
    }

    void JsonArchive::WriteChar(unsigned char Val)
    {
        Data.push_back(Val);
        ++Pos;
    }

    void JsonArchive::WriteInt(uint64_t Val, bool bIsSigned)
    {
        Data.push_back(Val);
        ++Pos;
    }

    void JsonArchive::WriteFloat(double Val)
    {
        Data.push_back(Val);
        ++Pos;
    }

    void JsonArchive::WriteString(const char* Str, size_t Length)
    {
        Data.push_back(String(Str));
        Pos += Length;
    }

    void JsonArchive::ReadBool(bool& OutVal)
    {
        OutVal = Data[Pos].get<bool>();
        ++Pos;
    }

    void JsonArchive::ReadChar(unsigned char& OutVal)
    {
        OutVal = Data[Pos].get<unsigned char>();
        ++Pos;
    }

    void JsonArchive::ReadInt(uint64_t& OutVal, bool bIsSigned)
    {
        OutVal = Data[Pos].get<uint64_t>();
        ++Pos;
    }

    void JsonArchive::ReadFloat(double& OutVal)
    {
        OutVal = Data[Pos].get<double>();
        ++Pos;
    }

    void JsonArchive::ReadString(char* Str, size_t Length)
    {
        String Temp = Data[Pos].get<String>();
        strcpy(Str, Temp.c_str());
        Pos += Length;
    }
}