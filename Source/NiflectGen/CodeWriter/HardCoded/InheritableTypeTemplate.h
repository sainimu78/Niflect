#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
#define LABEL_0 "TypeRegPrivate_Includes"
#define LABEL_1 "TypeRegPrivate_Namespace"
#define LABEL_2 "TypeRegPrivate_Implementation"
#define LABEL_4 "InheritableTypeReg_RegisterType"
#define LABEL_5 "InheritableTypeReg_InitInheritance"
#define LABEL_6 "InheritableTypeReg_CreateAccessorTree"
#define LABEL_7 "AccessorLevel"
#define LABEL_8 "InheritableTypeReg_BindingType"
#define LABEL_9 "InheritableTypeReg_InfoType"
#define LABEL_10 "InheritableTypeReg_InfoTypeOfBaseType"
#define LABEL_11 "InheritableTypeReg_BaseType"
#define LABEL_12 "InheritableTypeReg_AccessorOrRegisteredType"
#define LABEL_13 "InitFieldCode"
#define LABEL_14 "InitAccessor_Scope"
#define LABEL_15 "InheritableTypeReg_MiscAccessorType"
#define LABEL_16 "NativeTypeName"
#define LABEL_17 "MemberScope"
#define LABEL_18 "MemberVariableName"
#define LABEL_19 "ParentAccessorLevel"
#define LABEL_20 "SpecifyAccessorOffset"
#define LABEL_21 "MemberInternalName"
#define LABEL_23 "GetOrCastFromSharedAccessor"
#define LABEL_24 "FieldType"
#define LABEL_25 "MemberOwnerType"
#define LABEL_26 "TypeForHash"
#define LABEL_27 "StaticGetType_Misc_BindingType"
#define LABEL_28 "DeclStaticInitType"
#define LABEL_29 "DeclStaticInitMethod"
#define LABEL_30 "ImplStaticInitType"
#define LABEL_31 "ImplStaticInitMethod"
#define LABEL_32 "InheritableTypeReg_InitMethod"
#define LABEL_33 "TypeNameWithScope"

	namespace HardCodedTemplate
	{
		static const char* TypeReg_DeclStaticInitType =
"static void StaticInitType();"
;
		static const char* TypeReg_DeclStaticInitMethod =
"static void StaticInitMethod();"
;
		static const char* Deprecated_InheritableTypeReg_ClassDecl =
"class CInheritableTypeReg_" MAKELABEL(LABEL_SHARED_3) R"(
{
	using CTypeToReg = )" MAKELABEL(LABEL_SHARED_3) R"(;
	using CInfoType = )" MAKELABEL(LABEL_9) R"(;
public:
	static void StaticRegisterType(CNiflectTable& table);
	)" MAKELABEL(LABEL_28) R"(
	static CSharedField StaticCreateFieldLayout();
	)" MAKELABEL(LABEL_29) R"(
};)"
;
		static const char* Deprecated_TypeReg_ImplStaticInitType =
R"(void CInheritableTypeReg_)" MAKELABEL(LABEL_SHARED_3) R"(::StaticInitType()
{
	)" MAKELABEL(LABEL_5) R"(
})"
;
		static const char* TypeReg_ImplStaticInitMethod =
R"(void CInheritableTypeReg_)" MAKELABEL(LABEL_SHARED_3) R"(::StaticInitMethod()
{
	)" MAKELABEL(LABEL_32) R"(
})"
;
		static const char* Deprecated_InheritableTypeReg_ClassImpl =
R"(void CInheritableTypeReg_)" MAKELABEL(LABEL_SHARED_3) R"(::StaticRegisterType(CNiflectTable* table)
{
	)" MAKELABEL(LABEL_4) R"(
}
)" MAKELABEL(LABEL_30) R"(
CSharedField CInheritableTypeReg_)" MAKELABEL(LABEL_SHARED_3) R"(::StaticCreateFieldLayout()
{
	)" MAKELABEL(LABEL_6) R"(
}
)" MAKELABEL(LABEL_31)
;
		static const char* InheritableTypeReg_Class =
