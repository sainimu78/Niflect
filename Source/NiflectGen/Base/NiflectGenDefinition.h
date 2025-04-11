#pragma once
#include "Niflect/Util/StringUtil.h"
#include "Niflect/Util/ConcatHardCodedStrings.h"

#define ROOT_TEST_PROJECT_PATH "../../../../../.."
#define ROOT_TEST_INCLUDE_PATH CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_PROJECT_PATH, "/Source")
#define ROOT_TEST_PATH CONCAT_HARDCODED_STRINGS_2(ROOT_TEST_INCLUDE_PATH, "/NiflectGen/Test")

namespace NiflectGenDefinition
{
#define FILE_NAME_POSTFIX "_gen"

	namespace FileNamePostfix
	{
		constexpr const char* Gen = FILE_NAME_POSTFIX;
	}
	namespace FileExt
	{
		constexpr const char* Cpp = ".cpp";
		constexpr const char* H = ".h";
		constexpr const char* PrivateH = "_private.h";
		constexpr const char* GenH = CONCAT_HARDCODED_STRINGS_2(FILE_NAME_POSTFIX, ".h");//"_gen.h";

		static bool IsCpp(const Niflect::CString& text)
		{
			return NiflectUtil::EndsWith(text, Cpp);
		}
		static bool IsH(const Niflect::CString& text)
		{
			return NiflectUtil::EndsWith(text, H);
		}
	}

	namespace DirName
	{
		constexpr const char* GenSourceRoot = "_GenSource";
		constexpr const char* ModuleReg = "_ModuleReg";
		constexpr const char* TypeReg = "_TypeReg";
		constexpr const char* GenInclude = "include";
		constexpr const char* GenSrc = "";
		constexpr const char* GenTime = "_GenTime";
		constexpr const char* Splitted = "_Splitted";
	}

#define NIFLECT_GEN_TIME_MACRO "_NIFLECTGEN"

	namespace CodeTag
	{
#define MACROTAG_GENERATED_NON_STORAGE_DECLS "__NiflectGen_GeneratedNonStorageDecls"
#define MACROTAG_TYPE "__NiflectGen_Type"
#define MACROTAG_FIELD "__NiflectGen_Field"
#define MACROTAG_METHOD "__NiflectGen_Method"
#define MACROTAG_ENUMCONST "__NiflectGen_EnumConst"
#ifdef PORTING_ACCESS_METHOD
#define MACROTAG_ACCESSMETHOD "__ReflectionGen_AccessMethod"
#endif

#ifdef ACCESSOR_SETTING_ABCD
#else
		constexpr const char* BindingSetting = "__NiflectGen_BindingSetting";
#endif
		constexpr const char* GeneratedNonStorageDecls = MACROTAG_GENERATED_NON_STORAGE_DECLS;

		constexpr const char* Type = MACROTAG_TYPE;
		constexpr const char* Field = MACROTAG_FIELD;
		constexpr const char* Method = MACROTAG_METHOD;
		constexpr const char* EnumConstant = MACROTAG_ENUMCONST;
#ifdef PORTING_ACCESS_METHOD
		constexpr const char* AccessMethod = MACROTAG_ACCESSMETHOD;
#endif
		constexpr const char* GlobalVariable = Field;//暂同标识
		constexpr const char* GlobalFunction = Method;//暂同标识

#ifdef ACCESSOR_SETTING_ABCD
		//static bool IsAccessorSettingA(const Niflect::CString& text)
		//{
		//	return NiflectUtil::StartsWith(text, NiflectGenDefinition::CodeTag::AccessorSettingA);
		//}
		//static bool IsAccessorSettingB(const Niflect::CString& text)
		//{
		//	return NiflectUtil::StartsWith(text, NiflectGenDefinition::CodeTag::AccessorSettingB);
		//}
		//static bool IsAccessorSettingC(const Niflect::CString& text)
		//{
		//	return NiflectUtil::StartsWith(text, NiflectGenDefinition::CodeTag::AccessorSettingC);
		//}
		//static bool IsAccessorSettingD(const Niflect::CString& text)
		//{
		//	return NiflectUtil::StartsWith(text, NiflectGenDefinition::CodeTag::AccessorSettingD);
		//}
#else
		static bool IsBindingSetting(const Niflect::CString& text)
		{
			return NiflectUtil::StartsWith(text, NiflectGenDefinition::CodeTag::BindingSetting);
		}
#endif
		//static bool IsType(const Niflect::CString& text)
		//{
		//	return NiflectUtil::StartsWith(text, NiflectGenDefinition::CodeTag::Type);
		//}
		//static bool IsField(const Niflect::CString& text)
		//{
		//	return NiflectUtil::StartsWith(text, NiflectGenDefinition::CodeTag::Field);
		//}
		//static bool IsMethod(const Niflect::CString& text)
		//{
		//	return NiflectUtil::StartsWith(text, NiflectGenDefinition::CodeTag::Method);
		//}
	}

