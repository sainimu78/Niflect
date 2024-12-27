#pragma once
#include "Niflect/NiflectBase.h"
#include "clang-c/Index.h"

namespace NiflectGen
{
	class CMemSource
	{
	public:
		Niflect::CString m_filePath;
		Niflect::CString m_data;
	};
	using CSharedMemSource = Niflect::TSharedPtr<CMemSource>;

	class CMemSourceReferenceCache
	{
	public:
		void AddMemSourceRef(CMemSource& memSrc);
		CMemSource& CreateTempMemSource();

	public:
		Niflect::TArrayNif<CXUnsavedFile> m_vecHandle;
		Niflect::TArrayNif<CSharedMemSource> m_vecTempMemSrc;
	};
}