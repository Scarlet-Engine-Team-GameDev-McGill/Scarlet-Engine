#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine
{
	enum class ArchiveMode
	{
		Read,
		Write
	};

	class Archive
	{
	public:
		Archive();
		Archive(const Archive&) = default;
		Archive(Archive&&) = default;
		Archive(const String& Filename, ArchiveMode Mode = ArchiveMode::Read);
		~Archive() { Close(); }

		void Close();
		bool SaveToFile(const char* OverrideFile = nullptr);
		void SetReadModeandResetPos() { Pos = 0; Mode = ArchiveMode::Read; }

		ArchiveMode GetMode() { return Mode; }

		/* Write operations */

		inline Archive& operator<<(bool Data)
		{
			Write((uint8_t)(Data ? 1 : 0));
			return *this;
		}

		inline Archive& operator<<(char Data)
		{
			Write((uint8_t)Data);
			return *this;
		}

		inline Archive& operator<<(unsigned char Data)
		{
			Write((uint8_t)Data);
			return *this;
		}

		inline Archive& operator<<(int Data)
		{
			Write((uint64_t)Data);
			return *this;
		}

		inline Archive& operator<<(unsigned int Data)
		{
			Write((uint64_t)Data);
			return *this;
		}

		inline Archive& operator<<(long Data)
		{
			Write((uint64_t)Data);
			return *this;
		}

		inline Archive& operator<<(unsigned long Data)
		{
			Write((uint64_t)Data);
			return *this;
		}

		inline Archive& operator<<(long long Data)
		{
			Write((uint64_t)Data);
			return *this;
		}

		inline Archive& operator<<(unsigned long long Data)
		{
			Write((uint64_t)Data);
			return *this;
		}

		inline Archive& operator<<(float Data)
		{
			Write(Data);
			return *this;
		}

		inline Archive& operator<<(double Data)
		{
			Write(Data);
			return *this;
		}

		template <typename ElemType>
		inline Archive& operator<<(const Array<ElemType>& Arr)
		{
			(*this) << Arr.size();
			for (const auto& Data : Arr)
			{
				(*this) << Data;
			}
			return *this;
		}

		template <typename ElemType, typename Traits, typename Alloc>
		inline Archive& operator<<(const BasicString<ElemType, Traits, Alloc>& Str)
		{
			size_t StrCount = (size_t)Str.size();
			(*this) << StrCount;
			Write(*Str.c_str(), StrCount);
			return *this;
		}

		template <typename Serializable>
		inline Archive& operator<<(const Serializable& Data)
		{
			Data.Serialize(*this);
			return *this;
		}

		/* Read operations */

		inline Archive& operator>>(bool& DataOut)
		{
			uint8_t Temp;
			Read(Temp);
			DataOut = (Temp == 1);
			return *this;
		}

		inline Archive& operator>>(char& DataOut)
		{
			uint8_t Temp;
			Read(Temp);
			DataOut = (char)Temp;
			return *this;
		}

		inline Archive& operator>>(unsigned char& DataOut)
		{
			uint8_t Temp;
			Read(Temp);
			DataOut = (unsigned char)Temp;
			return *this;
		}

		inline Archive& operator>>(int& DataOut)
		{
			uint64_t Temp;
			Read(Temp);
			DataOut = (int)Temp;
			return *this;
		}

		inline Archive& operator>>(unsigned int& DataOut)
		{
			uint64_t Temp;
			Read(Temp);
			DataOut = (unsigned int)Temp;
			return *this;
		}

		inline Archive& operator>>(long& DataOut)
		{
			uint64_t Temp;
			Read(Temp);
			DataOut = (long)Temp;
			return *this;
		}

		inline Archive& operator>>(unsigned long& DataOut)
		{
			uint64_t Temp;
			Read(Temp);
			DataOut = (unsigned long)Temp;
			return *this;
		}

		inline Archive& operator>>(long long& DataOut)
		{
			uint64_t Temp;
			Read(Temp);
			DataOut = (long long)Temp;
			return *this;
		}

		inline Archive& operator>>(unsigned long long& DataOut)
		{
			uint64_t Temp;
			Read(Temp);
			DataOut = (unsigned long long)Temp;
			return *this;
		}

		inline Archive& operator>>(float& DataOut)
		{
			Read(DataOut);
			return *this;
		}

		inline Archive& operator>>(double& DataOut)
		{
			Read(DataOut);
			return *this;
		}
		
		template <typename Deserializable>
		inline Archive& operator>>(Deserializable& Data)
		{
			Data.Deserialize(*this);
			return *this;
		}

		template <typename ElemType>
		inline Archive& operator>>(Array<ElemType>& Arr)
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
		inline Archive& operator>>(BasicString<ElemType, Traits, Alloc>& Str)
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