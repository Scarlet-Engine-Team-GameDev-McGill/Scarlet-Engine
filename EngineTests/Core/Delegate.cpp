#include <gtest/gtest.h>
#include <Core.h>

using namespace ScarletEngine;

TEST(Delegate, DelegateOneParam)
{
	Delegate<bool(uint32_t)> Del;

	Del.Bind([](uint32_t Val)
		{
			return Val == 10;
		});

	ASSERT_TRUE(Del.IsBound());

	EXPECT_TRUE(Del(10));
}

TEST(Delegate, DelegateSingleCall)
{
	uint32_t Count = 0;

	Delegate<bool(uint32_t)> Del;

	Del.Bind([&Count](uint32_t Val)
		{
			Count++;
			return Val == 10;
		});

	ASSERT_TRUE(Del.IsBound());
	EXPECT_TRUE(Del(10));
	EXPECT_EQ(Count, (uint32_t)1);
}

TEST(Delegate, DelegateReturnVoid)
{
	uint32_t Count = 0;
	
	Delegate<void()> Del;

	Del.Bind([&Count]()
		{
			Count++;
		});

	ASSERT_TRUE(Del.IsBound());
	EXPECT_TRUE(std::is_void_v<decltype(Del())>);
	Del();
	EXPECT_EQ(Count, (uint32_t)1);
}

TEST(Delegate, BindRebind)
{
	Delegate<bool(uint32_t)> Del;

	Del.Bind([](uint32_t Val)
		{
			return Val == 10;
		});

	ASSERT_TRUE(Del.IsBound());

	EXPECT_TRUE(Del(10));

	Del.Bind([](uint32_t Val)
		{
			return Val == 11;
		});

	EXPECT_FALSE(Del(10));
}

TEST(Delegate, DelegateTwoParams)
{
	Delegate<bool(uint32_t, uint32_t)> Del;

	Del.Bind([](uint32_t Val1, uint32_t Val2)
		{
			return Val1 == Val2;
		});

	ASSERT_TRUE(Del.IsBound());

	EXPECT_TRUE(Del(10, 10));
	EXPECT_FALSE(Del(11, 10));
}

TEST(Delegate, DelegateThreeParams)
{
	Delegate<bool(uint32_t, uint32_t, uint32_t)> Del;

	Del.Bind([](uint32_t Val1, uint32_t Val2, uint32_t Val3)
		{
			return Val1 == Val2 && Val1 == Val3;
		});

	ASSERT_TRUE(Del.IsBound());

	EXPECT_TRUE(Del(10, 10, 10));
	EXPECT_FALSE(Del(11, 10, 9));
}

TEST(Delegate, DelegateFourParams)
{
	Delegate<bool(uint32_t, uint32_t, uint32_t, uint32_t)> Del;

	Del.Bind([](uint32_t Val1, uint32_t Val2, uint32_t Val3, uint32_t Val4)
		{
			return Val1 == Val2 && Val1 == Val3 && Val1 == Val4;
		});

	ASSERT_TRUE(Del.IsBound());

	EXPECT_TRUE(Del(10, 10, 10, 10));
	EXPECT_FALSE(Del(11, 10, 9, 8));
}