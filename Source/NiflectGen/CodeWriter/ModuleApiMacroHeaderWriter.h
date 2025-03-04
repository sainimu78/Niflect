#pragma once
#include "Niflect/Base/String.h"
#include "Niflect/Base/Array.h"
#include "NiflectGen/CodeWriter/CodeWriter.h"

namespace NiflectGen
{
	void GetModuleApiMacroHeaderInfo(const Niflect::CString& moduleGenSourceRootDirPath, const Niflect::CString& moduleName, Niflect::CString& moduleApiMacro, Niflect::CString& moduleApiMacroHeaderFilePath);
	Niflect::CString GetApiMacroHeaderFileName(const Niflect::CString& moduleName);
	void WriteModuleApiMacroHeader(const Niflect::CString& moduleName, const Niflect::CString& moduleApiMacro, CCodeLines& linesHeader);
}