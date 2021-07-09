#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine::Reflection
{
    class TypeInfo
    {
    public:
        virtual ~TypeInfo() {}
        // #todo_core: replace this once generic Archive type is ready
        virtual void Deserialize(byte_t* Location, Archive&) const = 0;
        virtual void Serialize(const byte_t* Location, Archive&) const = 0;

        virtual void Construct(byte_t* Location) const = 0;
        virtual void Destruct(byte_t* Location) const = 0;

        virtual const String& TypeName() const = 0;
        virtual size_t TypeSize() const = 0;
    protected:
        TypeInfo() {}
    };

    template <typename T>
    class TypeInfoOf : public TypeInfo
    {
    public:
        virtual void Deserialize(T& Object, Archive&) const = 0;
        virtual void Serialize(const T& Object, Archive&) const = 0;

        // Specifically do not override the interface
        virtual void Deserialize(byte_t* Location, Archive& Arc) const override
        {
            this->Deserialize(*reinterpret_cast<T*>(Location), Arc);
        }
        virtual void Serialize(const byte_t* Location, Archive& Arc) const override
        {
            this->Serialize(*reinterpret_cast<const T*>(Location), Arc);
        }
        virtual void Construct(byte_t* Location) const override
        {
            // #todo_core: use ScarNew here once it supports placement new
            new(Location) T;
        }
        virtual void Destruct(byte_t* Location) const override
        {
            reinterpret_cast<T*>(Location)->~T();
        }

        virtual size_t TypeSize() const override { return sizeof(T); }
    };

    class BasicTypeInfo : public TypeInfo
    {
    public:
        BasicTypeInfo(String InTypeName, size_t InSize, size_t InComponentSize, bool bInSigned) : Name(InTypeName), Size(InSize), ComponentSize(InComponentSize), bSigned(bInSigned) {}

        // Construct and destruct by memsetting to 0
        virtual void Construct(byte_t* Location) const override { std::memset(Location, 0, TypeSize()); }
        virtual void Destruct(byte_t* Location) const override { std::memset(Location, 0, TypeSize()); }

        virtual const String& TypeName() const override { return Name; }
        virtual size_t TypeSize() const override { return Size; }
        size_t ComponentNum() const { return Size / ComponentSize; }
    protected:
        String Name;
        size_t Size;
        size_t ComponentSize;
        bool bSigned;
    };

    class IntegerTypeInfo : public BasicTypeInfo
    {
    public:
        IntegerTypeInfo(String InTypeName, size_t InSize, bool bInSigned = true) : BasicTypeInfo(InTypeName, InSize, InSize, bInSigned) {}
        virtual void Serialize(const byte_t* Location, Archive&) const override;
        virtual void Deserialize(byte_t* Location, Archive&) const override;
    };

    class FloatTypeInfo : public BasicTypeInfo
    {
    public:
        FloatTypeInfo(String InTypeName, size_t InSize) : BasicTypeInfo(InTypeName, InSize, InSize, true) {}
        virtual void Serialize(const byte_t* Location, Archive&) const override;
        virtual void Deserialize(byte_t* Location, Archive&) const override;
    };

    class StringTypeInfo : public TypeInfoOf<String>
    {
    public:
        static const StringTypeInfo* Get();

        void Serialize(const String& Object, Archive&) const override;
        void Deserialize(String& Object, Archive&) const override;
        virtual const String& TypeName() const override;
    };

// ---------------------------------------------------------------------------------------------------------------------

    template <typename T> const TypeInfo* BuildTypeInfo();

#define DECLARE_TYPE(T) template<> const TypeInfo* BuildTypeInfo<T>();
    DECLARE_TYPE(int8_t)
    DECLARE_TYPE(int16_t)
    DECLARE_TYPE(int32_t)
    DECLARE_TYPE(int64_t)
    DECLARE_TYPE(uint8_t)
    DECLARE_TYPE(uint16_t)
    DECLARE_TYPE(uint32_t)
    DECLARE_TYPE(uint64_t)
    DECLARE_TYPE(float)
    DECLARE_TYPE(double)

    template <typename T> struct TypeInfoBuilder {};

    template <> struct TypeInfoBuilder<String>
    {
        static const StringTypeInfo* Build() { return StringTypeInfo::Get(); }
    };

    template <typename T>
    const TypeInfo* BuildTypeInfo()
    {
        return TypeInfoBuilder<T>::Build();
    }

    struct CheckHasTypeInfo
    {
        template <typename T, const TypeInfo*(*)() = T::BuildTypeInfo>
        struct Check {};
    };

    template <typename T>
    struct HasReflectionData : Utils::HasMember<T, CheckHasTypeInfo> {};

    template <typename T>
    typename std::enable_if<HasReflectionData<T>::Value, const TypeInfo*>::type
    GetTypeInfo() { return T::BuildTypeInfo(); }

    template <typename T>
    typename std::enable_if<!HasReflectionData<T>::Value, const TypeInfo*>::type
    GetTypeInfo() { return BuildTypeInfo<T>(); }

}