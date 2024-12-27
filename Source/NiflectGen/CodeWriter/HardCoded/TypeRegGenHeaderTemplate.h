#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
#define LABEL_0 "TypeDecls"
#define LABEL_1 "StaticGetTypeSpecializations"
#define LABEL_2 "ExposedPrivateMacros"

	namespace HardCodedTemplate
	{
		static const char* GenHeader =
R"(
#pragma once
#include "Niflect/Base/NiflectMacro.h"
#include "Niflect/Type/RegisteredType.h"

)" MAKELABEL(LABEL_0) R"(

namespace Niflect
{
	)" MAKELABEL(LABEL_1) R"(
}

)" MAKELABEL(LABEL_2)
;
	}
}