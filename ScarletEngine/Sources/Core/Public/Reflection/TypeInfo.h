#pragma once

#include "CoreMinimal.h"

namespace ScarletEngine::Reflection
{
    extern Json* GCurrentPointerMap;

    class TypeInfo
    {
    public:
        virtual ~TypeInfo() {}
        // #todo_core: replace this once a more generic Archive type is ready
        virtual void Deserialize(byte_t* Location, Json& Arc, const char* Label, size_t Index = 0) const = 0;
        virtual void Serialize(const byte_t* Location, Json& Arc, const char* Label) const = 0;

        virtual void Construct(byte_t* Location) const = 0;
        virtual void Destruct(byte_t* Location) const = 0;

        virtual const String& TypeName() const = 0;
        virtual size_t TypeSize() const = 0;
    protected:
        TypeInfo() {}
    };

    template <typename T> struct TypeInfoBuilder {};

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

    template <typename T>
    class TypeInfoOf : public TypeInfo
    {
    public:
        virtual void Deserialize(T& Object, Json& Arc, const char* Label, size_t Index = 0) const = 0;
        virtual void Serialize(const T& Object, Json& Arc, const char* Label) const = 0;

        // Specifically do not override the interface
        virtual void Deserialize(byte_t* Location, Json& Arc, const char* Label, size_t Index = 0) const override
        {
            this->Deserialize(*reinterpret_cast<T*>(Location), Arc, Label, Index);
        }
        virtual void Serialize(const byte_t* Location, Json& Arc, const char* Label) const override
        {
            this->Serialize(*reinterpret_cast<const T*>(Location), Arc, Label);
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
        size_t GetComponentNum() const { return Size / ComponentSize; }
    protected:
        const String Name;
        const size_t Size;
        const size_t ComponentSize;
        const bool bSigned;
    };

    class IntegerTypeInfo : public BasicTypeInfo
    {
    public:
        IntegerTypeInfo(String InTypeName, size_t InSize, bool bInSigned = true) : BasicTypeInfo(InTypeName, InSize, InSize, bInSigned) {}
        virtual void Serialize(const byte_t* Location, Json& Arc, const char* Label) const override;
        virtual void Deserialize(byte_t* Location, Json& Arc, const char* Label, size_t Index = 0) const override;
    };

    class FloatTypeInfo : public BasicTypeInfo
    {
    public:
        FloatTypeInfo(String InTypeName, size_t InSize) : BasicTypeInfo(InTypeName, InSize, InSize, true) {}
        virtual void Serialize(const byte_t* Location, Json& Arc, const char* Label) const override;
        virtual void Deserialize(byte_t* Location, Json& Arc, const char* Label, size_t Index = 0) const override;
    };

    class StringTypeInfo : public TypeInfoOf<String>
    {
    public:
        static const StringTypeInfo* Get();

        virtual void Serialize(const String& Object, Json& Arc, const char* Label) const override;
        virtual void Deserialize(String& Object, Json& Arc, const char* Label, size_t Index = 0) const override;
        virtual const String& TypeName() const override;
    };

    template <typename VectorType>
    class VectorTypeInfo : public BasicTypeInfo
    {
    public:
        using ComponentType = typename VectorType::value_type;
        using LengthType = typename VectorType::length_type;

        VectorTypeInfo(String InTypeName) : BasicTypeInfo(InTypeName, sizeof(VectorType), sizeof(ComponentType), true) {}

        virtual void Serialize(const byte_t* Location, Json& Arc, const char* Label) const override
        {
            Json JsonObj = Json::object();

            for (LengthType ComponentIndex = 0; ComponentIndex < GetComponentNum(); ++ComponentIndex)
            {
                const VectorType* Vector = reinterpret_cast<const VectorType*>(Location);

                JsonObj[VectorComponentNames[ComponentIndex]] = (*Vector)[ComponentIndex];
            }

            if (Arc.is_array())
            {
                Arc.push_back(JsonObj);
            }
            else
            {
                Arc[Label] = JsonObj;
            }
        }

        virtual void Deserialize(byte_t* Location, Json& Arc, const char* Label, size_t Index = 0) const override
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

            for (LengthType ComponentIndex = 0; ComponentIndex < GetComponentNum(); ++ComponentIndex)
            {
                VectorType* Vector = reinterpret_cast<VectorType*>(Location);

                (*Vector)[ComponentIndex] = JsonObj[VectorComponentNames[ComponentIndex]];
            }
        }
    protected:
        inline static const char* const VectorComponentNames[] = { "x", "y", "z", "w" };
    };

