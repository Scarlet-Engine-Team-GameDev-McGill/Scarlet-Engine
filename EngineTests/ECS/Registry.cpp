#include <gtest/gtest.h>
#include <ECS.h>
#include <algorithm>
#include <random>

#include "SystemScheduler.h"

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

	auto [TestEntity, TC] = Reg.CreateEntity<TestComponent>("TestEntity1");

	const EID EntityID = TestEntity->ID;

	EXPECT_NE(EntityID, INVALID_EID);

	auto [TestEntity2] = Reg.CreateEntity("TestEntity2");
	
	EXPECT_NE(TestEntity2->ID, INVALID_EID);
	EXPECT_NE(TestEntity2->ID, EntityID);
}

TEST(ECS, RegistryCreateComponent)
{
	TestComponent::ResetCounter();

	Registry Reg;

	auto [TestEntity, TC] = Reg.CreateEntity<TestComponent>("TestEntity");

	EXPECT_EQ(TestComponent::ConstructorCounter, static_cast<uint32_t>(1));

	TestComponent* GetTC = Reg.GetComponent<TestComponent>(TestEntity->ID);

	EXPECT_EQ(TestComponent::ConstructorCounter, static_cast<uint32_t>(1));

	EXPECT_EQ(TC, GetTC);
}

TEST(ECS, RegistryAddComponent)
{
	TestComponent::ResetCounter();

	Registry Reg;

	auto [TestEntity] = Reg.CreateEntity<>("TestEntity");
	
	EXPECT_EQ(TestComponent::ConstructorCounter, static_cast<uint32_t>(0));

	EXPECT_EQ(Reg.GetComponent<TestComponent>(TestEntity->ID), nullptr);

	Reg.AddComponent<TestComponent>(TestEntity->ID);

	EXPECT_EQ(TestComponent::ConstructorCounter, static_cast<uint32_t>(1));

	EXPECT_NE(Reg.GetComponent<TestComponent>(TestEntity->ID), nullptr);
}

TEST(ECS, RegistryAttachComponent)
{
	TestComponent::ResetCounter();

	Registry Reg;

	auto [TestEntity] = Reg.CreateEntity<>("TestEntity");

	TestComponent TC;
	
	Reg.AttachComponent(TestEntity->ID, TC);

	EXPECT_EQ(TestComponent::ConstructorCounter, static_cast<uint32_t>(2));
	// Attach should not call the move constructor since we expect the TC in this scope to remain alive.
	EXPECT_EQ(TestComponent::MoveCounter, static_cast<uint32_t>(0));

	EXPECT_NE(Reg.GetComponent<TestComponent>(TestEntity->ID), nullptr);
}

TEST(ECS, RegistrySort)
{
	const uint32_t NumEntities = 100;
	Registry Reg;

	Array<SharedPtr<Entity>> Entities;

	for (uint32_t i = 0; i < NumEntities; ++i)
	{
		auto [E, TC] = Reg.CreateEntity<TestComponent>(std::to_string(i).c_str());
		TC->Val = i;

		Entities.push_back(E);
	}

	Array<SharedPtr<Entity>> ShuffledEntities = Entities;

	auto RngEngine = std::default_random_engine{};
	std::shuffle(ShuffledEntities.begin(), ShuffledEntities.end(), RngEngine);

	for (const auto& Ent : ShuffledEntities)
	{
		const uint32_t Val = Reg.GetComponent<TestComponent>(Ent->ID)->Val;
		Reg.RemoveComponent<TestComponent>(Ent->ID);
		TestComponent* TC = Reg.AddComponent<TestComponent>(Ent->ID);
		TC->Val = Val;
	}

	Reg.SortAll();

	for (uint32_t i = 1; i < NumEntities; ++i)
	{
		const TestComponent* PreviousTC = Reg.GetComponent<TestComponent>(Entities[i - static_cast<uint32_t>(1)]->ID);
		const TestComponent* TC = Reg.GetComponent<TestComponent>(Entities[i]->ID);
		
		// Expect that they are contiguous in memory (no holes between them)
		EXPECT_EQ(PreviousTC + 1, TC);
		// Ensure they have the expected values after sorting
		EXPECT_EQ(TC->Val, i);
		EXPECT_EQ(PreviousTC->Val + 1, TC->Val);
	}
}

