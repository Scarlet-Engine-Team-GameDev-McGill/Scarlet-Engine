#pragma once

#include "TypeInfo.h"

namespace ScarletEngine::Reflection
{
    template <typename ObjectType>
    class PropertyOfType
    {
    public:
        PropertyOfType(String Name) : PropertyName(std::move(Name)) {}
        virtual ~PropertyOfType() {}

        const String& GetPropertyName() const { return PropertyName; };
        virtual const TypeInfo* GetPropertyType() const = 0;
        virtual void SerializeProperty(const ObjectType* Object, Json& Arc) const = 0;
        virtual void DeserializeProperty(ObjectType* OutObject, Json& Arc) const = 0;
    protected:
        String PropertyName;
    };

    template <typename ObjectType, typename MemberType>
    class MemberProperty final : public PropertyOfType<ObjectType>
    {
    public:
        using MemberPointer = MemberType ObjectType::*;

        MemberProperty(String Name, MemberPointer InMemberPtr)
            : PropertyOfType<ObjectType>(Name)
        {
            MemberPtr = InMemberPtr;
        }

        const MemberType& GetValue(const ObjectType& Object) const
        {
            return Object.*MemberPtr;
        }

        void SetValue(ObjectType& Object, MemberType Value) const
        {
            Object.*MemberPtr = std::move(Value);
        }

        virtual const TypeInfo* GetPropertyType() const override { return GetTypeInfo<MemberType>(); }

        virtual void SerializeProperty(const ObjectType* Object, Json& Arc) const override
        {
            MemberType Value = GetValue(*Object);
            GetPropertyType()->Serialize(reinterpret_cast<const byte_t*>(&Value), Arc, PropertyOfType<ObjectType>::GetPropertyName().c_str());
        }

        virtual void DeserializeProperty(ObjectType* OutObject, Json& Arc) const override
        {
            MemberType* Ptr = &(OutObject->*MemberPtr);
            GetPropertyType()->Deserialize(reinterpret_cast<byte_t*>(Ptr), Arc, PropertyOfType<ObjectType>::GetPropertyName().c_str());
        }
    private:
        MemberPointer MemberPtr;
    };
}