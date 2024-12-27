#pragma once
#include "Niflect/NiflectBase.h"
#include "clang-c/Index.h"

namespace NiflectGen
{
	struct SCursorHash {
		std::size_t operator()(const CXCursor& c) const noexcept {
			// ����Ҫ�����ϣ�ķ�ʽ����ֻ�Ǹ���ʾ��
			return clang_hashCursor(c);
		}
	};

	struct SCursorEqual {
		bool operator()(const CXCursor& a, const CXCursor& b) const noexcept {
			// ����Ҫ����Ƚϵķ�ʽ����ֻ�Ǹ���ʾ��
			return clang_equalCursors(a, b);
		}
	};
	template <typename TValue>
	using TCursorMap = Niflect::TUnorderedMap<CXCursor, TValue, SCursorHash, SCursorEqual>;

	using CCursorSet = Niflect::TUnorderedSet<CXCursor, SCursorHash, SCursorEqual>;

	struct SCXTypeLess {
		bool operator()(const CXType& a, const CXType& b) const noexcept {
			if (a.data[0] != b.data[0])
				return a.data[0] < b.data[0];
			return a.data[1] < b.data[1];
		}
	};
	template <typename TValue>
	using TCXTypeMap = Niflect::TMap<CXType, TValue, SCXTypeLess>;
}