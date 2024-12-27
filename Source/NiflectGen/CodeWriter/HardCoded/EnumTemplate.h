#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
	namespace HardCodedTemplate
	{
#define LABEL_0 "Type"
#define LABEL_1 "AddConsts"
#define LABEL_2 "Const"
#define LABEL_3 "NataNullOrVar"

		constexpr const char* InitTypeCode =
R"(CEnumMeta em;
)" MAKELABEL(LABEL_1) R"(
auto et = CEnum::Cast(StaticGetType<)" MAKELABEL(LABEL_0) R"(>());
et->InitEnumMeta(em);
)";
		constexpr const char* AddEnumConst =
R"(em.InitAddConst(")" MAKELABEL(LABEL_2) R"(", )" MAKELABEL(LABEL_3) ");";
	}
}