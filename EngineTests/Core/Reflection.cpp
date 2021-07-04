#include <gtest/gtest.h>
#include <Reflection/Reflection.h>

using namespace ScarletEngine;

struct TestObject
{
    REFLECTION();

    uint32_t SomeInt = 10;
    float SomeFloat = 15.f;
};

BEGIN_REFLECTION_INFO(TestObject)
    Property(&TestObject::SomeInt, "SomeInt");
    Property(&TestObject::SomeFloat, "SomeFloat");
END_REFLECTION_INFO()

TEST(Reflection, Construct)
{
    const Reflection::TypeInfo* TestObjectReflection = TestObject::BuildTypeInfo();
    TestObject* Obj = static_cast<TestObject*>(malloc(sizeof(TestObject)));

    ASSERT_NE(Obj, nullptr);

    TestObjectReflection->Construct(reinterpret_cast<byte_t*>(Obj));

    EXPECT_EQ(Obj->SomeInt, 10u);
    EXPECT_EQ(Obj->SomeFloat, 15.f);

    TestObjectReflection->Destruct(reinterpret_cast<byte_t*>(Obj));
    free(Obj);
}

TEST(Reflection, Serialize)
{
    const Reflection::TypeInfo* TestObjectReflection = TestObject::BuildTypeInfo();
    TestObject* Obj = static_cast<TestObject*>(malloc(sizeof(TestObject)));

    ASSERT_NE(Obj, nullptr);

    TestObjectReflection->Construct(reinterpret_cast<byte_t*>(Obj));
    Obj->SomeInt = 1000;

    BinaryArchive Arc;
    TestObjectReflection->Serialize(reinterpret_cast<byte_t*>(Obj), Arc);

    Arc.SetReadModeAndResetPos();
    TestObject Compare{};
    TestObjectReflection->Deserialize(reinterpret_cast<byte_t*>(&Compare), Arc);

    EXPECT_EQ(Compare.SomeInt, Obj->SomeInt);
    EXPECT_EQ(Compare.SomeFloat, Obj->SomeFloat);

    TestObjectReflection->Destruct(reinterpret_cast<byte_t*>(Obj));

    free(Obj);
}