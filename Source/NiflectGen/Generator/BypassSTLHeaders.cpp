#include "NiflectGen/Generator/BypassSTLHeaders.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/CodeWriter/CodeTemplate.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "Niflect/Util/SystemUtil.h"

namespace NiflectGen
{
	using WriteHeaderFileFunc = std::function<void(Niflect::CString& code)>;

	static void Write_cstddef(Niflect::CString& code)
	{
		code =
R"(#pragma once

#ifdef WIN32
	#ifdef _WIN64
	#else
	#endif
#else
	#ifdef __x86_64__
	#else 
	#endif
#endif

// With clang style
typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__  ptrdiff_t;

namespace std
{
	using :: ptrdiff_t;
	using :: size_t;
	using max_align_t = double; // most aligned type
	using nullptr_t   = decltype(nullptr);
})";
	}
	static void Write_map(Niflect::CString& code)
	{
		code =
R"(#pragma once
#include <xmemory>
#include <type_traits>

namespace std
{
template<class _Kty,
	class _Ty,
	class _Pr = less<_Kty>,
	class _Alloc = allocator<pair<const _Kty, _Ty> > >
	class map
	{
	};
})";
	}
	static void Write_memory(Niflect::CString& code)
	{
		code =
R"(#pragma once

namespace std
{
	template <class _Ty>
	class shared_ptr
	{
	};
})";
	}
	static void Write_set(Niflect::CString& code)
	{
		code =
R"(#pragma once
#include <xmemory>
#include <type_traits>

namespace std
{
template<class _Kty,
	class _Pr = less<_Kty>,
	class _Alloc = allocator<_Kty> >
	class set
	{
	};
})";
	}
	static void Write_sstream(Niflect::CString& code)
	{
		code =
R"(#pragma once
#include <xmemory>
#include <string>

namespace std
{
	template <class _Elem, class _Traits = char_traits<_Elem>, class _Alloc = allocator<_Elem>>
	class basic_stringstream
	{

	};
})";
	}
	static void Write_stddef_h(Niflect::CString& code)
	{
		code =
R"(#define NULL 0
)";
	}
	static void Write_string(Niflect::CString& code)
	{
		code =
R"(#pragma once
#include <xmemory>

namespace std
{
template<class _Elem,
	class _Traits,
	class _Alloc>
	class basic_string
	{
	public:
		basic_string()
		{
		}
		template <typename _TAny>
		basic_string(_TAny&& bypassForAny)
		{
		}
	};

template<class _Elem>
	struct char_traits
	{
	};

template<> struct char_traits<char>
	{	// properties of a string or stream char element
	};

typedef basic_string<char, char_traits<char>, allocator<char> >
	string;
})";
	}
	static void Write_type_traits(Niflect::CString& code)
	{
		code =
R"(#pragma once

namespace std
{
template<class _Ty = void>
	struct less
	{
	};
	
template<class _Kty>
	struct hash
	{
	};

template <class _From, class _To>
	struct is_convertible
	{
		// determine whether _From is convertible to _To
	};	
})";
	}
	static void Write_unordered_map(Niflect::CString& code)
	{
		code =
R"(#pragma once
#include <xmemory>
#include <type_traits>
#include <utility>

namespace std
{
template<class _Kty,
	class _Ty,
	class _Hasher = hash<_Kty>,
	class _Keyeq = equal_to<_Kty>,
	class _Alloc = allocator<pair<const _Kty, _Ty> > >
	class unordered_map
	{
	};
})";
	}
	static void Write_unordered_set(Niflect::CString& code)
	{
		code =
R"(#pragma once
#include <xmemory>
#include <type_traits>
#include <utility>

namespace std
{
template <class _Kty, class _Hasher = hash<_Kty>, class _Keyeq = equal_to<_Kty>,
    class _Alloc = allocator<_Kty>>
	class unordered_set
	{
	};
})";
	}
	static void Write_vector(Niflect::CString& code)
	{
		code =
R"(#pragma once
#include <xmemory>

namespace std
{
template<class _Ty,
	class _Alloc = allocator<_Ty> >
	class vector
	{
	};
})";
	}
	static void Write_utility(Niflect::CString& code)
	{
		code =
R"(#pragma once

namespace std
{
template<class _Ty1,
	class _Ty2>
	struct pair
	{
	_Ty1 first;		// the first stored value
	_Ty2 second;	// the second stored value
	};
	
template<class _Ty = void>
	struct equal_to
	{
	};
})";
	}
	static void Write_xmemory(Niflect::CString& code)
	{
		code =
R"(#pragma once

namespace std
{
template<class _Ty>
	class allocator
	{
	};
})";
	}
	static void Write_xtr1common(Niflect::CString& code)
	{
		code =
R"(#pragma once

namespace std
{
template <bool _Test, class _Ty = void>
struct enable_if {}; // no member "type" when !_Test

template <class _Ty>
struct enable_if<true, _Ty> { // type is _Ty for _Test
    using type = _Ty;
};

template <bool _Test, class _Ty = void>
using enable_if_t = typename enable_if<_Test, _Ty>::type;
})";
	}
	struct SHeaderData
	{
		const char* m_headerFileName;
		const bool m_withBypassDefinition;
	};
	void GenerateBypassSTLHeaders(const Niflect::CString& headersDirPath)
	{
		//��ĸ�����н�Ϊ���㲹�� BypassSTL, ���䲽��
		//1. ʵ��д��Ӧ��ϵͳͷ�ļ��� Write_xxx, �� Write_map
		//2. ��Ӧ�ر�� m_withBypassDefinition Ϊ true, �� {"map", true},//Write_map
		//3. �������д�����ӵ� vecWriteFunc ��, �� vecWriteFunc.push_back(&Write_map);
		//����ע�ⰴ��ĸ���д����
		SHeaderData arrAllNeeded[] = {
			{"algorithm", false},
			{"array", false},
			{"cassert", false},
			{"cstddef", true},//Write_cstddef
			{"cstdio", false},
			{"cstring", false},
			{"deque", false},
			{"exception", false},
			{"fstream", false},
			{"functional", false},
			{"hash", false},
			{"hash_map", false},
			{"hash_set", false},
			{"ios", false},
			{"iosfwd", false},
			{"iostream", false},
			{"istream", false},
			{"iterator", false},
			{"list", false},
			{"locale", false},
			{"map", true},//Write_map
			{"memory", true},//Write_memory
			{"new", false},
			{"numeric", false},
			{"ostream", false},
			{"queue", false},
			{"regex", false},
			{"set", true},//Write_set
			{"sstream", true},//Write_sstream
			{"stack", false},
			{"streambuf", false},
			{"stddef.h", true},//Write_stddef_h
			{"string", true},//Write_string
			{"system_error", false},
			{"tuple", false},
			{"typeindex", false},
			{"typeinfo", false},
			{"type_traits", true},//Write_type_traits
			{"unordered_map", true},//Write_unordered_map
			{"unordered_set", true},//Write_unordered_set
			{"utility", true},//Write_utility
			{"vector", true},//Write_vector
			{"xfunctional", false},
			{"xhash", false},
			{"xiosbase", false},
			{"xlocale", false},
			{"xlocbuf", false},
			{"xmemory", true},//Write_xmemory
			{"xstring", false},
			{"xtr1common", true},//Write_xtr1common
			{"xtree", false},
			{"xutility", false},
			{"xxatomic", false},
		};

		Niflect::TArrayNif<WriteHeaderFileFunc> vecWriteFunc;
		vecWriteFunc.push_back(&Write_cstddef);
		vecWriteFunc.push_back(&Write_map);
		vecWriteFunc.push_back(&Write_memory);
		vecWriteFunc.push_back(&Write_set);
		vecWriteFunc.push_back(&Write_sstream);
		vecWriteFunc.push_back(&Write_stddef_h);
		vecWriteFunc.push_back(&Write_string);
		vecWriteFunc.push_back(&Write_type_traits);
		vecWriteFunc.push_back(&Write_unordered_map);
		vecWriteFunc.push_back(&Write_unordered_set);
		vecWriteFunc.push_back(&Write_utility);
		vecWriteFunc.push_back(&Write_vector);
		vecWriteFunc.push_back(&Write_xmemory);
		vecWriteFunc.push_back(&Write_xtr1common);

		Niflect::CString defaultBypassHeaderCode;

		Niflect::TArrayNif<CMemSource> vecMemSrc;
		CMemSource memSrcDefaultHeader;
		{
			Niflect::CString fileName = NiflectGenDefinition::NiflectFramework::FileName::BypassSTLDefaultHeaderFileName;
			memSrcDefaultHeader.m_filePath = NiflectUtil::ConcatPath(headersDirPath, fileName);

			CSimpleCppWriter writer(defaultBypassHeaderCode);
			writer.AddHeaderFirstLine();
			writer.AddInclude(fileName);
		}

		uint32 cnt = sizeof(arrAllNeeded) / sizeof(SHeaderData);
		vecMemSrc.resize(cnt);
		uint32 idxFunc = 0;
		Niflect::TArrayNif<uint32> vecIdxWrittenByFunc;
		for (uint32 idx = 0; idx < cnt; ++idx)
		{
			auto& item = arrAllNeeded[idx];
			auto& memSrc = vecMemSrc[idx];
			memSrc.m_filePath = NiflectUtil::ConcatPath(headersDirPath, item.m_headerFileName);
			if (!item.m_withBypassDefinition)
			{
				memSrc.m_data = defaultBypassHeaderCode;
			}
			else
			{
				vecIdxWrittenByFunc.push_back(idx);
				vecWriteFunc[idxFunc++](memSrc.m_data);
			}
		}
		{
			CSimpleCppWriter writer(memSrcDefaultHeader.m_data);
			writer.AddHeaderFirstLine();
			for (auto& it : vecIdxWrittenByFunc)
				writer.AddSystemInclude(arrAllNeeded[it].m_headerFileName);
			vecMemSrc.push_back(memSrcDefaultHeader);
		}

		ASSERT(vecWriteFunc.size() == idxFunc);

		{
			//�ѳ��Բ������ļ�, ʹ�� CXUnsavedFile ���� clang_parseTranslationUnit, ���������޷��ڽ���ʱ�ҵ��ڴ��е��ļ�, ����������ɵ�����, ���ٿɱ���Ҫ���û�ָ�� Bypass ��·��
			for (auto& it : vecMemSrc)
			{
				NiflectUtil::MakeDirectories(it.m_filePath);
				NiflectUtil::WriteStringToFile(it.m_data, it.m_filePath);
			}
		}
	}
}