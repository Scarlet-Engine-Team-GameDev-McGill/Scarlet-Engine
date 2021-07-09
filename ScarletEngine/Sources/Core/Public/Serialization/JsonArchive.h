#pragma once

#include "CoreMinimal.h"
#include "Archive.h"

namespace ScarletEngine
{
    class JsonArchive : public Archive
    {
    public:
        JsonArchive(ArchiveMode InMode = ArchiveMode::Write);
        /**  Construct the Archive and immediately load data from the file. */
        JsonArchive(const String& Filename, ArchiveMode Mode = ArchiveMode::Read);

        virtual void Close() override;
        virtual bool SaveToFile(const char* OverrideFile = nullptr) const override;
        virtual void SetReadMode() override { Pos = 0; Mode = ArchiveMode::Read; }
    protected:
        virtual void WriteBool(bool Val) override;
        virtual void WriteChar(unsigned char Val) override;
        virtual void WriteInt(uint64_t Val, bool bIsSigned) override;
        virtual void WriteFloat(double Val) override;
        virtual void WriteString(const char* Str, size_t Length) override;

        virtual void ReadBool(bool& OutVal) override;
        virtual void ReadChar(unsigned char& OutVal) override;
        virtual void ReadInt(uint64_t& OutVal, bool bIsSigned) override;
        virtual void ReadFloat(double& OutVal) override;
        virtual void ReadString(char* OutStr, size_t Length) override;
    protected:
        Json Data;
        size_t Pos = 0;
    };
}