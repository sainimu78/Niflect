#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
#define LABEL_0 "TypeDeclWithoutNamespace"
#define LABEL_1 "TypeNameWithNamespace"
#define LABEL_2 "FileID"
#define LABEL_3 "LineNumber"
#define LABEL_4 "TypeDeclWithoutNamespaceAndScope"

	namespace HardCodedTemplate
	{
		static const char* TypeRegGenHeader_TypeDecl =
MAKELABEL(LABEL_0) ";"
;
		static const char* TypeRegGenHeader_StaticGetTypeSpecialization =
R"(template <>
CNiflectType* StaticGetType<)" MAKELABEL(LABEL_1) R"(>()
{
	return TInternalRegisteredType<)" MAKELABEL(LABEL_1) R"(>::s_type;
})"
;
		static const char* TypeRegGenHeader_ExposedPrivateMacro =
"#define " MAKELABEL(LABEL_2) "_" MAKELABEL(LABEL_3) R"(_EXPOSED_PRIVATE"
	friend )" MAKELABEL(LABEL_0) ";"
;
	}
}