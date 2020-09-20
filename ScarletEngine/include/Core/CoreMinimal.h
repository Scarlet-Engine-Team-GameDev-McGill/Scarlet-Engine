#pragma once

/** This is where we put core utility types which would be used both internally and externally */

#include "CorePCH.h"
#include "CoreUtils.h"
#include "Logger.h"
#include "Memory/Memory.h"

namespace ScarletEngine
{
	using byte_t = uint8_t;

	template <typename T, typename Alloc = GlobalAllocator<T>>
	using Array = std::vector<T, GlobalAllocator<T>>;

	template <typename T, size_t Size>
	using StaticArray = std::array<T, Size>;

	template <typename Key, typename Val, typename Pred = std::less<Key>, typename Alloc = GlobalAllocator<std::pair<const Key, Val>>>
	using Map = std::map<Key, Val, Pred, Alloc>;

	template <typename Key, typename Val, typename Hash = std::hash<Key>, typename KeyCmp = std::equal_to<Key>, typename Alloc = GlobalAllocator<std::pair<const Key, Val>>>
	using UnorderedMap = std::unordered_map<Key, Val, Hash, KeyCmp, Alloc>;

	template <typename CharType = char, typename Traits = std::char_traits<CharType>, typename Alloc = GlobalAllocator<CharType>>
	using BasicString = std::basic_string<CharType, Traits, Alloc>;

	using String = BasicString<char>;
}