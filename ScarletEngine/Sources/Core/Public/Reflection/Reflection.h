#pragma once

#include "CoreMinimal.h"

#include "Property.h"

namespace ScarletEngine::Reflection
{
    template <typename T>
    class ObjectTypeInfo : public TypeInfoOf<T>
    {
    public:
        ObjectTypeInfo(String InName) : Name(InName) {}
        virtual ~ObjectTypeInfo()
        {
            for (PropertyOfType<T>* Property : Properties)
            {
                delete Property;
            }
        }

        void SetProperties(Array<PropertyOfType<T>*> InProperties)
        {
            Properties = std::move(InProperties);
        }

        virtual void Serialize(const T& Object, Archive& Arc) const override
        {
            for (auto& Property : Properties)
            {
                // #todo_core: Arc.FindPropertyByName(Property->GetName())
                Property->SerializeProperty(&Object, Arc);
            }
        }

        virtual void Deserialize(T& Object, Archive& Arc) const override
        {
            for (auto& Property : Properties)
            {
                // #todo_core: Arc.FindPropertyByName(Property->GetName())
                Property->DeserializeProperty(&Object, Arc);
            }
        }

        virtual const String& TypeName() const override { return Name; }
    protected:
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

#define REFLECTION() public: static const Reflection::TypeInfo* BuildTypeInfo();

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
