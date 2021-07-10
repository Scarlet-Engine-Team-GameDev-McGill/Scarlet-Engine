#include "Reflection/TypeInfo.h"

namespace ScarletEngine::Reflection
{
#define DEFINE_INTEGRAL_TYPE(T, bIsFloat, bIsSigned)\
    template <> const TypeInfo* BuildTypeInfo<T>()\
    {\
        if constexpr (bIsFloat)\
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

#define DEFINE_VECTOR_TYPE(T)\
    template <> const TypeInfo* BuildTypeInfo<T>()\
    {\
        static const VectorTypeInfo<T> Type(#T); return &Type;\
    }

    DEFINE_VECTOR_TYPE(glm::vec2)
    DEFINE_VECTOR_TYPE(glm::vec3)
    DEFINE_VECTOR_TYPE(glm::vec4)
    DEFINE_VECTOR_TYPE(glm::ivec2)
    DEFINE_VECTOR_TYPE(glm::ivec3)
    DEFINE_VECTOR_TYPE(glm::ivec4)
    
    void IntegerTypeInfo::Serialize(const byte_t* Location, Json& Arc, const char* Label) const
    {
        // When label is nullptr we are inside an array
        if (Arc.is_array())
        {
            if (bSigned)
            {
                switch (Size)
                {
                case 1: Arc.push_back(*reinterpret_cast<const int8_t*>(Location)); return;
                case 2: Arc.push_back(*reinterpret_cast<const int16_t*>(Location)); return;
                case 4: Arc.push_back(*reinterpret_cast<const int32_t*>(Location)); return;
                case 8: Arc.push_back(*reinterpret_cast<const int64_t*>(Location)); return;
                default: check(false); return;
                }
            }
            else
            {
                switch (Size)
                {
                case 1: Arc.push_back(*reinterpret_cast<const uint8_t*>(Location)); return;
                case 2: Arc.push_back(*reinterpret_cast<const uint16_t*>(Location)); return;
                case 4: Arc.push_back(*reinterpret_cast<const uint32_t*>(Location)); return;
                case 8: Arc.push_back(*reinterpret_cast<const uint64_t*>(Location)); return;
                default: check(false); return;
                }
            }
        }
        else
        {
            if (bSigned)
            {
                switch (Size)
                {
                case 1: Arc[Label] = (*reinterpret_cast<const int8_t*>(Location)); return;
                case 2: Arc[Label] = (*reinterpret_cast<const int16_t*>(Location)); return;
                case 4: Arc[Label] = (*reinterpret_cast<const int32_t*>(Location)); return;
                case 8: Arc[Label] = (*reinterpret_cast<const int64_t*>(Location)); return;
                default: check(false);
                }
            }
            else
            {
                switch (Size)
                {
                case 1: Arc[Label] = (*reinterpret_cast<const uint8_t*>(Location)); return;
                case 2: Arc[Label] = (*reinterpret_cast<const uint16_t*>(Location)); return;
                case 4: Arc[Label] = (*reinterpret_cast<const uint32_t*>(Location)); return;
                case 8: Arc[Label] = (*reinterpret_cast<const uint64_t*>(Location)); return;
                default: check(false);
                }
            }
        }
    }

    void IntegerTypeInfo::Deserialize(byte_t* Location, Json& Arc, const char* Label, size_t Index) const
    {
        if (Arc.is_array())
        {
            if (bSigned)
            {
                switch (Size)
                {
                case 1: (*reinterpret_cast<int8_t*>(Location)) = Arc.at(Index); return;
                case 2: (*reinterpret_cast<int16_t*>(Location)) = Arc.at(Index); return;
                case 4: (*reinterpret_cast<int32_t*>(Location)) = Arc.at(Index); return;
                case 8: (*reinterpret_cast<int64_t*>(Location)) = Arc.at(Index); return;
                default: check(false); return; // Unknown int size
                }
            }
            else
            {
                switch (Size)
                {
                case 1: (*reinterpret_cast<uint8_t*>(Location)) = Arc.at(Index); return;
                case 2: (*reinterpret_cast<uint16_t*>(Location)) = Arc.at(Index); return;
                case 4: (*reinterpret_cast<uint32_t*>(Location)) = Arc.at(Index); return;
                case 8: (*reinterpret_cast<uint64_t*>(Location)) = Arc.at(Index); return;
                default: check(false); return; // Unknown int size
                }
            }
        }
        else
        {
            if (bSigned)
            {
                switch (Size)
                {
                case 1: (*reinterpret_cast<int8_t*>(Location)) = Arc[Label]; return;
                case 2: (*reinterpret_cast<int16_t*>(Location)) = Arc[Label]; return;
                case 4: (*reinterpret_cast<int32_t*>(Location)) = Arc[Label]; return;
                case 8: (*reinterpret_cast<int64_t*>(Location)) = Arc[Label]; return;
                default: check(false); return; // Unknown int size
                }
            }
            else
            {
                switch (Size)
                {
                case 1: (*reinterpret_cast<uint8_t*>(Location)) = Arc[Label]; return;
                case 2: (*reinterpret_cast<uint16_t*>(Location)) = Arc[Label]; return;
                case 4: (*reinterpret_cast<uint32_t*>(Location)) = Arc[Label]; return;
                case 8: (*reinterpret_cast<uint64_t*>(Location)) = Arc[Label]; return;
                default: check(false); return; // Unknown int size
                }
            }
        }
    }

    void FloatTypeInfo::Serialize(const byte_t* Location, Json& Arc, const char* Label) const
    {
        if (Arc.is_array())
        {
            if (Size == 4)
            {
                Arc.push_back(*reinterpret_cast<const float*>(Location));
            }
            else if (Size == 8)
            {
                Arc.push_back(*reinterpret_cast<const double*>(Location));
            }
            else
            {
                check(false); // Unknown float size
            }
        }
        else
        {
            if (Size == 4)
            {
                Arc[Label] = (*reinterpret_cast<const float*>(Location));
            }
            else if (Size == 8)
            {
                Arc[Label] = (*reinterpret_cast<const double*>(Location));
            }
            else
            {
                check(false); // Unknown float size
            }
        }
    }

    void FloatTypeInfo::Deserialize(byte_t* Location, Json& Arc, const char* Label, size_t Index) const
    {
        if (Arc.is_array())
        {
            if (Size == 4)
            {
                (*reinterpret_cast<float*>(Location)) = Arc.at(Index);
            }
            else if (Size == 8)
            {
                (*reinterpret_cast<double*>(Location)) = Arc.at(Index);
            }
            else
            {
                check(false); // Unknown float size
            }
        }
        else
        {
            if (Size == 4)
            {
                (*reinterpret_cast<float*>(Location)) = Arc[Label];
            }
            else if (Size == 8)
            {
                (*reinterpret_cast<double*>(Location)) = Arc[Label];
            }
            else
            {
                check(false); // Unknown float size
            }
        }
    }

    const StringTypeInfo* StringTypeInfo::Get()
    {
        static const StringTypeInfo TypeInfo;
        return &TypeInfo;
    }

    void StringTypeInfo::Serialize(const String& Str, Json& Arc, const char* Label) const
    {
        if (Arc.is_array())
        {
            Arc.emplace_back(Str);
        }
        else
        {
            Arc[Label] = Str;
        }
    }

    void StringTypeInfo::Deserialize(String& Str, Json& Arc, const char* Label, size_t Index) const
    {
        if (Arc.is_array())
        {
            Str = Arc.at(Index);
        }
        else
        {
            Str = Arc[Label];
        }
    }

    const String& StringTypeInfo::TypeName() const
    {
        static const String Name = "String";
        return Name;
    }
}
