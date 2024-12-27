#include "NiflectGen/Generator/SourceInMemory.h"
#include "clang-c/Index.h"

namespace NiflectGen
{
	void CMemSourceReferenceCache::AddMemSourceRef(CMemSource& memSrc)
	{
		m_vecHandle.push_back(CXUnsavedFile{});
		auto& handle = m_vecHandle.back();
		handle.Filename = memSrc.m_filePath.data();
		handle.Contents = memSrc.m_data.data();
		handle.Length = static_cast<uint32>(memSrc.m_data.length());
	}
	CMemSource& CMemSourceReferenceCache::CreateTempMemSource()
	{
		m_vecTempMemSrc.push_back(Niflect::MakeShared<CMemSource>());
		return *m_vecTempMemSrc.back();
	}
}