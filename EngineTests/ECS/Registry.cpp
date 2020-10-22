#include <gtest/gtest.h>
#include <ECS.h>
#include <algorithm>
#include <random>

using namespace ScarletEngine;

struct TestComponent
{
	inline static uint32_t ConstructorCounter = 0;
	inline static uint32_t MoveCounter = 0;

	static void ResetCounter()
	{
		ConstructorCounter = 0;
		MoveCounter = 0;
	}

	// Explicitly declare each constructor to keep track of moves and constructions

	TestComponent() { ConstructorCounter++; }
	TestComponent(const TestComponent& Other) { Val = Other.Val; ConstructorCounter++; }
	TestComponent(TestComponent&& Other) noexcept { Val = Other.Val; MoveCounter++; }

	TestComponent& operator=(const TestComponent& Other) { Val = Other.Val; ConstructorCounter++; return *this; }
	TestComponent& operator=(TestComponent&& Other) noexcept { Val = Other.Val; MoveCounter++; return *this; }

	uint32_t Val = 10;
};

TEST(ECS, RegistryCreateEntity)
{
	Registry Reg;
	Entity TestEntity("TestEnt");
	
	EXPECT_EQ(TestEntity.ID, INVALID_EID);

	Reg.CreateEntity<TestComponent>(TestEntity);

	EID EntityID = TestEntity.ID;

	EXPECT_NE(EntityID, INVALID_EID);

	Reg.CreateEntity(TestEntity);
	
	EXPECT_NE(TestEntity.ID, INVALID_EID);
	EXPECT_NE(TestEntity.ID, EntityID);
}

TEST(ECS, RegistryCreateComponent)
{
	TestComponent::ResetCounter();

	Registry Reg;
	Entity TestEntity("TestEnt");

	auto [TC] = Reg.CreateEntity<TestComponent>(TestEntity);

	EXPECT_EQ(TestComponent::ConstructorCounter, (uint32_t)1);

	TestComponent* GetTC = Reg.GetComponent<TestComponent>(TestEntity.ID);

	EXPECT_EQ(TestComponent::ConstructorCounter, (uint32_t)1);

	EXPECT_EQ(TC, GetTC);
}

TEST(ECS, RegistryAddComponent)
{
	TestComponent::ResetCounter();

	Registry Reg;
	Entity TestEntity("TestEnt");

	Reg.CreateEntity<>(TestEntity);
	
	EXPECT_EQ(TestComponent::ConstructorCounter, (uint32_t)0);

	EXPECT_EQ(Reg.GetComponent<TestComponent>(TestEntity.ID), nullptr);

	Reg.AddComponent<TestComponent>(TestEntity.ID);

	EXPECT_EQ(TestComponent::ConstructorCounter, (uint32_t)1);

	EXPECT_NE(Reg.GetComponent<TestComponent>(TestEntity.ID), nullptr);
}

TEST(ECS, RegistryAttachComponent)
{
	TestComponent::ResetCounter();

	Registry Reg;
	Entity TestEntity("TestEnt");

	Reg.CreateEntity<>(TestEntity);

	TestComponent TC;
	
	Reg.AttachComponent(TestEntity.ID, TC);

	EXPECT_EQ(TestComponent::ConstructorCounter, (uint32_t)2);
	// Attach should not call the move constructor since we expect the TC in this scope to remain alive.
	EXPECT_EQ(TestComponent::MoveCounter, (uint32_t)0);

	EXPECT_NE(Reg.GetComponent<TestComponent>(TestEntity.ID), nullptr);
}

TEST(ECS, RegistrySort)
{
	const uint32_t NumEntities = 100;
	Registry Reg;

	Array<Entity> Entities;

	for (uint32_t i = 0; i < NumEntities; ++i)
	{
		Entity E(std::to_string(i).c_str());

		auto [TC] = Reg.CreateEntity<TestComponent>(E);
		TC->Val = i;

		Entities.push_back(E);
	}

	Array<Entity> ShuffledEntities = Entities;

	auto RngEngine = std::default_random_engine{};
	std::shuffle(ShuffledEntities.begin(), ShuffledEntities.end(), RngEngine);

	for (const auto& Ent : ShuffledEntities)
	{
		uint32_t Val = Reg.GetComponent<TestComponent>(Ent.ID)->Val;
		Reg.RemoveComponent<TestComponent>(Ent.ID);
		TestComponent* TC = Reg.AddComponent<TestComponent>(Ent.ID);
		TC->Val = Val;
	}

	Reg.SortAll();

	for (uint32_t i = 1; i < NumEntities; ++i)
	{
		const TestComponent* PreviousTC = Reg.GetComponent<TestComponent>(Entities[i - (uint32_t)1].ID);
		const TestComponent* TC = Reg.GetComponent<TestComponent>(Entities[i].ID);
		
		// Expect that they are contiguous in memory (no holes between them)
		EXPECT_EQ(PreviousTC + 1, TC);
		// Ensure they have the expected values after sorting
		EXPECT_EQ(TC->Val, i);
		EXPECT_EQ(PreviousTC->Val + 1, TC->Val);
	}
}