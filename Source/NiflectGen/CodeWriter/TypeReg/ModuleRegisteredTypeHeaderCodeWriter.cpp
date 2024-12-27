#include "NiflectGen/CodeWriter/TypeReg/ModuleRegisteredTypeHeaderCodeWriter.h"
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/CodeWriter/CppWriter.h"

namespace NiflectGen
{
	namespace HardCodedTemplate
	{
#define LABEL_0 "Include"
#define LABEL_1 "Prefix"

		constexpr const char* ModuleRegisteredTypeHeader =
R"(#pragma once
)" MAKELABEL(LABEL_0) R"(

namespace Niflect
{
	template <typename T>
	class )" MAKELABEL(LABEL_1) R"(TRegisteredType
	{
	public:
		static bool IsValid()
		{
			return s_type != NULL;
		}

		static CNiflectType* s_type;
	};

	template <typename T>
	CNiflectType* )" MAKELABEL(LABEL_1) R"(TRegisteredType<T>::s_type = NULL;

	template <typename T>
	CNiflectType* )" MAKELABEL(LABEL_1) R"(StaticGetType()
	{
		return )" MAKELABEL(LABEL_1) R"(TRegisteredType<T>::s_type;
	}

	template <typename T>
	CSharedAccessor )" MAKELABEL(LABEL_1) R"(CreateTypeAccessor();

	template <typename TType, typename TInfo = CNiflectType>
	void )" MAKELABEL(LABEL_1) R"(RegisterType(CNiflectTable* table, const Niflect::CString& id, const CreateTypeAccessorFunc& inCreateTypeAccessorFunc, const CSharedNata& nata)
	{
		STypeLifecycleMeta lifecycleMeta;
		lifecycleMeta.m_typeSize = sizeof(TType);
		lifecycleMeta.m_InvokeConstructorFunc = &GenericInstanceInvokeConstructor<TType>;
		lifecycleMeta.m_InvokeDestructorFunc = &GenericInstanceInvokeDestructor<TType>;

		auto shared = Niflect::MakeShared<TInfo>();
		CNiflectType* type = shared.Get();
		auto idx = table->GetTypesCount();
		ASSERT(!)" MAKELABEL(LABEL_1) R"(TRegisteredType<TType>::IsValid());
		type->InitTypeMeta2(lifecycleMeta, inCreateTypeAccessorFunc, CNiflectType::GetTypeHash<TType>(), idx, id, &)" MAKELABEL(LABEL_1) R"(TRegisteredType<TType>::s_type, nata);
		ASSERT()" MAKELABEL(LABEL_1) R"(TRegisteredType<TType>::IsValid());
		table->InsertType(shared, idx);
	}
})";
	}

	void WriteModuleRegisteredTypeHeaderCodeWriter(const SModuleRegisteredTypeHeaderWritingContext& context, SModuleRegisteredTypeHeaderGenData& data)
	{
		CCodeTemplate tpl1;
		ReadTemplateFromRawData(tpl1, HardCodedTemplate::ModuleRegisteredTypeHeader);
		CLabelToCodeMapping map;

		CCodeLines linesInclude;
		{
			Niflect::TArrayNif<CHeaderFilePathData> vecHeaderData;
			vecHeaderData.push_back(NiflectGenDefinition::NiflectFramework::FilePath::NiflectTableHeader);
			CIncludesHelper::ConvertFromHeaderFilePaths(vecHeaderData, context.m_moduleRegInfo.m_writingHeaderSearchPaths, linesInclude);
			//linesCppInclude = linesFilePath;
		}
		MapLabelToLines(map, LABEL_0, linesInclude);
		MapLabelToText(map, LABEL_1, context.m_moduleRegInfo.m_moduleScopeSymbolPrefix);
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl1.ReplaceLabels(map, data.m_linesHeader, &setReplacedLabel);
	}
}