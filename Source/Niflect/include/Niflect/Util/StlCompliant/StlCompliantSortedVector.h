#pragma once
#include "Niflect/NiflectBase.h"
#include "Niflect/Util/sorted_vector.h"

namespace StlCompliantType2
{
	template <typename TKey, typename TCompare, typename TAllocator>
	using TSortedVector = codeproject::sorted_vector<TKey, true, TCompare, TAllocator>;
}