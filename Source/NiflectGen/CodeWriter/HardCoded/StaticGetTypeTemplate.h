#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
#define LABEL_0 "Include"
#define LABEL_1 "TypeDecl"
#define LABEL_2 "StaticGetTypeDecl"
#define LABEL_3 "Impl"
#define LABEL_4 "FileID"
#define LABEL_6 "Macros"
#define LABEL_7 "LineNum"
#define LABEL_8 "Header"
#define LABEL_9 "MacroDefinitionItems"
#define LABEL_10 "MacroDefinitionRoot"
#define LABEL_11 "GenHMacro"
#define LABEL_12 "GenHFileName"
#define LABEL_13 "ModuleHFileName"

	namespace HardCodedTemplate
	{
		static const char* LineNumberMacros =
"//This is the expansion of " NIFLECTFRAMEWORK_MACROPOSTFIX_GENERATED_NON_STORAGE_DECLS " at the line " MAKELABEL(LABEL_7) " of " MAKELABEL(LABEL_8) R"(
)" MAKELABEL(LABEL_9) R"(
)" MAKELABEL(LABEL_10);
		static const char* GenH =
"" MAKELABEL(LABEL_0) R"(

#ifdef )" MAKELABEL(LABEL_11) R"(
#error ")" MAKELABEL(LABEL_12) R"( already included, missing '#pragma once' in )" MAKELABEL(LABEL_13) R"("
#endif
#define )" MAKELABEL(LABEL_11) R"(

)" MAKELABEL(LABEL_1) R"(

)" MAKELABEL(LABEL_2) R"(

)" MAKELABEL(LABEL_6) R"(

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID )" MAKELABEL(LABEL_4);
		static const char* GenCpp =
"" MAKELABEL(LABEL_0) R"(

)" MAKELABEL(LABEL_3);
	}
}