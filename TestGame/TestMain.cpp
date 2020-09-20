#include "Core/Core.h"
#include "World.h"
#include "ECS/ECS.h"

struct Position
{
	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;

	void Serialize(ScarletEngine::Archive& Arc) const
	{
		Arc << X
			<< Y
			<< Z;
	}

	void Deserialize(ScarletEngine::Archive& Arc)
	{
		Arc >> X
			>> Y
			>> Z;
	}
};

struct Velocity
{
	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
};

int main()
{
	using namespace ScarletEngine;
	Archive Arch;

	for (int i = 0; i < 20; ++i)
	{
		Position Pos{ (float)i, (float)(i + 1), (float)(i + 2) };
		Arch << Pos;
	}

	Arch.SaveToFile("TestFile.data");

	Array<Position> Values;

	Archive InArch("TestFile.data");

	for (int i = 0; i < 20; ++i)
	{
		Position Pos;
		InArch >> Pos;
		Values.push_back(Pos);
	}
	{
		Archive NestedArrayArch;
		Array<Array<int>> NestedArray(10);
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				NestedArray[i].emplace_back(j + i);
			}
		}
		NestedArrayArch << NestedArray;
		NestedArrayArch.SaveToFile("NestedArray.data");

	Values.clear();
	
	Array<Position> SerializedArray;
	for (int i = 0; i < 20; ++i)
	{
		SerializedArray.push_back({ (float)(i / 1.f), (float)(i / 2.f), (float)(i / 3.f) });
	}
	Archive ArrayArch;
	ArrayArch << SerializedArray;
	ArrayArch.SaveToFile("ArrayFile.data");

	Archive InArrayArch("ArrayFile.data");
	Array<Position> InArray;
	InArrayArch >> InArray;


	Array<String> SerializedStringArray;
	for (int i = 0; i < 20; ++i)
	{
		char Buff[32];
		snprintf(Buff, 32, "Hello world %d", i);
		SerializedStringArray.emplace_back(Buff);
	}
	Archive StringArch;
	StringArch << SerializedStringArray;
	StringArch.SaveToFile("StringFile.data");

	Archive InStringArch("StringFile.data");
	Array<String> InStringArray;
	InStringArch >> InStringArray;

	return 0;
}