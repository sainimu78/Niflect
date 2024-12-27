#pragma once
#include "NiflectGen/CodeWriter/CodeTemplate.h"

namespace NiflectGen
{
	void ReadTemplateFromRawData(CCodeTemplate& tpl, const char* data);
	void ReadTemplateFromInputStream(CCodeTemplate& tpl, std::istream& is);
	Niflect::CString ReplaceLabelToText1(const Niflect::CString& templateText, const Niflect::CString& label, const Niflect::CString& text);
	Niflect::CString ReplaceLabelToText2(const Niflect::CString& templateText, const Niflect::CString& label0, const Niflect::CString& label1, const Niflect::CString& text0, const Niflect::CString& text1);
	void ReplaceLabelToLines1(const Niflect::CString& templateText, const Niflect::CString& label0, const Niflect::CString& text0, CCodeLines& linesResult);
	void ReplaceLabelToImplScopeLines(const CCodeLines& linesInScope, CCodeLines& vecLine);
	void WriteLinesIntoNamespaceScope(const Niflect::CString& namespaceName, const CCodeLines& linesInScope, CCodeLines& vecLine);
}