#pragma once
#include <unordered_set>
#include <functional>

namespace StlCompliantType2
{
	template <typename TKey, typename THash, typename TCompare, typename TAllocator>
	using TUnorderedSet = std::unordered_set<TKey, THash, TCompare, TAllocator>;
}