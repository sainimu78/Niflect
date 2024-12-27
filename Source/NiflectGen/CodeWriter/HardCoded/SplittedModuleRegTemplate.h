#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
#define LABEL_0 "Module"
#define LABEL_1 "Index"
#define LABEL_2 "Func"
#define LABEL_3 "Invokes"
#define LABEL_4 "Include"
#define LABEL_5 "Funcs"
#define LABEL_6 "Decls"

	namespace HardCodedTemplate
	{
		static const char* RegisterTypesFuncName =
"" MAKELABEL(LABEL_0) R"(_SplittedModuleReg_RegisterTypes_)" MAKELABEL(LABEL_1);
		static const char* InitTypesFuncName =
"" MAKELABEL(LABEL_0) R"(_SplittedModuleReg_InitTypes_)" MAKELABEL(LABEL_1);
		static const char* SplittedModuleRegFuncDecl =
R"(void )" MAKELABEL(LABEL_2) R"((Niflect::CNiflectTable* table);)";
		static const char* SplittedModuleRegFuncImpl =
R"(void )" MAKELABEL(LABEL_2) R"((Niflect::CNiflectTable* table)
{
	using namespace Niflect;
	)" MAKELABEL(LABEL_3) R"(
})";
		static const char* InitTypesFuncDecl =
R"(void )" MAKELABEL(LABEL_2) R"(();)";
		static const char* InitTypesFuncImpl =
R"(void )" MAKELABEL(LABEL_2) R"(()
{
	using namespace Niflect;
	)" MAKELABEL(LABEL_3) R"(
})";
		static const char* SplittedModuleRegH =
R"(#pragma once
)" MAKELABEL(LABEL_4) R"(

namespace Niflect
{
	class CNiflectTable;
}

)" MAKELABEL(LABEL_6) R"(
)";
		static const char* SplittedModuleRegCpp =
"" MAKELABEL(LABEL_4) R"(

)" MAKELABEL(LABEL_5) R"(
)";
		static const char* ImplScope =
R"({
	)" MAKELABEL(LABEL_3) R"(
})";
	}
}