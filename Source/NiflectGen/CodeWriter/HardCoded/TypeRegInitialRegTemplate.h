#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"

namespace NiflectGen
{
#define LABEL_0 "Type"
#define LABEL_1 "Info"
#define LABEL_2 "FuncName"
#define LABEL_3 "Body"
#define LABEL_4 "Accessor"
#define LABEL_5 "Nexts"
#define LABEL_6 "AccessorForm"
#define LABEL_7 "Offset"
#define LABEL_8 "FieldName"
#define LABEL_9 "TypeForLastTemplateArg"
#define LABEL_10 "API"
#define LABEL_13 "CreateTypeAccessorFuncName"
#define LABEL_14 "NataNullOrVar"
#define LABEL_15 "InvokeCtorAddr"

	namespace HardCodedTemplate
	{
		constexpr const char* InvokeRegisterTypeByFrameworkTableMethod =
"table->" MAKELABEL(LABEL_2) R"((")" MAKELABEL(LABEL_0) R"(", &)" MAKELABEL(LABEL_13) ", " MAKELABEL(LABEL_14) ", "  MAKELABEL(LABEL_15) ");";
		constexpr const char* InvokeRegisterTypeByGeneratedStaticFunc =
"" MAKELABEL(LABEL_2) R"((table, ")" MAKELABEL(LABEL_0) R"(", &)" MAKELABEL(LABEL_13) ", " MAKELABEL(LABEL_14) ");";
		constexpr const char* CreateTypeAccessorDecl =
R"(template <>
CSharedAccessor )" MAKELABEL(LABEL_2) "();";
		constexpr const char* CreateTypeAccessorImpl =
R"(template <>
CSharedAccessor )" MAKELABEL(LABEL_2) R"(()
{
	)" MAKELABEL(LABEL_3) R"(
})";
		constexpr const char* BuildTypeMetaDecl =
R"(template <>
void )" MAKELABEL(LABEL_2) R"((CNiflectType* type0);)";
		constexpr const char* BuildTypeMetaImpl =
R"(template <>
void )" MAKELABEL(LABEL_2) R"((CNiflectType* type0)
{
	)" MAKELABEL(LABEL_3) R"(
})";
		constexpr const char* CreateAndInitTypeAccessor =
"auto node0 = Niflect::MakeShared<" MAKELABEL(LABEL_4) R"(>();
node0->InitType2()" MAKELABEL(LABEL_2) R"(());
)" MAKELABEL(LABEL_5) R"(
return node0;
)";
		constexpr const char* CreateAndInitNextsAccessor =
"auto type1 = " MAKELABEL(LABEL_2) "();";
	constexpr const char* InitChildAccessor =
R"(type1->InitAddFieldToAccessor(node0.Get(), ")" MAKELABEL(LABEL_0) R"(", )" MAKELABEL(LABEL_7) ", " MAKELABEL(LABEL_14) ");";
	constexpr const char* GetFieldOffset =
R"(GetFieldOffset(&)" MAKELABEL(LABEL_0) "::" MAKELABEL(LABEL_8) R"())";
	constexpr const char* InitElementAccessor =
R"(type1->InitAccessorElementLayout(node0.Get());)";
	constexpr const char* StaticGetTypeFuncName =
R"(StaticGetType<)" MAKELABEL(LABEL_9) ">";
	constexpr const char* StaticGetTypeSpecDecl =
R"(template <>
)" MAKELABEL(LABEL_10) "CNiflectType* " MAKELABEL(LABEL_2) R"(();)";
	constexpr const char* StaticGetTypeSpecImpl =
R"(template <>
CNiflectType* )" MAKELABEL(LABEL_2) R"(()
{
	return TRegisteredType<)" MAKELABEL(LABEL_9) R"(>::s_type;
})";
	static const char* MacroBodyThisType =
"using CThis = " MAKELABEL(LABEL_9) ";";
	static const char* MacroBodyExposeToAccessor =
R"(friend Niflect::CSharedAccessor Niflect::)" NIFLECTFRAMEWORK_TEMPLATEFUNC_CTypeBody_CreateTypeAccessorL MAKELABEL(LABEL_9) ">();";
	static const char* MacroBodyExposeToCTypeBody =
"friend class Niflect::CTypeBody;";
	static const char* MacroBodyAnotherExample0 =
"using _DebugMyAnotherLineNumberMacroExpansionDecl = " MAKELABEL(LABEL_9) ";";
	}
}