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
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
#define LABEL_13 "BuildTypeMetaFuncName"
#else
#define LABEL_13 "CreateTypeAccessorFuncName"
#endif
#define LABEL_14 "NataNullOrVar"
#define LABEL_15 "InvokeCtorAddr"
#define LABEL_16 "MethodIdx"
#define LABEL_17 "TypeBodyFuncName"
#define LABEL_18 "Conversion"
#define LABEL_19 "Passing"
#define LABEL_20 "ArgRegs"
#define LABEL_21 "MethodName"
#define LABEL_22 "ResultAssignment"
#define LABEL_23 "TypeHash"
#define LABEL_24 "SignatureHash"
#define LABEL_25 "ArgsSignature"
#define LABEL_26 "FieldHash"

	namespace HardCodedTemplate
	{
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		constexpr const char* InvokeRegisterTypeByFrameworkTableMethod =
"table->" MAKELABEL(LABEL_2) R"((")" MAKELABEL(LABEL_0) R"(", &)" MAKELABEL(LABEL_13) ", " MAKELABEL(LABEL_14) ", " MAKELABEL(LABEL_15) ", " MAKELABEL(LABEL_23) ");";
		constexpr const char* InvokeRegisterTypeByGeneratedStaticFunc =
"" MAKELABEL(LABEL_2) R"((table, ")" MAKELABEL(LABEL_0) R"(", &)" MAKELABEL(LABEL_13) ", " MAKELABEL(LABEL_14) ", " MAKELABEL(LABEL_23) ");";
#else
		constexpr const char* InvokeRegisterTypeByFrameworkTableMethod =
"table->" MAKELABEL(LABEL_2) R"((")" MAKELABEL(LABEL_0) R"(", &)" MAKELABEL(LABEL_13) ", " MAKELABEL(LABEL_14) ", " MAKELABEL(LABEL_15) ");";
		constexpr const char* InvokeRegisterTypeByGeneratedStaticFunc =
"" MAKELABEL(LABEL_2) R"((table, ")" MAKELABEL(LABEL_0) R"(", &)" MAKELABEL(LABEL_13) ", " MAKELABEL(LABEL_14) ");";
#endif
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
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		constexpr const char* BuildTypeMetaBlock =
R"({
	auto node0 = Niflect::MakeShared<)" MAKELABEL(LABEL_4) R"(>();
	node0->InitType()" MAKELABEL(LABEL_2) R"(());
	type0->InitAccessor(node0);
	)" MAKELABEL(LABEL_5) R"(
})";
#else
		constexpr const char* CreateAndInitTypeAccessor =
"auto node0 = Niflect::MakeShared<" MAKELABEL(LABEL_4) R"(>();
node0->InitType2()" MAKELABEL(LABEL_2) R"(());
)" MAKELABEL(LABEL_5) R"(
return node0;
)";
#endif
		constexpr const char* CreateAndInitNextsAccessor =
"auto type1 = " MAKELABEL(LABEL_2) "();";
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		constexpr const char* InitChildAccessor =
R"(type0->InitAddField(")" MAKELABEL(LABEL_0) R"(", )" MAKELABEL(LABEL_7) ", type1, " MAKELABEL(LABEL_14) ", " MAKELABEL(LABEL_26) ");";
#else
	constexpr const char* InitChildAccessor =
R"(type1->InitAddFieldToAccessor(node0.Get(), ")" MAKELABEL(LABEL_0) R"(", )" MAKELABEL(LABEL_7) ", " MAKELABEL(LABEL_14) ");";
#endif
	constexpr const char* GetFieldOffset =
R"(GetFieldOffset(&)" MAKELABEL(LABEL_0) "::" MAKELABEL(LABEL_8) R"())";
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
	constexpr const char* InitElementAccessor =
"node0->InitElementType(type1);";
#else
	constexpr const char* InitElementAccessor =
R"(type1->InitAccessorElementLayout(node0.Get());)";
#endif
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
		constexpr const char* TypeBodyInvokeConstructorFuncName = 
"CTypeBody::InvokeConstructor<" MAKELABEL(LABEL_0) ", " MAKELABEL(LABEL_16) ">";
		constexpr const char* TypeBodyInvokeMethodFuncName =
"CTypeBody::InvokeMethod<" MAKELABEL(LABEL_0) ", " MAKELABEL(LABEL_16) ">";
		constexpr const char* TypeBodyInvokeStaticMemberFunctionFuncName =
"CTypeBody::InvokeStaticMemberFunction<" MAKELABEL(LABEL_0) ", " MAKELABEL(LABEL_16) ">";
		constexpr const char* TypeBodyInvokeConstructorImpl =
R"(template <>
void )" MAKELABEL(LABEL_17) R"((InstanceType* base, InstanceType** const args)
{
	)" MAKELABEL(LABEL_18) R"(
	GenericInstanceInvokeConstructor<)" MAKELABEL(LABEL_9) ">(base, " MAKELABEL(LABEL_19) R"();
})";
		constexpr const char* TypeBodyInvokeMethodImpl =
R"(template <>
void )" MAKELABEL(LABEL_17) R"((InstanceType* base, InstanceType** const args)
{
	)" MAKELABEL(LABEL_18) R"(
	)" MAKELABEL(LABEL_22) "static_cast<" MAKELABEL(LABEL_0) "*>(base)->" MAKELABEL(LABEL_21) "(" MAKELABEL(LABEL_19) R"();
})";
		constexpr const char* TypeBodyInvokeFunctionImpl =
R"(template <>
void )" MAKELABEL(LABEL_17) R"((InstanceType** const args)
{
	)" MAKELABEL(LABEL_18) R"(
	)" MAKELABEL(LABEL_22) "" MAKELABEL(LABEL_0) "::" MAKELABEL(LABEL_21) "(" MAKELABEL(LABEL_19) R"();
})";
		constexpr const char* MethodRegConstructorInfo =
R"(CConstructorInfo info(&)" MAKELABEL(LABEL_17) ", " MAKELABEL(LABEL_14) ", " MAKELABEL(LABEL_24) ", " MAKELABEL(LABEL_25) R"();
)" MAKELABEL(LABEL_20) R"(
type0->InitAddConstructorInfo(info);)";
		constexpr const char* MethodRegMethodInfo =
R"(CMethodInfo info(&)" MAKELABEL(LABEL_17) R"(, ")" MAKELABEL(LABEL_21) R"(", )" MAKELABEL(LABEL_14) ", " MAKELABEL(LABEL_24) ", " MAKELABEL(LABEL_25) R"();
)" MAKELABEL(LABEL_20) R"(
type0->InitAddMethodInfo(info);)";
		constexpr const char* MethodRegStaticMemberFunctionInfo =
R"(CFunctionInfo info(&)" MAKELABEL(LABEL_17) R"(, ")" MAKELABEL(LABEL_21) R"(", )" MAKELABEL(LABEL_14) ", " MAKELABEL(LABEL_24) ", " MAKELABEL(LABEL_25) R"();
)" MAKELABEL(LABEL_20) R"(
type0->InitAddStaticMemberFunctionInfo(info);)";
	}
}