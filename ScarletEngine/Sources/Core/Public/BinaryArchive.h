#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
// ---------------------------------------------------------------------------------------------------------------------

    namespace Utils
    {
        template <typename T>
        class HasSerialize
        {
            using One = char;
            struct Two { char x[2]; };

            template <typename C> static One Test( decltype(&C::Serialize) ) ;
            template <typename C> static Two Test(...);    

        public:
            enum { Value = sizeof(Test<T>(0)) == sizeof(char) };
        };

        template <typename T>
        class HasDeserialize
        {
            using One = char;
            struct Two { char x[2]; };

            template <typename C> static One Test( decltype(&C::Deserialize) ) ;
            template <typename C> static Two Test(...);    

        public:
            enum { Value = sizeof(Test<T>(0)) == sizeof(char) };
        };
    }

// ---------------------------------------------------------------------------------------------------------------------

    enum class ArchiveMode
    {
        Read,
        Write
    };

    /**
     * Handles binary data serialization and deserialization.
     */
    class BinaryArchive
    {
    public:
        BinaryArchive();
        BinaryArchive(const BinaryArchive&) = default;
        BinaryArchive(BinaryArchive&&) = default;
        /**  Construct the Archive and immediately load data from the file. */
        BinaryArchive(const String& Filename, ArchiveMode Mode = ArchiveMode::Read);
        ~BinaryArchive() { Close(); }

        /** Close the archive by dumping contents to file and clearing it of data. */
        void Close();
        /** Dump the binary contents of the Archive to a file. By default uses the stored filename but can be overriden with the OverrideFile param.  */
        bool SaveToFile(const char* OverrideFile = nullptr);
        /** Set the archive to Read Mode and reset the position to the start */
        void SetReadModeAndResetPos() { Pos = 0; Mode = ArchiveMode::Read; }
        /** Return the mode of the archive */
        ArchiveMode GetMode() const { return Mode; }

        /* Write operations */

        inline BinaryArchive& operator<<(bool Data)
        {
            Write((uint8_t)(Data ? 1 : 0));
            return *this;
        }

        inline BinaryArchive& operator<<(char Data)
        {
            Write((uint8_t)Data);
            return *this;
        }

        inline BinaryArchive& operator<<(unsigned char Data)
        {
            Write((uint8_t)Data);
            return *this;
        }

        inline BinaryArchive& operator<<(int Data)
        {
            Write((uint64_t)Data);
            return *this;
        }

        inline BinaryArchive& operator<<(unsigned int Data)
        {
            Write((uint64_t)Data);
            return *this;
        }

        inline BinaryArchive& operator<<(long Data)
        {
            Write((uint64_t)Data);
            return *this;
        }

        inline BinaryArchive& operator<<(unsigned long Data)
        {
            Write((uint64_t)Data);
            return *this;
        }

        inline BinaryArchive& operator<<(long long Data)
        {
            Write((uint64_t)Data);
            return *this;
        }

        inline BinaryArchive& operator<<(unsigned long long Data)
        {
            Write((uint64_t)Data);
            return *this;
        }

        inline BinaryArchive& operator<<(float Data)
        {
            Write(Data);
            return *this;
        }

        inline BinaryArchive& operator<<(double Data)
        {
            Write(Data);
            return *this;
        }

        template <typename ElemType, typename Alloc>
        inline BinaryArchive& operator<<(const Array<ElemType, Alloc>& Arr)
        {
            (*this) << Arr.size();
            for (const auto& Data : Arr)
            {
                (*this) << Data;
            }
            return *this;
        }

        template <typename ElemType, typename Traits, typename Alloc>
        inline BinaryArchive& operator<<(const BasicString<ElemType, Traits, Alloc>& Str)
        {
            size_t StrCount = (size_t)Str.size();
            (*this) << StrCount;
            Write(*Str.c_str(), StrCount);
            return *this;
        }

        template <typename Type>
        inline BinaryArchive& operator<<(Type& Data)
        {
            if constexpr (Utils::HasSerialize<Type>::Value)
            {
                Data.Serialize(*this);
            }
            else
            {
                Write(Data);
            }
            return *this;
        }

        /* Read operations */

        inline BinaryArchive& operator>>(bool& DataOut)
        {
            uint8_t Temp;
            Read(Temp);
            DataOut = (Temp == 1);
            return *this;
        }

        inline BinaryArchive& operator>>(char& DataOut)
        {
            uint8_t Temp;
            Read(Temp);
            DataOut = (char)Temp;
            return *this;
        }

        inline BinaryArchive& operator>>(unsigned char& DataOut)
        {
            uint8_t Temp;
            Read(Temp);
            DataOut = (unsigned char)Temp;
            return *this;
        }

        inline BinaryArchive& operator>>(int& DataOut)
        {
            uint64_t Temp;
            Read(Temp);
            DataOut = (int)Temp;
            return *this;
        }

        inline BinaryArchive& operator>>(unsigned int& DataOut)
        {
            uint64_t Temp;
            Read(Temp);
            DataOut = (unsigned int)Temp;
            return *this;
        }

        inline BinaryArchive& operator>>(long& DataOut)
        {
            uint64_t Temp;
            Read(Temp);
            DataOut = (long)Temp;
            return *this;
        }

        inline BinaryArchive& operator>>(unsigned long& DataOut)
        {
            uint64_t Temp;
            Read(Temp);
            DataOut = (unsigned long)Temp;
            return *this;
        }

        inline BinaryArchive& operator>>(long long& DataOut)
        {
            uint64_t Temp;
            Read(Temp);
            DataOut = (long long)Temp;
            return *this;
        }

        inline BinaryArchive& operator>>(unsigned long long& DataOut)
        {
            uint64_t Temp;
            Read(Temp);
            DataOut = (unsigned long long)Temp;
            return *this;
        }

        inline BinaryArchive& operator>>(float& DataOut)
        {
            Read(DataOut);
            return *this;
        }

        inline BinaryArchive& operator>>(double& DataOut)
        {
            Read(DataOut);
            return *this;
        }

        // default deserialize function
        template <typename Type>
        inline BinaryArchive& operator>>(Type& DataOut)
        {
            if constexpr (Utils::HasDeserialize<Type>::Value)
            {
                DataOut.Deserialize(*this);
            }
            else
            {
                // Default to memcpy if there is no custom deserialize method
                Read(DataOut);
            }
            return *this;
        }

        template <typename ElemType, typename Alloc>
        inline BinaryArchive& operator>>(Array<ElemType, Alloc>& Arr)
        {
            size_t ArrayCount = 0;
            (*this) >> ArrayCount;
            Arr.resize(ArrayCount);
            for (size_t i = 0; i < ArrayCount; ++i)
            {
                (*this) >> Arr[i];
            }
            return *this;
        }

        template <typename ElemType, typename Traits, typename Alloc>
        inline BinaryArchive& operator>>(BasicString<ElemType, Traits, Alloc>& Str)
        {
            size_t StringCount = 0;
            (*this) >> StringCount;
            Str.resize(StringCount);
            Read(*(char*)Str.c_str(), StringCount);
            return *this;
        }

    private:
        template <typename DataType>
        inline void Write(const DataType& Data, uint64_t Count = 1)
        {
            check(Mode == ArchiveMode::Write);
            size_t Size = (size_t)(sizeof(Data) * Count);
            size_t NewPos = Pos + Size;
            if (NewPos > DataArray.size())
            {
                DataArray.resize(NewPos * 2);
            }
            memcpy(reinterpret_cast<void*>((uint64_t)DataArray.data() + (uint64_t)Pos), &Data, Size);
            Pos = NewPos;
        }

        template <typename DataType>
        inline void Read(DataType& DataOut, uint64_t Count = 1)
        {
            check(Mode == ArchiveMode::Read);
            size_t Size = (size_t)(sizeof(DataOut) * Count);
            check(Pos + Size <= DataArray.size()); // Attempting to read past the end of the archive.
            memcpy(&DataOut, reinterpret_cast<void*>((uint64_t)DataArray.data() + (uint64_t)Pos), Size);
            Pos += Size;
        }
    private:
        String Filename;
        Array<byte_t> DataArray;
        size_t Pos;
        ArchiveMode Mode;
    };
}
