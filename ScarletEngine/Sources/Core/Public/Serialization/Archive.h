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

// ---------------------------------------------------------------------------------------------------------------------

    class Archive
    {
    public:
        Archive(ArchiveMode InMode = ArchiveMode::Write);
        /** Constructor which immediately loads data from a file */
        Archive(const String& InFilename, ArchiveMode InMode = ArchiveMode::Read);

        virtual ~Archive() {}

        /** Return the mode of the archive */
        ArchiveMode GetMode() const { return Mode; }

        /** Dump the contents of the Archive to a file. By default uses the stored filename but can be overriden with the OverrideFile param.  */
        virtual bool SaveToFile(const char* OverrideFile = nullptr) const;
        /** Clear the stored data. */
        virtual void Close() = 0;
        /** Set the archive to Read mode. */
        virtual void SetReadMode() { Mode = ArchiveMode::Read; }
        /** Set the archive to Write mode */
        virtual void SetWriteMode() { Mode = ArchiveMode::Write; }

        /* Write Operations */

        inline Archive& operator<<(bool Data)
        {
            WriteBool(Data);
            return *this;
        }

        inline Archive& operator<<(char Data)
        {
            WriteChar(Data);
            return *this;
        }

        inline Archive& operator<<(signed char Data)
        {
            WriteChar(Data);
            return *this;
        }

        inline Archive& operator<<(unsigned char Data)
        {
            WriteChar(Data);
            return *this;
        }

        inline Archive& operator<<(unsigned short Data)
        {
            WriteInt(Data);
            return *this;
        }

        inline Archive& operator<<(unsigned int Data)
        {
            WriteInt(Data);
            return *this;
        }

        inline Archive& operator<<(unsigned long Data)
        {
            WriteInt(Data);
            return *this;
        }
        
        inline Archive& operator<<(unsigned long long Data)
        {
            WriteInt(Data);
            return *this;
        }

        inline Archive& operator<<(short Data)
        {
            WriteInt(Data);
            return *this;
        }

        inline Archive& operator<<(int Data)
        {
            WriteInt(Data);
            return *this;
        }

        inline Archive& operator<<(long Data)
        {
            WriteInt(Data);
            return *this;
        }
        
        inline Archive& operator<<(long long Data)
        {
            WriteInt(Data);
            return *this;
        }

        inline Archive& operator<<(float Data)
        {
            WriteFloat(Data);
            return *this;
        }

        inline Archive& operator<<(double Data)
        {
            WriteFloat(Data);
            return *this;
        }

        template <typename ElemType, typename Alloc>
        inline Archive& operator<<(const Array<ElemType, Alloc>& Arr)
        {
            (*this) << Arr.size();
            for (const auto& Data : Arr)
            {
                (*this) << Data;
            }
            return *this;
        }

        template <typename ElemType, typename Alloc>
        inline Archive& operator<<(const Array<SharedPtr<ElemType>, Alloc>& Arr)
        {
            (*this) << Arr.size();
            for (const auto& DataPtr : Arr)
            {
                (*this) << *DataPtr;
            }
            return *this;
        }

        template <typename ElemType, typename Traits, typename Alloc>
        inline Archive& operator<<(const BasicString<ElemType, Traits, Alloc>& Str)
        {
            const size_t StrCount = static_cast<size_t>(Str.size());
            (*this) << StrCount;
            WriteString(Str.c_str(), StrCount);
            return *this;
        }

        template <typename ElemType, typename Alloc>
        inline Archive& operator<<(Array<ElemType, Alloc>& Arr)
        {
            (*this) << Arr.size();
            for (const auto& Data : Arr)
            {
                (*this) << Data;
            }
            return *this;
        }

        template <typename ElemType, typename Alloc>
        inline Archive& operator<<(Array<SharedPtr<ElemType>, Alloc>& Arr)
        {
            (*this) << Arr.size();
            for (const auto& DataPtr : Arr)
            {
                (*this) << *DataPtr;
            }
            return *this;
        }

        template <typename ElemType, typename Traits, typename Alloc>
        inline Archive& operator<<(BasicString<ElemType, Traits, Alloc>& Str)
        {
            const size_t StrCount = (size_t)Str.size();
            (*this) << StrCount;
            WriteString(Str.c_str(), StrCount);
            return *this;
        }

        inline Archive& operator<<(const char* CStr)
        {
            const size_t StrCount = static_cast<size_t>(strlen(CStr));
            (*this) << StrCount;
            WriteString(CStr, StrCount);
            return *this;
        }

        template <typename Type>
        inline Archive& operator<<(Type& Data)
        {
            static_assert(Utils::HasSerialize<Type>::Value, "Attempting to serialize a non-serializable type");
            Data.Serialize(*this);
            return *this;
        }

        /* Read operations */

        inline Archive& operator>>(bool& DataOut)
        {
            ReadBool(DataOut);
            return *this;
        }

        inline Archive& operator>>(char& DataOut)
        {
            unsigned char Temp;
            ReadChar(Temp);
            DataOut = static_cast<char>(Temp);
            return *this;
        }

        inline Archive& operator>>(signed char& DataOut)
        {
            unsigned char Temp;
            ReadChar(Temp);
            DataOut = static_cast<signed char>(Temp);
            return *this;
        }

        inline Archive& operator>>(unsigned char& DataOut)
        {
            ReadChar(DataOut);
            return *this;
        }

        inline Archive& operator>>(unsigned short& DataOut)
        {
            uint64_t Temp;
            ReadInt(Temp);
            DataOut = static_cast<unsigned short>(Temp);
            return *this;
        }

        inline Archive& operator>>(unsigned int& DataOut)
        {
            uint64_t Temp;
            ReadInt(Temp);
            DataOut = static_cast<unsigned int>(Temp);
            return *this;
        }

        inline Archive& operator>>(unsigned long& DataOut)
        {
            uint64_t Temp;
            ReadInt(Temp);
            DataOut = static_cast<unsigned long>(Temp);
            return *this;
        }

        inline Archive& operator>>(unsigned long long& DataOut)
        {
            uint64_t Temp;
            ReadInt(Temp);
            DataOut = static_cast<unsigned long long>(Temp);
            return *this;
        }

        inline Archive& operator>>(short& DataOut)
        {
            uint64_t Temp;
            ReadInt(Temp);
            DataOut = static_cast<short>(Temp);
            return *this;
        }

        inline Archive& operator>>(int& DataOut)
        {
            uint64_t Temp;
            ReadInt(Temp);
            DataOut = static_cast<int>(Temp);
            return *this;
        }

        inline Archive& operator>>(long& DataOut)
        {
            uint64_t Temp;
            ReadInt(Temp);
            DataOut = static_cast<long>(Temp);
            return *this;
        }

        inline Archive& operator>>(long long& DataOut)
        {
            uint64_t Temp;
            ReadInt(Temp);
            DataOut = static_cast<long long>(Temp);
            return *this;
        }
 
        inline Archive& operator>>(float& DataOut)
        {
            double Temp;
            ReadFloat(Temp);
            DataOut = static_cast<float>(Temp);
            return *this;
        }

        inline Archive& operator>>(double& DataOut)
        {
            ReadFloat(DataOut);
            return *this;
        }

        // default deserialize function
        template <typename Type>
        inline Archive& operator>>(Type& DataOut)
        {
            static_assert(Utils::HasDeserialize<Type>::Value, "Attempting to deserialize a non-serializable type!");
            DataOut.Deserialize(*this);
            return *this;
        }

        template <typename ElemType, typename Alloc>
        inline Archive& operator>>(Array<ElemType, Alloc>& Arr)
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

        template <typename ElemType, typename Alloc>
        inline Archive& operator>>(Array<SharedPtr<ElemType>, Alloc>& Arr)
        {
            size_t ArrayCount = 0;
            (*this) >> ArrayCount;
            Arr.resize(ArrayCount);
            for (size_t i = 0; i < ArrayCount; ++i)
            {
                // this could be a bit sketchy of the type doesn't have a default constructor
                Arr[i] = MakeShared<ElemType>();
                (*this) >> *Arr[i];
            }
            return *this;
        }

        template <typename ElemType, typename Traits, typename Alloc>
        inline Archive& operator>>(BasicString<ElemType, Traits, Alloc>& Str)
        {
            size_t StringCount = 0;
            (*this) >> StringCount;
            Str.resize(StringCount);
            check(Str.capacity() >= StringCount);
            ReadString(const_cast<char*>(Str.c_str()), StringCount);
            return *this;
        }

    protected:
        virtual void WriteBool(bool Val) = 0;
        virtual void WriteChar(unsigned char Val) = 0;
        virtual void WriteInt(uint64_t Data) = 0;
        virtual void WriteFloat(double Data) = 0;
        virtual void WriteString(const char* Str, size_t Length) = 0;

        virtual void ReadBool(bool& OutVal) = 0;
        virtual void ReadChar(unsigned char& OutVal) = 0;
        virtual void ReadInt(uint64_t& Data) = 0;
        virtual void ReadFloat(double& Data) = 0;
        virtual void ReadString(char* Str, size_t Length) = 0;
    protected:
        String Filename;
        ArchiveMode Mode;
    };
}