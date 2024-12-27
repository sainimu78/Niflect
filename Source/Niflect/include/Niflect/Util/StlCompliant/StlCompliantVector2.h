#pragma once
#include <vector>

namespace StlCompliantType2
{
	template <typename TElement, typename TAllocator>
	using TVector = std::vector<TElement, TAllocator>;
}