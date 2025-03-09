#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
#define LABEL_0 "Include"
#define LABEL_1 "RegisterTypes"
#define LABEL_2 "InitTypes"
//#define LABEL_3 "StaticModuleReg"
#define LABEL_4 "ModuleName"
#define LABEL_5 "ModuleAPI"
#define LABEL_6 "FuncNamePrefix"

	namespace HardCodedTemplate
	{
#define GENERATED_FUNC_NAME_GeneratedRegisterTypes "GeneratedRegisterTypes"

		static const char* ModuleRegImpl =
"" MAKELABEL(LABEL_0) R"(

namespace Niflect
{
	static void )" GENERATED_FUNC_NAME_GeneratedRegisterTypes R"((Niflect::CNiflectTable* table)
	{
		)" MAKELABEL(LABEL_1) R"(
	}
	static void GeneratedInitTypes()
	{
		)" MAKELABEL(LABEL_2) R"(
	}
})";
		static const char* StaticModuleRegHeader =
R"(#pragma once
)" MAKELABEL(LABEL_0) R"(

extern "C" )" MAKELABEL(LABEL_5) R"( Niflect::CNiflectModuleInfo* )" MAKELABEL(LABEL_6) "" MAKELABEL(LABEL_4) "();";
		static const char* StaticModuleRegHeader2 =
R"(#pragma once
)" MAKELABEL(LABEL_0) R"(

extern "C" )" MAKELABEL(LABEL_5) R"( Niflect::CNiflectModuleInfo* )" MAKELABEL(LABEL_6) "" MAKELABEL(LABEL_4) "();";
		static const char* StaticModuleRegImpl =
R"(namespace Niflect
{
	class CRegger_)" MAKELABEL(LABEL_4) R"(
	{
	public:
		CRegger_)" MAKELABEL(LABEL_4) R"((const Niflect::CString& name, const ModuleRegisterTypesFunc& RegisterTypesFunc, const ModuleInitTypesFunc& InitTypesFunc)
		{
			s_info = Niflect::MakeShared<CNiflectModuleInfo>();
			s_info->InitMeta(name, RegisterTypesFunc, InitTypesFunc);
		}
		static CSharedModuleInfo s_info;
	};
	CSharedModuleInfo CRegger_)" MAKELABEL(LABEL_4) R"(::s_info;
	static CRegger_)" MAKELABEL(LABEL_4) R"( g_staticRegger_)" MAKELABEL(LABEL_4) R"((")" MAKELABEL(LABEL_4) R"(", &)" GENERATED_FUNC_NAME_GeneratedRegisterTypes R"(, & GeneratedInitTypes);
})";
#define FUNCNAMEHARDCODEDSTRING_GetModuleInfoPrivate "NiflectGeneratedGetModuleInfoPrivate"
		static const char* StaticModuleRegImpl2 =
R"(namespace Niflect
{
	static CNiflectModuleRegger g_staticRegger_)" MAKELABEL(LABEL_4) R"((")" MAKELABEL(LABEL_4) R"(", &)" GENERATED_FUNC_NAME_GeneratedRegisterTypes R"(, &GeneratedInitTypes);
})";
		static const char* StaticModuleRegGetModuleC =
"Niflect::CNiflectModuleInfo* " MAKELABEL(LABEL_6) "" MAKELABEL(LABEL_4) R"(()
{
	return Niflect::CRegger_)" MAKELABEL(LABEL_4) R"(::s_info.Get();
})";
		static const char* StaticModuleRegGetModuleC2 =
"Niflect::CNiflectModuleInfo* " MAKELABEL(LABEL_6) "" MAKELABEL(LABEL_4) R"(()
{
	return )" FUNCNAMEHARDCODEDSTRING_GetModuleInfoPrivate R"(();
})";
	}
}