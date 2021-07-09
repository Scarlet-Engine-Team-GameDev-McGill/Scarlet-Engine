#pragma once

#include "CoreMinimal.h"
#include "TypeInfo.h"

namespace ScarletEngine::Reflection
{
    template <typename ArrayType>
    class ArrayTypeInfo : public TypeInfoOf<ArrayType>
    {
    public:
        using ElemType = typename ArrayType::value_type;

        static const TypeInfo* GetElementTypeInfo() { return GetTypeInfo<ElemType>(); }

        void Serialize(const ArrayType& Arr, Json& Arc, const char* Label) const override
        {
            Json JsonArr = Json::array();
            const TypeInfo* ElementTypeInfo = GetElementTypeInfo();
            for (size_t Index = 0; Index < Arr.size(); ++Index)
            {
                ElementTypeInfo->Serialize(reinterpret_cast<const byte_t*>(&Arr[Index]), JsonArr, nullptr);
            }
            
            if (Arc.is_array())
            {
                Arc.emplace_back(JsonArr);
            }
            else
            {
                Arc[Label] = JsonArr;
            }
        }

        void Deserialize(ArrayType& OutArray, Json& Arc, const char* Label, size_t Index = 0) const override
        {
            if (Arc.is_array())
            {
                Json JsonArr = Arc.at(Index);
                check(JsonArr.is_array());

                const size_t Size = JsonArr.size();
                OutArray.resize(Size);
                const TypeInfo* ElementTypeInfo = GetElementTypeInfo();
                for (size_t InnerIndex = 0; InnerIndex < Size; ++InnerIndex)
                {
                    ElementTypeInfo->Deserialize(reinterpret_cast<byte_t*>(&OutArray[InnerIndex]), JsonArr, nullptr, InnerIndex);
                }
            }
            else
            {
                Json JsonArr = Arc[Label];
                check(JsonArr.is_array());

                const size_t Size = JsonArr.size();
                OutArray.resize(Size);
                const TypeInfo* ElementTypeInfo = GetElementTypeInfo();
                for (size_t InnerIndex = 0; InnerIndex < Size; ++InnerIndex)
                {
                    ElementTypeInfo->Deserialize(reinterpret_cast<byte_t*>(&OutArray[InnerIndex]), JsonArr, nullptr, InnerIndex);
                }
            }
        }

        virtual const String& TypeName() const override
        {
            static String TypeName = "Array<" + GetElementTypeInfo()->TypeName() + ">";
            return TypeName;
        }

    };

    template <typename ElemType>
    struct TypeInfoBuilder<Array<ElemType>>
    {
        static const TypeInfo* Build()
        {
            static const ArrayTypeInfo<Array<ElemType>> Type;
            return &Type;
        }
    };
}