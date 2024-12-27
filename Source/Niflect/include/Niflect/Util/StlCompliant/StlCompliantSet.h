#pragma once
#include <set>

namespace StlCompliantType2
{
	template <typename TElement, typename TCompare, typename TAllocator>
	using TSet = std::set<TElement, TCompare, TAllocator>;
}