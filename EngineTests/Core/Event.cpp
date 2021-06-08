#include <gtest/gtest.h>
#include <Core.h>

using namespace ScarletEngine;

TEST(Event, EventSingleParam)
{
	uint32_t Count = 0;

	Event<uint32_t> Ev;

	Ev.Bind([&Count](uint32_t Val)
		{
			Count += Val;
		});

	Ev.Bind([&Count](uint32_t Val)
		{
			Count +=  2 * Val;
		});

	EXPECT_EQ(Count, (uint32_t)0);
	
	Ev.Broadcast(10);

	EXPECT_EQ(Count, (uint32_t)30);
}

TEST(Event, EventClear)
{
	uint32_t Count = 0;

	Event<uint32_t> Ev;

	Ev.Bind([&Count](uint32_t Val)
		{
			Count += Val;
		});

	Ev.Bind([&Count](uint32_t Val)
		{
			Count += 2 * Val;
		});

	EXPECT_EQ(Count, (uint32_t)0);

	Ev.Broadcast(10);

	EXPECT_EQ(Count, (uint32_t)30);

	Ev.Clear();
	Ev.Broadcast(12);
	EXPECT_EQ(Count, (uint32_t)30);
}

TEST(Event, EventUnbind)
{
	uint32_t Count = 0;

	Event<uint32_t> Ev;

	// Here we are using the event as the owner of this callback. 
	// This is not a common pattern but we need two pointers and these are the only two we have
	Ev.Bind([&Count](uint32_t Val)
		{
			Count += Val;
		}, &Ev);

	Ev.Bind([&Count](uint32_t Val)
		{
			Count += Val;
		}, &Count);

	Ev.Broadcast(20);
	EXPECT_EQ(Count, (uint32_t)40);
	
	Ev.Unbind(&Count);

	Ev.Broadcast(10);
	EXPECT_EQ(Count, (uint32_t)50);
}