TEST(ECS, System)
{
	struct Component
	{
		uint32_t X = 11;	
	};
	
	class TestSystem : public ScarletEngine::System<Component>
	{
	public:
		TestSystem(Registry* Reg) : ScarletEngine::System<Component>(Reg, "TestSystem") {}
		
		virtual void Update() const override
		{
			for (auto& [Ent, TC] : GetEntities<Component>())
			{
				TC->X = 50;
			}
		}
	};

	Registry Reg;
	SystemScheduler Scheduler(&Reg);
	Scheduler.RegisterSystem<TestSystem>();
	auto [Entity, TC] = Reg.CreateEntity<Component>("TestComponent");

	ASSERT_NE(TC, nullptr);
	EXPECT_EQ(TC->X, static_cast<uint32_t>(11));

	Scheduler.RunUpdate();

	EXPECT_EQ(TC->X, static_cast<uint32_t>(50));
}

TEST(ECS, ConstSystem)
{
	struct Component
	{
		uint32_t X = 11;	
	};
	
	class TestSystem : public ScarletEngine::System<const Component>
	{
	public:
		TestSystem(Registry* Reg) : ScarletEngine::System<const Component>(Reg, "TestSystem") {}
		
		virtual void Update() const override
		{
			for (auto& [Ent, TC] : GetEntities<const Component>())
			{
				//TC->X = 50;
			}
		}
	};

	Registry Reg;
	SystemScheduler Scheduler(&Reg);
	Scheduler.RegisterSystem<TestSystem>();
	auto [Entity, TC] = Reg.CreateEntity<Component>("TestComponent");

	ASSERT_NE(TC, nullptr);
	EXPECT_EQ(TC->X, static_cast<uint32_t>(11));

	Scheduler.RunUpdate();

	EXPECT_EQ(TC->X, static_cast<uint32_t>(11));
}

TEST(ECS, Singleton)
{
	struct SingletonComponent
	{
		uint32_t X = 10;	
	};
	
	Registry Reg;
	SingletonComponent* SC = Reg.GetSingleton<SingletonComponent>();

	ASSERT_NE(SC, nullptr);
	EXPECT_EQ(SC->X, static_cast<uint32_t>(10));

	SingletonComponent* SC1 = Reg.GetSingleton<SingletonComponent>();

	EXPECT_EQ(SC, SC1); 

	Registry const* ConstReg = &Reg;

	SC1 = ConstReg->GetSingleton<SingletonComponent>();

	EXPECT_EQ(SC, SC1);

	class TestSystem : public ScarletEngine::System<SingletonComponent>
	{
	public:
		TestSystem(Registry* Reg) : ScarletEngine::System<SingletonComponent>(Reg, "TestSystem") {}
		
		virtual void Update() const override
		{
			SingletonComponent* SC = GetSingleton<SingletonComponent>();
			SC->X = 100;
		}
	};

	SystemScheduler Scheduler(&Reg);
	Scheduler.RegisterSystem<TestSystem>();

	Scheduler.RunUpdate();

	EXPECT_EQ(SC->X, 100);
}

TEST(ECS, ConstSingleton)
{
	struct SingletonComponent
	{
		uint32_t X = 10;	
	};

	Registry Reg;
	SingletonComponent* SC = Reg.GetSingleton<SingletonComponent>();

	ASSERT_NE(SC, nullptr);
	EXPECT_EQ(SC->X, static_cast<uint32_t>(10));

	class TestSystem : public ScarletEngine::System<const SingletonComponent>
	{
	public:
		TestSystem(Registry* Reg) : ScarletEngine::System<const SingletonComponent>(Reg, "TestSystem") {}
	
		virtual void Update() const override
		{
			const SingletonComponent* SC = GetSingleton<const SingletonComponent>();
			//SC->X = 100;
		}
	};

	SystemScheduler Scheduler(&Reg);
	Scheduler.RegisterSystem<TestSystem>();

	Scheduler.RunUpdate();

	EXPECT_EQ(SC->X, 10);
}
