#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
#define LABEL_4 "Include"
#define LABEL_5 "Funcs"
#define LABEL_6 "Decls"

	namespace HardCodedTemplate
	{
		static const char* FieldLayoutSpecCpp =
"" MAKELABEL(LABEL_4) R"(

namespace Niflect
{
	)" MAKELABEL(LABEL_5) R"(
})";
		static const char* FieldLayoutSpecH =
"#pragma once" R"(
)" MAKELABEL(LABEL_4) R"(

namespace Niflect
{
	)" MAKELABEL(LABEL_6) R"(
})";
	}
}