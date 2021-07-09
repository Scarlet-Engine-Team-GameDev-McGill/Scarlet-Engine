#include "Reflection/TypeInfo.h"

#include "Serialization/Archive.h"

namespace ScarletEngine::Reflection
{
#define DEFINE_INTEGRAL_TYPE(T, bIsFloat, bIsSigned)    \
    template <> const TypeInfo* BuildTypeInfo<T>()        \
    {\
        if constexpr (bIsFloat) \
        {\
            static const FloatTypeInfo Type(#T, sizeof(T)); return &Type;\
        }\
        else\
        {\
            static const IntegerTypeInfo Type(#T, sizeof(T), bIsSigned); return &Type;\
        }\
    }

    DEFINE_INTEGRAL_TYPE(int8_t, false, true)
    DEFINE_INTEGRAL_TYPE(int16_t, false, true)
    DEFINE_INTEGRAL_TYPE(int32_t, false, true)
    DEFINE_INTEGRAL_TYPE(int64_t, false, true)
    DEFINE_INTEGRAL_TYPE(uint8_t, false, false)
    DEFINE_INTEGRAL_TYPE(uint16_t, false, false)
    DEFINE_INTEGRAL_TYPE(uint32_t, false, false)
    DEFINE_INTEGRAL_TYPE(uint64_t, false, false)
    DEFINE_INTEGRAL_TYPE(float, true, true)
    DEFINE_INTEGRAL_TYPE(double, true, true)

    void IntegerTypeInfo::Serialize(const byte_t* Location, Archive& Arc) const
    {
        if (bSigned)
        {
            switch (Size)
            {
            case 1: Arc << (*reinterpret_cast<const int8_t*>(Location)); return;
            case 2: Arc << (*reinterpret_cast<const int16_t*>(Location)); return;
            case 4: Arc << (*reinterpret_cast<const int32_t*>(Location)); return;
            case 8: Arc << (*reinterpret_cast<const int64_t*>(Location)); return;
            default: check(false);
            }
        }
        else
        {
            switch (Size)
            {
            case 1: Arc << (*reinterpret_cast<const uint8_t*>(Location)); return;
            case 2: Arc << (*reinterpret_cast<const uint16_t*>(Location)); return;
            case 4: Arc << (*reinterpret_cast<const uint32_t*>(Location)); return;
            case 8: Arc << (*reinterpret_cast<const uint64_t*>(Location)); return;
            default: check(false);
            }
        }
    }

    void IntegerTypeInfo::Deserialize(byte_t* Location, Archive& Arc) const
    {
        if (bSigned)
        {
            switch (Size)
            {
            case 1: Arc >> (*reinterpret_cast<int8_t*>(Location)); return;
            case 2: Arc >> (*reinterpret_cast<int16_t*>(Location)); return;
            case 4: Arc >> (*reinterpret_cast<int32_t*>(Location)); return;
            case 8: Arc >> (*reinterpret_cast<int64_t*>(Location)); return;
            default: check(false); // Unknown int size
            }
        }
        else
        {
            switch (Size)
            {
            case 1: Arc >> (*reinterpret_cast<uint8_t*>(Location)); return;
            case 2: Arc >> (*reinterpret_cast<uint16_t*>(Location)); return;
            case 4: Arc >> (*reinterpret_cast<uint32_t*>(Location)); return;
            case 8: Arc >> (*reinterpret_cast<uint64_t*>(Location)); return;
            default: check(false); // Unknown int size
            }
        }
    }

    void FloatTypeInfo::Serialize(const byte_t* Location, Archive& Arc) const
    {
        if (Size == 4)
        {
            Arc << (*reinterpret_cast<const float*>(Location));
        }
        else if (Size == 8)
        {
            Arc << (*reinterpret_cast<const double*>(Location));
        }
        else
        {
            check(false); // Unknown float size
        }
    }

    void FloatTypeInfo::Deserialize(byte_t* Location, Archive& Arc) const
    {
        if (Size == 4)
        {
            Arc >> (*reinterpret_cast<float*>(Location));
        }
        else if (Size == 8)
        {
            Arc >> (*reinterpret_cast<double*>(Location));
        }
        else
        {
            check(false); // Unknown float size
        }
    }

    const StringTypeInfo* StringTypeInfo::Get()
    {
        static const StringTypeInfo TypeInfo;
        return &TypeInfo;
    }

    void StringTypeInfo::Serialize(const String& Object, Archive& Arc) const
    {
        Arc << Object;
    }

    void StringTypeInfo::Deserialize(String& Object, Archive& Arc) const
    {
        Arc >> Object;
    }

    const String& StringTypeInfo::TypeName() const
    {
        static const String Name = "String";
        return Name;
    }
}
