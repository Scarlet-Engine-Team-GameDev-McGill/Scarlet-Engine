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
        virtual void SerializeProperty(const ObjectType* Object, Archive&) const = 0;
        virtual void DeserializeProperty(ObjectType* OutObject, Archive&) const = 0;
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

        virtual void SerializeProperty(const ObjectType* Object, Archive& Arc) const override
        {
            MemberType Value = GetValue(*Object);
            GetPropertyType()->Serialize(reinterpret_cast<const byte_t*>(&Value), Arc);
        }

        virtual void DeserializeProperty(ObjectType* OutObject, Archive& Arc) const override
        {
            MemberType* Ptr = &(OutObject->*MemberPtr);
            GetPropertyType()->Deserialize(reinterpret_cast<byte_t*>(Ptr), Arc);
        }
    private:
        MemberPointer MemberPtr;
    };
}