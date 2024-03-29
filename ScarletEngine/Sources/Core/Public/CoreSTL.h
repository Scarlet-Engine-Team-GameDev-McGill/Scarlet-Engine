#pragma once

#include "CorePCH.h"
#include "Memory/Memory.h"

namespace ScarletEngine
{
    using byte_t = uint8_t;

    template <typename T, typename Alloc = GlobalAllocator<T>>
    using Array = std::vector<T, Alloc>;

    template <typename T, size_t Size>
    using StaticArray = std::array<T, Size>;

    template <typename Key, typename Val, typename Pred = std::less<Key>, typename Alloc = GlobalAllocator<std::pair<const Key, Val>>>
    using Map = std::map<Key, Val, Pred, Alloc>;

    template <typename Key, typename Val, typename Hash = std::hash<Key>, typename KeyCmp = std::equal_to<Key>, typename Alloc = GlobalAllocator<std::pair<const Key, Val>>>
    using UnorderedMap = std::unordered_map<Key, Val, Hash, KeyCmp, Alloc>;

    template <typename Key, typename KeyCmp = std::less<Key>, typename Alloc = GlobalAllocator<Key >>
    using Set = std::set<Key, KeyCmp, Alloc>;

    template <typename Key, typename KeyHash = std::hash<Key>, typename KeyEq = std::equal_to<Key>, typename Alloc = GlobalAllocator<Key>>
    using UnorderedSet = std::unordered_set<Key, KeyHash, KeyEq, Alloc>;

    template <typename T, typename Alloc = GlobalAllocator<T>>
    using Deque = std::deque<T, Alloc>;

    template <typename T, typename Container = Deque<T>>
    using Queue = std::queue<T, Container>;

    template <typename CharType, typename Traits = std::char_traits<CharType>, typename Alloc = GlobalAllocator<CharType>>
    using BasicString = std::basic_string<CharType, Traits, Alloc>;

    template <typename CharType>
    using BasicStringView = std::basic_string_view<CharType>;

    using String = BasicString<char>;
    using StringView = BasicStringView<char>;

    template <typename CharType, typename Traits = std::char_traits<CharType>, typename Alloc = GlobalAllocator<CharType>>
    using BasicStringStream = std::basic_stringstream<CharType, Traits, Alloc>;

    using StringStream = BasicStringStream<char>;

    template <typename FunctionType>
    using Function = std::function<FunctionType>;

    using Json = nlohmann::basic_json<Map, Array, String, bool, std::int64_t, std::uint64_t, double, GlobalAllocator>;
}


// Inject some template specializations into the std namespace for our custom stl types
namespace std
{
    // This is only needed in non-msvc builds
#ifndef _MSC_VER
    template <class Elem, class Traits, class Alloc>
    struct hash<ScarletEngine::BasicString<Elem, Traits, Alloc>>
    {
        using argument_type = ScarletEngine::BasicString<Elem, Traits, Alloc>;
        using result_type = size_t;

        NODISCARD size_t operator()(const ScarletEngine::BasicString<Elem, Traits, Alloc>& Keyval) const
        {
            return std::hash<std::string_view>{}(Keyval.c_str());
        }
    };
#endif
}