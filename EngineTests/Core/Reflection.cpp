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

struct TestArrayInt
{
    REFLECTION();

    Array<uint32_t> Arr = { 1, 2, 3 };
};

BEGIN_REFLECTION_INFO(TestArrayInt)
    Property(&TestArrayInt::Arr, "Arr");
END_REFLECTION_INFO()

struct TestArrayFloat
{
    REFLECTION();

    Array<float> FloatArr = { 0.1f, 0.2f, 0.3f };
    Array<double> DoubleArr = { 0.4, 0.5, 0.6 };
};

BEGIN_REFLECTION_INFO(TestArrayFloat)
    Property(&TestArrayFloat::FloatArr, "FloatArr");
    Property(&TestArrayFloat::DoubleArr, "DoubleArr");
END_REFLECTION_INFO()

struct TestArrayStr
{
    REFLECTION();

    Array<String> Arr = { "empty" };
};

BEGIN_REFLECTION_INFO(TestArrayStr)
    Property(&TestArrayStr::Arr, "Arr");
END_REFLECTION_INFO()

struct TestArrayArray
{
    REFLECTION();

    Array<Array<uint8_t>> Arr = { { 0u } };
};

BEGIN_REFLECTION_INFO(TestArrayArray)
    Property(&TestArrayArray::Arr, "Arr");
END_REFLECTION_INFO()

struct TestArrayObj
{
    REFLECTION();

    Array<TestOuter> Arr = { TestOuter{} };
};

BEGIN_REFLECTION_INFO(TestArrayObj)
    Property(&TestArrayObj::Arr, "Arr");
END_REFLECTION_INFO()

struct TestVectorObj
{
    REFLECTION();

    glm::vec2 vec2 = { 0.1f, 0.2f };
    glm::vec3 vec3;
    glm::vec4 vec4;
    glm::ivec2 ivec2 = { 1, 2 };
    glm::ivec3 ivec3;
    glm::ivec4 ivec4;
};

BEGIN_REFLECTION_INFO(TestVectorObj)
    Property(&TestVectorObj::vec2, "vec2");
    Property(&TestVectorObj::vec3, "vec3");
    Property(&TestVectorObj::vec4, "vec4");
    Property(&TestVectorObj::ivec2, "ivec2");
    Property(&TestVectorObj::ivec3, "ivec3");
    Property(&TestVectorObj::ivec4, "ivec4");
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

    std::cout << Arc.dump(4) << std::endl;

    TestOuter Compare;
    EXPECT_NE(Compare.InnerObject.SomeFloat, 3.14f);
    Compare.Deserialize(Arc, "TestObject");

    EXPECT_EQ(Outer.InnerObject.SomeFloat, 3.14f);
    EXPECT_STREQ(Compare.AString.c_str(), Outer.AString.c_str());
}

TEST(Reflection, IntArrayProperty)
{
    TestArrayInt Obj;
    Obj.Arr = { 5u, 6u, 7u };

    Json Arc;
    Obj.Serialize(Arc, "TestObject");

    std::cout << Arc.dump(4) << std::endl;

    TestArrayInt Compare;
    EXPECT_NE(Compare.Arr[0], 5u);
    Compare.Deserialize(Arc, "TestObject");

    EXPECT_EQ(Compare.Arr.size(), Obj.Arr.size());
    EXPECT_EQ(Compare.Arr[0], Obj.Arr[0]);
    EXPECT_EQ(Compare.Arr[1], Obj.Arr[1]);
    EXPECT_EQ(Compare.Arr[2], Obj.Arr[2]);
}

