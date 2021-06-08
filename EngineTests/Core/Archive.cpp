#include <gtest/gtest.h>
#include <Archive.h>

using namespace ScarletEngine;

TEST(Archive, Basic)
{
	Archive Ar;

	int A = 10;
	int B = 11;
	int C = 12;

	Ar << A;
	Ar << B;
	Ar << C;

	Ar.SetReadModeAndResetPos();

	int ReadA = 0;
	int ReadB = 0;
	int ReadC = 0;

	Ar >> ReadA;
	Ar >> ReadB;
	Ar >> ReadC;

	EXPECT_EQ(A, ReadA);
	EXPECT_EQ(B, ReadB);
	EXPECT_EQ(C, ReadC);
}


TEST(Archive, Primitives)
{
	Archive Ar;

	bool Boolean1 = true;
	bool Boolean2 = false;
	char Char = 'c';
	unsigned char UChar = 'd';
	int Integer = -15;
	int UInteger = 20;
	long Long = -1000;
	unsigned long ULong = 1234;
	long long LLong = -4567;
	unsigned long long ULLong = 8765;
	float Float = 3.14f;
	double Double = 2.71;

	Ar << Boolean1
		<< Boolean2
		<< Char
		<< UChar
		<< Integer
		<< UInteger
		<< Long
		<< ULong
		<< LLong
		<< ULLong
		<< Float
		<< Double;

	Ar.SetReadModeAndResetPos();

	bool ReadBoolean1 = false;
	bool ReadBoolean2 = false;
	char ReadChar = 0;
	unsigned char ReadUChar = 0;
	int ReadInteger = 0;
	int ReadUInteger = 0;
	long ReadLong = 0;
	unsigned long ReadULong = 0;
	long long ReadLLong = 0;
	unsigned long long ReadULLong = 0;
	float ReadFloat = 0.0f;
	double ReadDouble = 0.0;

	Ar >> ReadBoolean1
		>> ReadBoolean2
		>> ReadChar
		>> ReadUChar
		>> ReadInteger
		>> ReadUInteger
		>> ReadLong
		>> ReadULong
		>> ReadLLong
		>> ReadULLong
		>> ReadFloat
		>> ReadDouble;

	EXPECT_EQ(Boolean1, ReadBoolean1);
	EXPECT_EQ(Boolean2, ReadBoolean2);
	EXPECT_EQ(Char, ReadChar);
	EXPECT_EQ(UChar, ReadUChar);
	EXPECT_EQ(Integer, ReadInteger);
	EXPECT_EQ(UInteger, ReadUInteger);
	EXPECT_EQ(Long, ReadLong);
	EXPECT_EQ(ULong, ReadULong);
	EXPECT_EQ(LLong, ReadLLong);
	EXPECT_EQ(ULLong, ReadULLong);
	EXPECT_EQ(Float, ReadFloat);
	EXPECT_EQ(Double, ReadDouble);
}

TEST(Archive, Serializable)
{
	Archive Ar;

	struct SerializableType
	{
		int X;

		void Serialize(Archive& Arch) const
		{
			Arch << X;
		}

		void Deserialize(Archive& Arch)
		{
			Arch >> X;
		}
	};

	SerializableType S{ 50 };

	Ar << S;

	Ar.SetReadModeAndResetPos();

	SerializableType ReadS{};

	Ar >> ReadS;

	EXPECT_EQ(S.X, ReadS.X);
}

TEST(Archive, Array)
{
	Archive Ar;

	Array<uint32_t> IntArray{ 9, 6, 10, 11, 100, 2, 0, 40 };

	Ar << IntArray;

	Ar.SetReadModeAndResetPos();

	Array<uint32_t> ReadIntArray;

	Ar >> ReadIntArray;

	ASSERT_EQ(IntArray.size(), ReadIntArray.size());

	for (size_t i = 0; i < IntArray.size(); ++i)
	{
		EXPECT_EQ(IntArray[i], ReadIntArray[i]);
	}
}

TEST(Archive, String)
{
	Archive Ar;

	String Str = "This is a test string";

	Ar << Str;

	Ar.SetReadModeAndResetPos();

	String ReadStr;

	Ar >> ReadStr;

	EXPECT_EQ(Str, ReadStr);
}

TEST(Archive, File)
{
	Archive Ar;

	Array<uint32_t> IntArray{ 9, 6, 10, 11, 100, 2, 0, 40 };

	Ar << IntArray;

	Ar.SaveToFile("TestFile.data");

	Archive InArch("TestFile.data");

	Array<uint32_t> ReadIntArray;

	InArch >> ReadIntArray;

	ASSERT_EQ(IntArray.size(), ReadIntArray.size());

	for (size_t i = 0; i < IntArray.size(); ++i)
	{
		EXPECT_EQ(IntArray[i], ReadIntArray[i]);
	}
}