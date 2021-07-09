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
    TestObject* Obj = static_cast<TestObject*>(malloc(sizeof(TestObject)));

    ASSERT_NE(Obj, nullptr);

    Obj->Construct();

    EXPECT_EQ(Obj->SomeInt, 10u);
    EXPECT_EQ(Obj->SomeFloat, 15.f);

    Obj->Destruct();
    free(Obj);
}

TEST(Reflection, Serialize)
{
    TestObject* Obj = static_cast<TestObject*>(malloc(sizeof(TestObject)));

    ASSERT_NE(Obj, nullptr);

    Obj->Construct();
    Obj->SomeInt = 1000;
    Obj->TransientVal = "Don't Serialize";

    Json Arc;
    Obj->Serialize(Arc, "TestObject");

    TestObject Compare{};
    Compare.Deserialize(Arc, "TestObject");

    EXPECT_EQ(Compare.SomeInt, Obj->SomeInt);
    EXPECT_EQ(Compare.SomeFloat, Obj->SomeFloat);
    EXPECT_STRNE(Compare.TransientVal.c_str(), Obj->TransientVal.c_str());

    Obj->Destruct();

    free(Obj);
}

TEST(Reflection, NestedObjectSerialization)
{
    TestOuter Outer;
    Outer.InnerObject.SomeFloat = 3.14f;
    Outer.AString = "Hello world";

    Json Arc;
    Outer.Serialize(Arc, "TestObject");

    TestOuter Compare;
    EXPECT_NE(Compare.InnerObject.SomeFloat, 3.14f);
    Compare.Deserialize(Arc, "TestObject");

    EXPECT_EQ(Outer.InnerObject.SomeFloat, 3.14f);
    EXPECT_STREQ(Compare.AString.c_str(), Outer.AString.c_str());

    std::cout << Arc.dump(4) << std::endl;
}