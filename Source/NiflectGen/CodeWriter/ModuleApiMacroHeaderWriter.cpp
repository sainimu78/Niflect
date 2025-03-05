#include "NiflectGen/CodeWriter/ModuleApiMacroHeaderWriter.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"
#include <algorithm>
#include "Niflect/Util/StringUtil.h"
#include "Niflect/Util/SystemUtil.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"

namespace NiflectGen
{
#define LABEL_0 "EnabledMacro"
#define LABEL_1 "ApiMacro"

	namespace HardCodedTemplate
	{
		static const char* Header = 
R"(#pragma once

#ifdef )" MAKELABEL(LABEL_1) R"(
#else
	#ifdef WIN32
		#ifdef )" MAKELABEL(LABEL_0) R"(
			#define )" MAKELABEL(LABEL_1) R"( __declspec(dllexport)
		#else
			#define )" MAKELABEL(LABEL_1) R"( __declspec(dllimport)
		#endif
	#else
		#ifdef )" MAKELABEL(LABEL_0) R"(
			#define )" MAKELABEL(LABEL_1) R"( __attribute__((visibility("default")))
		#else
			#define )" MAKELABEL(LABEL_1) R"( 
		#endif
	#endif
#endif)";
	}
	static Niflect::CString to_upper(Niflect::CString s) {
		std::transform(s.begin(), s.end(), s.begin(),
			[](unsigned char c) { return std::toupper(c); });
		return s;
	}
	static void GetApiMacroNames(const Niflect::CString& moduleName, Niflect::CString& enabledMacro, Niflect::CString& moduleApiMacro)
	{
		auto apiMacro = to_upper("_" + moduleName);
		enabledMacro = apiMacro + "_EXPORTS";
		moduleApiMacro = apiMacro + "_API";
	}
	Niflect::CString GetApiMacroHeaderFileName(const Niflect::CString& moduleName)
	{
		return "_" + moduleName + "ApiHeader.h";
	}
	void GetModuleApiMacroHeaderInfo(const Niflect::CString& moduleName, Niflect::CString& moduleApiMacro, Niflect::CString& moduleApiMacroHeaderFilePath)
	{
		Niflect::CString placeholder;
		GetApiMacroNames(moduleName, placeholder, moduleApiMacro);
		moduleApiMacroHeaderFilePath = GetApiMacroHeaderFileName(moduleName);
	}
	void WriteModuleApiMacroHeader(const Niflect::CString& moduleName, const Niflect::CString& moduleApiMacro, CCodeLines& linesHeader)
	{
		Niflect::CString placeholder;
		Niflect::CString enabledMacro;
		GetApiMacroNames(moduleName, enabledMacro, placeholder);

		CCodeTemplate tpl1;
		ReadTemplateFromRawData(tpl1, HardCodedTemplate::Header);
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_0, enabledMacro);
		MapLabelToText(map, LABEL_1, moduleApiMacro);
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl1.ReplaceLabels(map, linesHeader, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
	}
}