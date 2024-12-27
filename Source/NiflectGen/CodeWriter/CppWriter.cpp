#include "NiflectGen/CodeWriter/CppWriter.h"
#include "Niflect/Util/StringUtil.h"

namespace NiflectGen
{
	void CSimpleCppWriter::AddInclude(const Niflect::CString& filePath)
	{
		ASSERT(!filePath.empty());
		this->AddLine(NiflectUtil::FormatString("#include \"%s\"", filePath.c_str()));
	}
	void CSimpleCppWriter::AddSystemInclude(const Niflect::CString& filePath)
	{
		ASSERT(!filePath.empty());
		this->AddLine(NiflectUtil::FormatString("#include <%s>", filePath.c_str()));
	}

	void CIncludesHelper::WriteIncludeDirectives(const Niflect::TArrayNif<SIncludeDerectiveData>& vecIncData, CCodeLines& lines)
	{
		for (auto& it : vecIncData)
		{
			char cL = '\"';
			char cR = '\"';
			if (it.m_isFromSystemHeaderSearchPaths)
			{
				cL = '<';
				cR = '>';
			}
			lines.push_back(NiflectUtil::FormatString("#include %c%s%c", cL, it.m_incPath.c_str(), cR));
		}
	}
	void CIncludesHelper::WriteUsingNamespaceDirectives(const CCodeLines& vecIncludePath, CCodeLines& lines)
	{
		for (auto& it : vecIncludePath)
			lines.push_back(NiflectUtil::FormatString("using namespace %s;", it.c_str()));
	}

	Niflect::CString ConvertToInternalFilePath(const Niflect::CString& relativeTypeRegFilePathNoExt)
	{
		auto fileNameNoExt = NiflectUtil::GetFileName(relativeTypeRegFilePathNoExt);
		auto internalFileNameNoExt = "_" + fileNameNoExt;
		Niflect::CString dirPath;
		NiflectUtil::GetParentDirPathSafe(relativeTypeRegFilePathNoExt, dirPath);
		auto internalRelativeTypeRegFilePathNoExt = internalFileNameNoExt;
		if (!dirPath.empty())
			internalRelativeTypeRegFilePathNoExt = NiflectUtil::ConcatPath(dirPath, internalFileNameNoExt);
		return internalRelativeTypeRegFilePathNoExt;
	}
}