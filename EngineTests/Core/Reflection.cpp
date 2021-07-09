#include <gtest/gtest.h>
#include <Serialization/BinaryArchive.h>
#include <Reflection/Reflection.h>

using namespace ScarletEngine;

struct TestObject
{
    REFLECTION();

    uint32_t SomeInt = 10;
    float SomeFloat = 15.f;
    String TransientVal = "Transient";
};

BEGIN_REFLECTION_INFO(TestObject)
    Property(&TestObject::SomeInt, "SomeInt");
    Property(&TestObject::SomeFloat, "SomeFloat");
END_REFLECTION_INFO()

struct TestOuter
{
    REFLECTION();

    TestObject InnerObject;
    String AString;
};

BEGIN_REFLECTION_INFO(TestOuter)
    Property(&TestOuter::InnerObject, "Inner");
    Property(&TestOuter::AString, "AString");
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
    Obj->TransientVal = "Don't Serialize";

    Json Arc;
    TestObjectReflection->Serialize(reinterpret_cast<byte_t*>(Obj), Arc, "TestObject");

    TestObject Compare{};
    TestObjectReflection->Deserialize(reinterpret_cast<byte_t*>(&Compare), Arc, "TestObject");

    EXPECT_EQ(Compare.SomeInt, Obj->SomeInt);
    EXPECT_EQ(Compare.SomeFloat, Obj->SomeFloat);
    EXPECT_STRNE(Compare.TransientVal.c_str(), Obj->TransientVal.c_str());

    TestObjectReflection->Destruct(reinterpret_cast<byte_t*>(Obj));

    free(Obj);
}

TEST(Reflection, NestedObjectSerialization)
{
    const Reflection::TypeInfo* TestObjectReflection = TestOuter::BuildTypeInfo();
    TestOuter Outer;
    Outer.InnerObject.SomeFloat = 3.14f;
    Outer.AString = "Hello world";

    Json Arc;
    TestObjectReflection->Serialize(reinterpret_cast<const byte_t*>(&Outer), Arc, "TestObject");

    TestOuter Compare;
    EXPECT_NE(Compare.InnerObject.SomeFloat, 3.14f);
    TestObjectReflection->Deserialize(reinterpret_cast<byte_t*>(&Compare), Arc, "TestObject");

    EXPECT_EQ(Outer.InnerObject.SomeFloat, 3.14f);
    EXPECT_STREQ(Compare.AString.c_str(), Outer.AString.c_str());

    std::cout << Arc.dump(4) << std::endl;
}