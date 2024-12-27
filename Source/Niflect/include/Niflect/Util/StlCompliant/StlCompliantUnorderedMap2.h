#pragma once
#include <unordered_map>
#include <functional>

namespace StlCompliantType2
{
	template <typename TKey, typename TValue, typename THash, typename TCompare, typename TAllocator>
	using TUnorderedMap = std::unordered_map<TKey, TValue, THash, TCompare, TAllocator>;
}