    class PointerTypeInfo : public TypeInfo
    {
    public:
        virtual void Construct(byte_t* /*Location*/) const override {}
        virtual void Destruct(byte_t* /*Location*/) const override {}

        virtual const String& TypeName() const override { return Name; }
        virtual size_t TypeSize() const override { return sizeof(void*); }
    protected:
        /** Global pointer map used for deserialization. Maps a pointers unique id to its current location in memory. */
        static UnorderedMap<uint64_t, void*> PointerIDMap;
        const String Name = "Pointer";
    };

    template <typename PointerToType>
    class PointerTypeInfoOf : public PointerTypeInfo
    {
    public:
        virtual void Serialize(const byte_t* Location, Json& Arc, const char* Label) const override
        {
            // #todo: implement for array of pointers
            const PointerToType** PointerToPointer = (const PointerToType**)(Location);
            Json& PointerMap = *GCurrentPointerMap;
            const uint64_t PointerID = (uintptr_t)(*PointerToPointer);
            const auto PointerIDString = std::to_string(PointerID);

            // If the pointer doesn't already exist, serialize it otherwise do nothing.
            if (PointerMap.find(PointerIDString.c_str()) == PointerMap.end())
            {
                const PointerToType* ObjectPtr = *PointerToPointer;
                if (ObjectPtr == nullptr)
                {
                    PointerMap[PointerIDString.c_str()] = nullptr;
                }
                else
                {
                    GetTypeInfo<PointerToType>()->Serialize(reinterpret_cast<const byte_t*>(ObjectPtr), PointerMap, PointerIDString.c_str());
                }
            }
            Arc[Label] = PointerID;
        }
        virtual void Deserialize(byte_t* Location, Json& Arc, const char* Label, size_t Index = 0) const override
        {
            // #todo: implement for array of pointers
            PointerToType** PointerToPointer = (PointerToType**)(Location);
            Json& PointerMap = *GCurrentPointerMap;
            const uint64_t PointerID = Arc[Label];
            const auto PointerIDString = std::to_string(PointerID);

            if (const auto It = PointerIDMap.find(PointerID); It != PointerIDMap.end())
            {
                // Set the pointer we are pointing to to point to the value stored in the map
                *PointerToPointer = reinterpret_cast<PointerToType*>(It->second);
            }
            else
            {
                PointerToType* ObjectPtr = ScarNew(PointerToType);
                GetTypeInfo<PointerToType>()->Deserialize(reinterpret_cast<byte_t*>(ObjectPtr), PointerMap, PointerIDString.c_str(), Index);
                PointerIDMap[PointerID] = reinterpret_cast<void*>(ObjectPtr);
                *PointerToPointer = ObjectPtr;
            }
        }
    };

// ---------------------------------------------------------------------------------------------------------------------

    template <typename T> const TypeInfo* BuildTypeInfo();

#define DECLARE_INTEGRAL_TYPE(T) template<> const TypeInfo* BuildTypeInfo<T>();
    DECLARE_INTEGRAL_TYPE(int8_t)
    DECLARE_INTEGRAL_TYPE(int16_t)
    DECLARE_INTEGRAL_TYPE(int32_t)
    DECLARE_INTEGRAL_TYPE(int64_t)
    DECLARE_INTEGRAL_TYPE(uint8_t)
    DECLARE_INTEGRAL_TYPE(uint16_t)
    DECLARE_INTEGRAL_TYPE(uint32_t)
    DECLARE_INTEGRAL_TYPE(uint64_t)
    DECLARE_INTEGRAL_TYPE(float)
    DECLARE_INTEGRAL_TYPE(double)
    DECLARE_INTEGRAL_TYPE(glm::vec2)
    DECLARE_INTEGRAL_TYPE(glm::vec3)
    DECLARE_INTEGRAL_TYPE(glm::vec4)
    DECLARE_INTEGRAL_TYPE(glm::ivec2)
    DECLARE_INTEGRAL_TYPE(glm::ivec3)
    DECLARE_INTEGRAL_TYPE(glm::ivec4)

    template <> struct TypeInfoBuilder<String>
    {
        static const StringTypeInfo* Build() { return StringTypeInfo::Get(); }
    };

    template <typename T> struct TypeInfoBuilder<T*>
    {
        static const PointerTypeInfo* Build() { static PointerTypeInfoOf<T> TypeInfo; return &TypeInfo; }
    };

}