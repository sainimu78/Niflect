#pragma once
#include "Niflect/Memory/AllocatorInterface.h"
#include "Niflect/Memory/Generic/GenericHeapAllocator.h"
#include "Niflect/Memory/Generic/GenericSharedPtr.h"
#include "Niflect/Util/StlCompliant/StlCompliantString2.h"
#include "Niflect/Util/StlCompliant/StlCompliantVector2.h"
//#include "Niflect/Util/StlCompliant/StlCompliantSortedVector.h"
#include "Niflect/Util/StlCompliant/StlCompliantMap2.h"
#include "Niflect/Util/StlCompliant/StlCompliantUnorderedMap2.h"
#include "Niflect/Util/StlCompliant/StlCompliantStringStream.h"
#include "Niflect/Util/StlCompliant/StlCompliantSet.h"
#include "Niflect/Util/StlCompliant/StlCompliantUnorderedSet.h"
//#include <string_view>

namespace Niflect
{
	class CMemory
	{
	public:
		NIFLECT_API static void* Alloc(size_t size);
		NIFLECT_API static void* Realloc(void* ptr, size_t size);
		NIFLECT_API static void Free(void* ptr);

	public:
		NIFLECT_API static void SetCurrentAllocator(IAllocatorInterface* p);
	};

	class CInstance
	{
	public:
		template <typename TClass, typename ...TArgs>
		inline static TClass* New(TArgs&& ...args)
		{
			return CGenericInstance::New<CMemory, TClass>(args...);
		}
		template <typename TClass>
		inline static void Delete(TClass* obj)
		{
			CGenericInstance::Delete<CMemory, TClass>(obj);
		}
	};

	template <typename T>
	using THeapAllocator = TGenericHeapAllocator<T, CMemory>;

	template <typename T>
	using TSharedPtr = TGenericSharedPtr<T, CMemory>;

	template <typename T>
	inline static TSharedPtr<T> MakeSharable(T* rawPtr)
	{
		return GenericMakeSharable<T, CMemory>(rawPtr);
	}
	template <typename T, typename ...TArgs>
	inline static TSharedPtr<T> MakeShared(TArgs&& ...args)
	{
		return GenericMakeShared<T, CMemory>(std::forward<TArgs>(args)...);
	}
	
	using CString = StlCompliantType2::CBasicString<THeapAllocator<char> >;

	template <typename TElement>
	using TArray = StlCompliantType2::TVector<TElement, THeapAllocator<TElement> >;
	template <typename TElement>
	using TArrayNif = StlCompliantType2::TVector<TElement, THeapAllocator<TElement> >;

	//备用
	//template <typename TKey, typename TCompare = std::less<TKey>, typename TAllocator = THeapAllocator<TKey> >
	//using TSortedArray = StlCompliantType2::TSortedVector<TKey, TCompare, TAllocator>;

	template <typename TKey, typename TValue, typename TCompare = std::less<TKey> >
	using TMap = StlCompliantType2::TMap<TKey, TValue, TCompare, THeapAllocator<std::pair<const TKey, TValue> > >;
	
	template <typename TKey, typename TValue, typename THash = std::hash<TKey>, typename TCompare = std::equal_to<TKey> >
	using TUnorderedMap = StlCompliantType2::TUnorderedMap<TKey, TValue, THash, TCompare, THeapAllocator<std::pair<const TKey, TValue> > >;

	using CStringStream = StlCompliantType2::CBasicStringStream<char, THeapAllocator<char> >;

	template <typename TElement, typename TCompare = std::less<TElement> >
	using TSet = StlCompliantType2::TSet<TElement, TCompare, THeapAllocator<TElement> >;

	template <typename TKey, typename THash = std::hash<TKey>, typename TCompare = std::equal_to<TKey> >
	using TUnorderedSet = StlCompliantType2::TUnorderedSet<TKey, THash, TCompare, THeapAllocator<TKey> >;
}

namespace std
{
	template <>
	struct hash<Niflect::CString>
	{
		size_t operator()(const Niflect::CString& s) const noexcept
		{
			//return std::hash<std::string_view>()(std::string_view(s.data(), s.size()));
			//todo: 无好办法复用std::has<std::string>, 只能接受复制字符串, 改用c++17后可用string_view避免复制
			return std::hash<std::string>{}(std::string(s.data(), s.size()));
		}
	};
}
