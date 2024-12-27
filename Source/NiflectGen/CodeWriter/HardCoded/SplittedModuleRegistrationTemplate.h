#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
#define LABEL_0 "SplittedModuleRegID"
#define LABEL_1 "SplittedModuleName"
#define LABEL_2 "TypeRegCodeRegisterType"
#define LABEL_3 "TypeRegInvokationRegisterType"
#define LABEL_4 "TypeRegInvokationInitType"
#define LABEL_5 "IncludesForSplittedModuleRegCpp"
#define LABEL_6 "TypeRegNamespace"
#define LABEL_7 "TypeRegClassScope"

	namespace HardCodedTemplate
	{
		static const char* SplittedModuleName =
MAKELABEL(LABEL_SHARED_0) MAKELABEL(LABEL_0)
;
		static const char* SplittedModuleRegInvokationRegisterTypes =
"SplittedModuleRegRegsterTypes_" MAKELABEL(LABEL_1) "();"
;
		static const char* SplittedModuleRegInvokationInitTypes =
"SplittedModuleRegInitTypes_" MAKELABEL(LABEL_1) "();"
;
		static const char* SplittedModuleRegH =
R"(#pragma once

void SplittedModuleRegRegsterTypes_)" MAKELABEL(LABEL_1) R"(();
void SplittedModuleRegInitTypes_)" MAKELABEL(LABEL_1) R"(();
)"
;
		static const char* SplittedModuleRegCpp =
R"(#include "Niflect/NiflectRegistration.h")" R"(
)" MAKELABEL(LABEL_5) R"(

void SplittedModuleRegRegsterTypes_)" MAKELABEL(LABEL_1) R"(()
{
	)" MAKELABEL(LABEL_2) R"(
}
void SplittedModuleRegInitTypes_)" MAKELABEL(LABEL_1) R"(()
{
	)" MAKELABEL(LABEL_4) R"(
})"
;
		static const char* ModuleReg_TypeRegInitTable =
R"({
	auto table = this->AddNewTable();
	table->Init(CString());
})"
;
		static const char* TypeRegRegsterTypeScope =
R"({
	auto reg = Niflect::CNiflectRegistration::GetMutable();
	auto table = reg->GetMutableMiscTable();
	)" MAKELABEL(LABEL_3) R"(
})"
;
#ifdef INVOKE_TYPEREG_WITH_NAMESPACE
		static const char* InvokationRegisterType =
MAKELABEL(LABEL_6) "::" MAKELABEL(LABEL_7) "::StaticRegisterType(table);"
;
#else
		static const char* InvokationRegisterType2 =
MAKELABEL(LABEL_7) "::StaticRegisterType(table);"
;
#endif
#ifdef INVOKE_TYPEREG_WITH_NAMESPACE
		static const char* InvokationInitType =
MAKELABEL(LABEL_6) "::" MAKELABEL(LABEL_7) "::InitType();"
;
#else
		static const char* InvokationInitType2 =
MAKELABEL(LABEL_7) "::InitType();"
;
#endif
	}
}