#pragma once

#include "CoreMinimal.h"

#include "Array.h"
#include "Property.h"

namespace ScarletEngine::Reflection
{
    template <typename T>
    class ObjectTypeInfo : public TypeInfoOf<T>
    {
    public:
        ObjectTypeInfo(String InName) : Name(InName) {}

        void SetProperties(Array<PropertyOfType<T>*> InProperties)
        {
            Properties = std::move(InProperties);
        }

        virtual void Serialize(const T& Object, Json& Arc, const char* Label) const override
        {
            Json JsonObj;
            for (auto& Property : Properties)
            {
                Property->SerializeProperty(&Object, JsonObj);
            }
            
            if (Arc.is_array())
            {
                Arc.emplace_back(JsonObj);
            }
            else
            {
                Arc[Label] = JsonObj;
            }
        }

        virtual void Deserialize(T& Object, Json& Arc, const char* Label, size_t Index = 0) const override
        {
            Json JsonObj;
            if (Arc.is_array())
            {
                JsonObj = Arc.at(Index);
            }
            else
            {
                JsonObj = Arc[Label];
            }

            for (auto& Property : Properties)
            {
                Property->DeserializeProperty(&Object, JsonObj, Index);
            }
        }

        virtual const String& TypeName() const override { return Name; }
    protected:
        // We will allow these to leak at exit since this object will never be destroyed at runtime.
        Array<PropertyOfType<T>*> Properties;
        String Name;
    };

    template <typename ObjectType>
    class ObjectReflectionInfoBuilder
    {
    public:
        virtual ~ObjectReflectionInfoBuilder() {}

        template <typename AttributeType>
        void Property(AttributeType ObjectType::* AttributePtr, String PropertyName)
        {
            Properties.push_back(new MemberProperty<ObjectType, AttributeType>(std::move(PropertyName), AttributePtr));
        }

        virtual void Define() = 0;

        ObjectTypeInfo<ObjectType> Build()
        {
            Define();
            ObjectTypeInfo<ObjectType> Type(std::move(Name));
            Type.SetProperties(std::move(Properties));
            return Type;
        }
    protected:
        Array<PropertyOfType<ObjectType>*> Properties;
        String Name;
    };
}

#define REFLECTION() \
    public: \
        static const Reflection::TypeInfo* BuildTypeInfo();\
        void Construct() { BuildTypeInfo()->Construct(reinterpret_cast<byte_t*>(this)); } \
        void Destruct() { BuildTypeInfo()->Destruct(reinterpret_cast<byte_t*>(this)); } \
        void Serialize(Json& Arc, const char* Label) const\
        {\
            Reflection::GCurrentPointerMap = &Arc["PointerMap"];\
            BuildTypeInfo()->Serialize(reinterpret_cast<const byte_t*>(this), Arc, Label);\
            Reflection::GCurrentPointerMap = nullptr;\
        }\
        void Deserialize(Json& Arc, const char* Label)\
        {\
            Reflection::GCurrentPointerMap = &Arc["PointerMap"];\
            BuildTypeInfo()->Deserialize(reinterpret_cast<byte_t*>(this), Arc, Label);\
            Reflection::GCurrentPointerMap = nullptr;\
        }


#define BEGIN_REFLECTION_INFO(TypeName)           \
    const Reflection::TypeInfo* TypeName::BuildTypeInfo()   \
    {                                       \
        static struct ObjectReflectionInfoBuilderImpl : public Reflection::ObjectReflectionInfoBuilder<TypeName>   \
        {\
            virtual void Define() override  \
            {                               \
                Name = #TypeName;

#define END_REFLECTION_INFO() \
            }           \
        } Builder;      \
        static const auto Type = Builder.Build();\
        return &Type;   \
    }
