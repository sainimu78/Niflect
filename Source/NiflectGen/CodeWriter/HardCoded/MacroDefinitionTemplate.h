#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
//#define LABEL_4 "FileID"
#define LABEL_5 "LineNum"
#define LABEL_7 "MacroName"
#define LABEL_8 "MacroBody"
	namespace HardCodedTemplate
	{
		static const char* LineNumberMacroDefinition =
R"(#define )" MAKELABEL(LABEL_7) R"(\
	)" MAKELABEL(LABEL_8);
	}
}