R"(class CInheritableTypeReg_)" MAKELABEL(LABEL_SHARED_3) R"(
{
	using CTypeToReg = )" MAKELABEL(LABEL_33) R"(;
	using CInfoType = )" MAKELABEL(LABEL_9) R"(;
public:
	static void StaticRegisterType(CNiflectTable* table)
	{
		)" MAKELABEL(LABEL_4) R"(
	}
	)" MAKELABEL(LABEL_30) R"(
	static CSharedAccessor StaticCreateFieldLayout()
	{
		)" MAKELABEL(LABEL_6) R"(
	}
};)"
;
		static const char* TypeReg_StaticInitType =
R"(static void StaticInitType()
{
	)" MAKELABEL(LABEL_5) R"(
})"
;
		static const char* InheritableTypeReg_RegisterType_Class2 =
R"(table->RegisterType<CInfoType, CTypeToReg>(")" MAKELABEL(LABEL_SHARED_3) R"(", &StaticCreateFieldLayout);)"
;
		static const char* InheritableTypeReg_InitInheritance2 =
R"(auto type = Cast<CInfoType>(StaticGetType<CTypeToReg>());
type->InitInheritableTypeMeta(Cast<)" MAKELABEL(LABEL_10) ">(StaticGetType<" MAKELABEL(LABEL_11) ">()));"
;
		static const char* CreateFieldLayout_CreateField =
"auto shared" MAKELABEL(LABEL_7) " = Niflect::MakeShared<" MAKELABEL(LABEL_24) ">();"
;
		static const char* CreateFieldLayout_InitType222222222 =
"node" MAKELABEL(LABEL_7) "->InitType(StaticGetType<CTypeToReg>());"
;
		static const char* CreateFieldLayout_GetNodeFromShared2 =
"auto node" MAKELABEL(LABEL_7) " = shared" MAKELABEL(LABEL_7) ".Get();"
;
		static const char* CreateFieldLayout_GetNodeFromSharedCasted2 =
"auto node" MAKELABEL(LABEL_7) " = Cast<" MAKELABEL(LABEL_24) ">(shared" MAKELABEL(LABEL_7) ".Get());"
;
		static const char* CreateFieldLayout_Return2 =
"return shared" MAKELABEL(LABEL_7) ";"
;
		static const char* InitField_Scope =
R"({
	)" MAKELABEL(LABEL_14) R"(
})"
;
		static const char* StaticGetType_Registered2 =
R"(auto type = StaticGetType<)" MAKELABEL(LABEL_12) ">();"
;
		static const char* StaticGetType_Misc =
R"(auto type = StaticGetType_Misc<)" MAKELABEL(LABEL_12) ", " MAKELABEL(LABEL_27) R"( >(")" MAKELABEL(LABEL_27) R"(");)"
;
		static const char* GetMemberVariableOffset2 =
"GetMemberVariableOffset(&" MAKELABEL(LABEL_25) "::" MAKELABEL(LABEL_21) ")"
;
		static const char* NoOffset2 =
"CAddrOffset::None"
;
		static const char* InitField_CreateForMember =
"auto shared" MAKELABEL(LABEL_7) R"( = type->CreateFieldLayout();
)" MAKELABEL(LABEL_23) R"(
node)" MAKELABEL(LABEL_7) R"(->InitMemberMeta(")" MAKELABEL(LABEL_21) R"(", )" MAKELABEL(LABEL_20) ");"
;
		static const char* InitField_AssignToOwner =
"node" MAKELABEL(LABEL_19) "->AddChild(shared" MAKELABEL(LABEL_7) ");"
;
		static const char* InitField_AssignElementToOwner =
"node" MAKELABEL(LABEL_19) "->InitElementAccessor(shared" MAKELABEL(LABEL_7) ");"
;
		static const char* TypeRegClassScopeName_InheritableType =
"CInheritableTypeReg_" MAKELABEL(LABEL_SHARED_3)
;
	}
}