	namespace CodeStyle
	{
		constexpr const bool AddingSpaceBetweenNestedTemplateRightAngleBrackets = true;
		static void TryFormatNestedTemplate(Niflect::CString& str)
		{
			if (AddingSpaceBetweenNestedTemplateRightAngleBrackets)
			{
				if (str.back() == '>')
					str += ' ';
			}
		}
		static void TemplateAngleBracketL(Niflect::CString& str)
		{
			str += '<';
		}
		static void TemplateAngleBracketR(Niflect::CString& str)
		{
			TryFormatNestedTemplate(str);
			str += '>';
		}
	}

	namespace NiflectFramework
	{
		namespace ScopeName
		{
			constexpr const char* Niflect = "Niflect";
		}
		namespace AccessorTypeName
		{
			constexpr const char* Field = "CAccessor";
			constexpr const char* ArrayField = "CArrayField";
			constexpr const char* CompoundField = "CCompoundAccessor";
		}
		namespace InfoTypeName
		{
			constexpr const char* NiflectType = "CNiflectType";
			constexpr const char* Struct = "CStruct";
			constexpr const char* Class = "CClass";
			constexpr const char* Enum = "CEnum";
		}
		namespace FuncNamePrefix
		{
			constexpr const char* GeneratedGetModuleInfo = "NiflectGeneratedGetModuleInfo_";
		}
		namespace FileName
		{
			constexpr const char* ModuleRegisteredTypeHeader = "_RegisteredType.h";
			constexpr const char* BypassingHeader = "NiflectSTLDeclaration.h";
			constexpr const char* BypassSTLDefaultHeaderFileName = "NiflectSTLDeclaration.h";
			constexpr const char* FinishedFlag = "FinishedFlag.txt";
		}
		namespace FilePath
		{
#ifdef USER_PROVIDED_CCOMPONENTFIELD_CLASS_DEFINITION
			constexpr const char* CompoundFieldHeader = "Niflect/Field/CompoundField.h";
#endif
			constexpr const char* NiflectTypeHeader = "Niflect/NiflectType.h";
			constexpr const char* NiflectTableHeader = "Niflect/NiflectTable.h";
			constexpr const char* NiflectMacroTagHeader = "Niflect/NiflectMacroTag.h";
			constexpr const char* DefaultMacroTagHeader = "Niflect/Default/DefaultMacroTag.h";
			constexpr const char* NiflectModuleInfoHeader = "Niflect/NiflectModuleInfo.h";
			constexpr const char* NiflectModuleRegistryHeader = "Niflect/NiflectModuleRegistry.h";
			constexpr const char* NiflectMethodHeader = "Niflect/NiflectMethod.h";
		}
		namespace Setting
		{
			constexpr uint32 ThresholdLinesCountForRegCodeSplitting = 30;
		}
		namespace MacroNata
		{
			constexpr const char TokensBrackets[] = { '(', ')' };
			constexpr const uint32 IdentifierTokenFindingEndCount = 2;
		}
		namespace LineNumberMacro
		{
#define NIFLECTFRAMEWORK_MACROPOSTFIX_GENERATED_NON_STORAGE_DECLS "GENERATED_NON_STORAGE_DECLS"
			//constexpr const char* GENERATED_NON_STORAGE_DECLS = NIFLECTFRAMEWORK_MACROPOSTFIX_GENERATED_NON_STORAGE_DECLS;
			constexpr const char* FID = "FID_";
		}
		namespace FuncName
		{
			constexpr const char* CopyDerivedMeta = "MakeDerivedNata";
		}
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
#define NIFLECTFRAMEWORK_TEMPLATEFUNCADDR_InvokeDefaultConstructorL "&InvokeDefaultConstructor<"
#else
#define NIFLECTFRAMEWORK_TEMPLATEFUNCADDR_InvokeConstructorL "&GenericInstanceInvokeConstructor<"
#endif
#define NIFLECTFRAMEWORK_TEMPLATEFUNC_CTypeBody_CreateTypeAccessorL "CTypeBody::CreateTypeAccessor<"
#define NIFLECTFRAMEWORK_TEMPLATEFUNC_CTypeBody_BuildTypeMetaL "CTypeBody::BuildTypeMeta<"
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
		namespace BuiltinMetadata
		{
			constexpr const char* Getter = "Getter";
			constexpr const char* Setter = "Setter";
			constexpr const char* DefaultValue = "DefaultValue";
		}
#endif
	}
}