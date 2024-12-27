#pragma once
#include <map>
#include <functional>

namespace StlCompliantType2
{
	template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
	using TMap = std::map<TKey, TValue, TCompare, TAllocator>;
}