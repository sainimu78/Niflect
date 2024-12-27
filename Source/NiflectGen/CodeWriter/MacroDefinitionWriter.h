#pragma once
#include "NiflectGen/CodeWriter/CppWriter.h"

namespace NiflectGen
{
	class CMacroDefinitionData
	{
	public:
		Niflect::CString m_namePostfix;
		CCodeLines m_linesBody;
	};

	class CTypeLineNumberMacroData
	{
	public:
		CTypeLineNumberMacroData()
			: m_generatedBodyLineNumber(INDEX_NONE)
		{
		}
		uint32 m_generatedBodyLineNumber;
		Niflect::TArrayNif<CMacroDefinitionData> m_vecMacroDefinitionData;
	};

	Niflect::CString GenerateLineNumberMacroName(const Niflect::CString& fileId, uint32 lineNumber, const Niflect::CString& namePostfix);
	void WriteLineNumberMacroDefinition(const Niflect::CString& macroName, const CCodeLines& linesBody, CCodeLines& linesDefinition);
}