TEST(Reflection, FloatArrayProperty)
{
    TestArrayFloat Obj;
    Obj.FloatArr = { 0.4f, 0.5f, 0.6f };
    Obj.DoubleArr = { 0.1, 0.2, 0.3 };

    Json Arc;
    Obj.Serialize(Arc, "TestObject");

    std::cout << Arc.dump(4) << std::endl;

    TestArrayFloat Compare;
    EXPECT_NE(Compare.DoubleArr[0], 0.1);
    Compare.Deserialize(Arc, "TestObject");

    EXPECT_EQ(Compare.FloatArr.size(), Obj.FloatArr.size());
    EXPECT_EQ(Compare.FloatArr[0], Obj.FloatArr[0]);
    EXPECT_EQ(Compare.FloatArr[1], Obj.FloatArr[1]);
    EXPECT_EQ(Compare.FloatArr[2], Obj.FloatArr[2]);

    EXPECT_EQ(Compare.DoubleArr.size(), Obj.DoubleArr.size());
    EXPECT_EQ(Compare.DoubleArr[0], Obj.DoubleArr[0]);
    EXPECT_EQ(Compare.DoubleArr[1], Obj.DoubleArr[1]);
    EXPECT_EQ(Compare.DoubleArr[2], Obj.DoubleArr[2]);
}

TEST(Reflection, StringArrayProperty)
{
    TestArrayStr Obj;
    Obj.Arr = { "Hello", "World" };

    Json Arc;
    Obj.Serialize(Arc, "TestObject");

    std::cout << Arc.dump(4) << std::endl;

    TestArrayStr Compare;
    EXPECT_STRNE(Compare.Arr[0].c_str(), "Hello");
    Compare.Deserialize(Arc, "TestObject");

    EXPECT_EQ(Compare.Arr.size(), Obj.Arr.size());
    EXPECT_STREQ(Compare.Arr[0].c_str(), Obj.Arr[0].c_str());
    EXPECT_STREQ(Compare.Arr[1].c_str(), Obj.Arr[1].c_str());
}

TEST(Reflection, ArrayArrayProperty)
{
    TestArrayArray Obj;
    Obj.Arr = { { 1u }, { 2u }};

    Json Arc;
    Obj.Serialize(Arc, "TestObject");

    std::cout << Arc.dump(4) << std::endl;

    TestArrayArray Compare;
    EXPECT_NE(Compare.Arr[0][0], 1u);
    Compare.Deserialize(Arc, "TestObject");

    EXPECT_EQ(Compare.Arr[0].size(), Obj.Arr[0].size());
    EXPECT_EQ(Compare.Arr[0][0], Obj.Arr[0][0]);
    EXPECT_EQ(Compare.Arr[1].size(), Obj.Arr[1].size());
    EXPECT_EQ(Compare.Arr[1][0], Obj.Arr[1][0]);
}

TEST(Reflection, ObjectArrayProperty)
{
    TestArrayObj Obj;
    Obj.Arr = { TestOuter{ .InnerObject{ .SomeFloat = 2.71f } }, TestOuter{ .InnerObject{ .SomeFloat = 3.14f } } };

    Json Arc;
    Obj.Serialize(Arc, "TestObject");

    std::cout << Arc.dump(4) << std::endl;

    TestArrayObj Compare;
    EXPECT_NE(Compare.Arr[0].InnerObject.SomeFloat, Obj.Arr[0].InnerObject.SomeFloat);
    Compare.Deserialize(Arc, "TestObject");

    EXPECT_EQ(Compare.Arr.size(), Obj.Arr.size());
    EXPECT_EQ(Compare.Arr[0].InnerObject.SomeFloat, Obj.Arr[0].InnerObject.SomeFloat);
    EXPECT_EQ(Compare.Arr[1].InnerObject.SomeFloat, Obj.Arr[1].InnerObject.SomeFloat);
}

TEST(Reflection, VectorProperty)
{
    TestVectorObj Obj{};
    Obj.vec2 = { .5f, .6f, };
    Obj.ivec2 = { 5, 6, };

    Json Arc;
    Obj.Serialize(Arc, "TestObject");

    std::cout << Arc.dump(4) << std::endl;

    TestVectorObj Compare;
    EXPECT_NE(Compare.vec2.x, .5f);
    Compare.Deserialize(Arc, "TestObject");

    EXPECT_EQ(Compare.vec2.x, Obj.vec2.x);
    EXPECT_EQ(Compare.vec2.y, Obj.vec2.y);
    EXPECT_EQ(Compare.ivec2.x, Obj.ivec2.x);
    EXPECT_EQ(Compare.ivec2.y, Obj.ivec2.y);
}