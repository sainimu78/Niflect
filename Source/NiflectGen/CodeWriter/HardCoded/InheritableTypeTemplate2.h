#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
#define LABEL_0 "Type"
#define LABEL_1 "InfoType"
#define LABEL_2 "GetType"
#define LABEL_3 "BaseType"
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
#define LABEL_4 "FieldType"
#define LABEL_5 "FieldName"
#define LABEL_6 "FuncName"
#endif

	namespace HardCodedTemplate
	{
		static const char* GetTypeCode = 
"StaticGetType<" MAKELABEL(LABEL_0) ">()";
		static const char* CastGetTypeCode = 
"" MAKELABEL(LABEL_1) "::Cast(StaticGetType<" MAKELABEL(LABEL_0) ">())";
		static const char* InitTypeCode =
"auto type = " MAKELABEL(LABEL_2) R"(;
type->InitInheritableTypeMeta(CInheritableType::Cast(StaticGetType<)" MAKELABEL(LABEL_3) ">()));";
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
		static const char* GetterFunc =
"" MAKELABEL(LABEL_4) " " MAKELABEL(LABEL_6) R"(() const
{
	return )" MAKELABEL(LABEL_5) R"(;
})";
		static const char* SetterFunc =
"void " MAKELABEL(LABEL_6) "(" MAKELABEL(LABEL_4) R"( in)
{
	)" MAKELABEL(LABEL_5) R"( = in;
})";
#endif
	}
}