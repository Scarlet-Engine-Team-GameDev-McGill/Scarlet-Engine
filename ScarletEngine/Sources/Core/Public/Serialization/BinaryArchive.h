#pragma once

#include "CoreMinimal.h"
#include "Archive.h"

namespace ScarletEngine
{
    /**
     * Handles binary data serialization and deserialization.
     */
    class BinaryArchive : public Archive
    {
    public:
        BinaryArchive(ArchiveMode InMode = ArchiveMode::Write);
        /**  Construct the Archive and immediately load data from the file. */
        BinaryArchive(const String& Filename, ArchiveMode Mode = ArchiveMode::Read);

        /** Close the archive by dumping contents to file and clearing it of data. */
        virtual void Close() override;
        /** Dump the binary contents of the Archive to a file. By default uses the stored filename but can be overriden with the OverrideFile param.  */
        virtual bool SaveToFile(const char* OverrideFile = nullptr) const override;
        /** Set the archive to Read Mode and reset the position to the start */
        virtual void SetReadMode() override { Pos = 0; Mode = ArchiveMode::Read; }
        /** Set the archive to Write mode and reset the position to the start */
        virtual void SetWriteMode() override { Pos = 0; Mode = ArchiveMode::Write; }
    protected:
        virtual void WriteBool(bool Val) override;
        virtual void WriteChar(unsigned char Val) override;
        virtual void WriteInt(uint64_t Val) override;
        virtual void WriteFloat(double Val) override;
        virtual void WriteString(const char* Str, size_t Length) override;

        virtual void ReadBool(bool& OutVal) override;
        virtual void ReadChar(unsigned char& OutVal) override;
        virtual void ReadInt(uint64_t& OutVal) override;
        virtual void ReadFloat(double& OutVal) override;
        virtual void ReadString(char* OutStr, size_t Length) override;

        void Write(const byte_t* Data, size_t Size);
        void Read(byte_t* OutData, size_t Size);
    protected:
        Array<byte_t> DataArray;
        size_t Pos;
    };
}
