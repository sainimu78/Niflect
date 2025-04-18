#include "NiflectGen/CodeWriter/TypeReg/InheritableTypeRegCodeWriter2.h"
#include "NiflectGen/Util/CursorUtil.h"
#include "NiflectGen/CodeWriter/HardCoded/InheritableTypeTemplate2.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/CodeWriter/TypeReg/TypeAccessorCodeWriter.h"

namespace NiflectGen
{
	CInheritableTypeRegCodeWriter2::CInheritableTypeRegCodeWriter2(const Niflect::TArrayNif<CResolvedCursorNode>& vecFieldResocursorNode, const CTaggedType* baseTaggedType, uint32 generatedBodyLineNumber
		, const Niflect::TArrayNif<CTaggedInheritableTypeField*>& vecField
#ifdef PORTING_ACCESS_METHOD
		, const Niflect::TArrayNif<CTaggedInheritableTypeAccessMethod*>& vecAccessMethod
#endif
		, const Niflect::TArray<CResolvedMethod>& vecResomethod
		, const Niflect::TArray<CTaggedInheritableTypeMethod*>& vecMethod
	)
		: m_vecFieldResocursorNode(vecFieldResocursorNode)
		, m_vecField(vecField)
#ifdef PORTING_ACCESS_METHOD
		, m_vecAccessMethod(vecAccessMethod)
#endif
		, m_vecResomethod(vecResomethod)
		, m_vecTaggedMethod(vecMethod)
		, m_baseTaggedType(baseTaggedType)
		, m_generatedBodyLineNumber(generatedBodyLineNumber)
	{

	}
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
	static bool IsTypePrefix(char charPrefix, Niflect::CString& result)
	{
		if (result.size() >= 1 && result[0] == charPrefix)
		{
			bool isTypePrefix = result.size() >= 2;
			if (isTypePrefix)
				isTypePrefix = std::isupper(result[1]);
			return isTypePrefix;
		}
		return false;
	}
	static void ConvertFieldToFuncName(const Niflect::CString& fieldName, const Niflect::CString& funcNamePrefix, Niflect::CString& funcName)
	{
		//funcName = fieldName;
		//const Niflect::CString memberPrefix = "m_";
		//auto pos = fieldName.find(memberPrefix);
		//if (pos != std::string::npos)
		//	funcName = fieldName.substr(pos + memberPrefix.length(), fieldName.length() - memberPrefix.length());
		//auto& firstLetter = funcName[0];
		//if (std::islower(firstLetter))
		//	firstLetter = std::toupper(firstLetter);
		//funcName = funcNamePrefix + funcName;

		// Start by assuming the fieldName is the input to be converted
		Niflect::CString result = fieldName;

		// Remove 'm_' or 'm' prefix
		if (result.size() >= 2 && result[0] == 'm' && result[1] == '_') {
			result = result.substr(2);
		}
		else if (result.size() >= 1 && result[0] == 'm') {
			result = result.substr(1);
		}

		if (IsTypePrefix('b', result))
			result = result.substr(1);

		// Capitalize the first letter
		if (!result.empty() && std::islower(result[0])) {
			result[0] = std::toupper(result[0]);
		}

		// Prepend the function name prefix
		funcName = funcNamePrefix + result;
	}
	static void WriteGetterSetter(const char* hct, const Niflect::CString& funcNamePrefix, const Niflect::CString& fieldTypeName, const Niflect::CString& fieldName, CCodeLines& lines, Niflect::CString& funcName)
	{
		CCodeTemplate tpl1;
		ReadTemplateFromRawData(tpl1, hct);
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_4, fieldTypeName);
		MapLabelToText(map, LABEL_5, fieldName);
		ConvertFieldToFuncName(fieldName, funcNamePrefix, funcName);
		MapLabelToText(map, LABEL_6, funcName);
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl1.ReplaceLabels(map, lines, &setReplacedLabel);
	}
#endif
	static bool TryParseInClassInitializer(const CXCursor& cursor, Niflect::CString& data)
	{
		if (false)
		{
			//基于 token 的方法无法获取到如常量构成的 Initializer

			CXSourceRange range = clang_getCursorExtent(cursor);
			CXTranslationUnit translationUnit = clang_Cursor_getTranslationUnit(cursor);

			bool hasInClassInitializer = false;
			{
				//获取无换行的所有字符
				CXToken* tokens = nullptr;
				unsigned int numTokens = 0;
				clang_tokenize(translationUnit, range, &tokens, &numTokens);
				uint32 identifierKindsIdx = 0;
				uint32 asignOpIdx = -1;
				for (unsigned idx = 0; idx < numTokens; ++idx)
				{
					auto& token = tokens[idx];
					auto str = CXStringToCString(clang_getTokenSpelling(translationUnit, token));
					if (hasInClassInitializer)
					{
						if (str != " ")
							data += str;
					}
					if (!hasInClassInitializer)
					{
						if (str == "=")
						{
							hasInClassInitializer = true;
						}
					}
				}
				clang_disposeTokens(translationUnit, tokens, numTokens);
			}
			return hasInClassInitializer;
		}
		if (true)
		{
			auto fieldName = CXStringToCString(clang_getCursorSpelling(cursor));

			CXSourceRange range = clang_getCursorExtent(cursor);
			CXTranslationUnit translationUnit = clang_Cursor_getTranslationUnit(cursor);

			bool hasInClassInitializer = false;
			{
				CXFile begin_file, end_file;
				unsigned begin_line, begin_column, end_line, end_column, begin_offset, end_offset;
				clang_getSpellingLocation(clang_getRangeStart(range),
					&begin_file, &begin_line, &begin_column, &begin_offset);
				clang_getSpellingLocation(clang_getRangeEnd(range),
					&end_file, &end_line, &end_column, &end_offset);
				if (begin_file && end_file)
				{
					ASSERT(begin_file == end_file);
					ASSERT(end_offset > begin_offset);

					size_t size = 0;
					auto contents = clang_getFileContents(translationUnit, begin_file, &size);

					Niflect::CString line;
					for (uint32 idx = begin_offset; idx < size; ++idx)
					{
						auto& ch = contents[idx];
						if (ch == ';')
							break;
						line += ch;
					}

					uint32 movedPos = 0;
					hasInClassInitializer = ExtractBuiltinMetadata(fieldName, line, data, movedPos);
				}
			}

			return hasInClassInitializer;
		}
	}
#ifdef DEPRECATED_ACCESSMETHOD_MACRO_TAG
#else
	static bool ErrorIfSpecifiedButNotTagged(const Niflect::CString& specifiedMethodName, const char* pszMethodType, const Niflect::TArrayNif<CTaggedInheritableTypeAccessMethod*>& vecAccessMethod, CGenLog* log)
	{
		bool found = false;
		for (auto& it : vecAccessMethod)
		{
			auto methodName = CXStringToCString(clang_getCursorSpelling(it->GetCursor()));
			if (specifiedMethodName == methodName)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			GenLogError(log, NiflectUtil::FormatString("Builtin metadata %s of %s is not tagged", specifiedMethodName.c_str(), pszMethodType));
		}
		return found;
	}
#endif
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
	//void CInheritableTypeRegCodeWriter2::WriteInvokeMethodFunc(const STypeReginvokeMethodFuncWritingInput& input, STypeReginvokeMethodFuncWritingOutput& output) const
	//{

	//	this->CollectDependencyHeaderFilePathAddrs(output.m_dependencyHeaderFilePathAddrs);
	//}
	void CInheritableTypeRegCodeWriter2::WriteResocursorNodeBodyCode(const SResocursorNodeBodyCodeWritingContext& context, SGetterSetterWritingData& data) const
	{
		ASSERT(m_vecTaggedMethod.size() == m_vecResomethod.size());
		uint32 constructorsCount = 0;
		uint32 methodsCount = 0;
		uint32 staticMemberFunctionsCount = 0;
		for (uint32 idx0 = 0; idx0 < m_vecResomethod.size(); ++idx0)
		{
			auto& resomethod = m_vecResomethod[idx0];
			auto& taggedMethod = m_vecTaggedMethod[idx0];
			auto& methodCursor = taggedMethod->GetCursor();

			CCodeLines linesNata;
			taggedMethod->WriteCopyNataCode(linesNata);

			Niflect::CString typeBodyFuncName;
			auto kind = clang_getCursorKind(methodCursor);
			bool isGlobalsType = m_bindingTypeIndexedRoot->IsGlobalsType();
			bool isGlobal = isGlobalsType;
			if (!isGlobal)
			{
				if (kind == CXCursor_Constructor)
				{
					if (!clang_CXXConstructor_isDefaultConstructor(methodCursor))
					{
						WriteInvokeConstructorBody(m_bindingTypeIndexedRoot->m_resocursorName, m_bindingTypeIndexedRoot->GetResocursorNameForLastTemplateArg(), constructorsCount, resomethod.m_vecArgument, typeBodyFuncName, data.m_linesInvokeMethodFuncImpl);
						WriteMethodRegConstructorInfo(typeBodyFuncName, linesNata, context.m_moduleRegInfo.m_moduleScopeSymbolPrefix, resomethod.m_vecArgument, resomethod.m_resultType, data.m_linesResoBodyCode);
						constructorsCount++;
					}
				}
				else if (kind == CXCursor_CXXMethod)
				{
					auto methodName = CXStringToCString(clang_getCursorSpelling(methodCursor));
					if (clang_Cursor_getStorageClass(methodCursor) != CX_StorageClass::CX_SC_Static)
					{
						WriteInvokeMethodBody(m_bindingTypeIndexedRoot->m_resocursorName, methodsCount, resomethod.m_vecArgument, resomethod.m_resultType, methodName, typeBodyFuncName, data.m_linesInvokeMethodFuncImpl);
						WriteMethodRegMethodInfo(typeBodyFuncName, linesNata, context.m_moduleRegInfo.m_moduleScopeSymbolPrefix, resomethod.m_vecArgument, resomethod.m_resultType, methodName, data.m_linesResoBodyCode);
						methodsCount++;
					}
					else
					{
						isGlobal = true;
					}
				}
			}
			else if (kind == CXCursor_FunctionDecl)
			{
				isGlobal = true;
			}
			if (isGlobal)
			{
				auto methodName = CXStringToCString(clang_getCursorSpelling(methodCursor));
				Niflect::CString scopeName = GenerateNamespacesAndScopesCode(methodCursor);
				if (!isGlobalsType)
					scopeName = m_bindingTypeIndexedRoot->m_resocursorName + "::";
				methodName = scopeName + methodName;
				WriteInvokeStaticMemberFunctionBody(m_bindingTypeIndexedRoot->m_resocursorName, staticMemberFunctionsCount, resomethod.m_vecArgument, resomethod.m_resultType, methodName, typeBodyFuncName, data.m_linesInvokeMethodFuncImpl);
				WriteMethodRegStaticMemberFunctionInfo(typeBodyFuncName, linesNata, context.m_moduleRegInfo.m_moduleScopeSymbolPrefix, resomethod.m_vecArgument, resomethod.m_resultType, methodName, data.m_linesResoBodyCode);
				staticMemberFunctionsCount++;
			}
		}

#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
		data.m_vecGetSetData.resize(m_vecFieldResocursorNode.size());
#endif
		for (uint32 idx = 0; idx < m_vecFieldResocursorNode.size(); ++idx)
		{
			auto& itB = m_vecField[idx];
			auto& fieldCursor = itB->GetCursor();
			auto fieldName = CXStringToCString(clang_getCursorSpelling(fieldCursor));
			auto fieldStaticGetTypeFuncName = m_vecFieldResocursorNode[idx].GetStaticGetTypeFuncName(context.m_moduleRegInfo.m_moduleScopeSymbolPrefix);

#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
			auto& fieldCursor = itB->GetCursor();
			auto& fieldResonode = m_vecFieldResocursorNode[idx];
			auto fieldTypeName = fieldResonode.GetResocursorNameForLastTemplateArg();
			CCodeLines linesNata;
			CBuiltinMetadata builtinMetadata;
			itB->WriteCopyNataCodeExtractBuiltinMetadata(linesNata, builtinMetadata, context.m_log);
			auto& gs = data.m_vecGetSetData[idx];
			if (builtinMetadata.m_getterName.empty())
				WriteGetterSetter(HardCodedTemplate::GetterFunc, "Get", fieldTypeName, fieldName, gs.m_linesGetter, builtinMetadata.m_getterName);
#ifdef DEPRECATED_ACCESSMETHOD_MACRO_TAG
#else
			else
				ErrorIfSpecifiedButNotTagged(builtinMetadata.m_getterName, NiflectGenDefinition::NiflectFramework::BuiltinMetadata::Getter, m_vecAccessMethod, context.m_log);
#endif
			if (builtinMetadata.m_setterName.empty())
				WriteGetterSetter(HardCodedTemplate::SetterFunc, "Set", fieldTypeName, fieldName, gs.m_linesSetter, builtinMetadata.m_setterName);
#ifdef DEPRECATED_ACCESSMETHOD_MACRO_TAG
#else
			else
				ErrorIfSpecifiedButNotTagged(builtinMetadata.m_setterName, NiflectGenDefinition::NiflectFramework::BuiltinMetadata::Setter, m_vecAccessMethod, context.m_log);
#endif
			if (builtinMetadata.m_defaultValue.empty())
				TryParseInClassInitializer(fieldCursor, builtinMetadata.m_defaultValue);

			{
				//现未使用 Getter, Setter, DefaultValue 关键字, Field 的 In-class 初始化, 仅打印解析结果预留功能
				Niflect::CString strNata;
				for (auto& it1 : linesNata)
					strNata += it1 += EscapeChar::NewLine;
				printf("%s, %s, %s, %s\n", builtinMetadata.m_getterName.c_str(), builtinMetadata.m_setterName.c_str(), builtinMetadata.m_defaultValue.c_str(), strNata.c_str());
			}
#else
			CCodeLines linesNata;
			itB->WriteCopyNataCode(linesNata);
#endif

			Niflect::CString scopeName = GenerateNamespacesAndScopesCode(fieldCursor);
			if (!m_bindingTypeIndexedRoot->IsGlobalsType())
				scopeName = m_bindingTypeIndexedRoot->m_resocursorName + "::";
			else
				fieldName = scopeName + fieldName;
			WriteNextInitChildAccessor2(m_bindingTypeIndexedRoot->m_resocursorName, m_bindingTypeIndexedRoot->m_taggedTypeIndex == INDEX_NONE, scopeName, fieldStaticGetTypeFuncName, fieldName, linesNata, data.m_linesResoBodyCode);
		}
	}
#else
	void CInheritableTypeRegCodeWriter2::WriteResocursorNodeBodyCode(const SResocursorNodeBodyCodeWritingContext& context, SGetterSetterWritingData& data) const
	{
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
		data.m_vecGetSetData.resize(m_vecFieldResocursorNode.size());
#endif
		for (uint32 idx = 0; idx < m_vecFieldResocursorNode.size(); ++idx)
		{
			auto& itB = m_vecField[idx];
			auto fieldName = CXStringToCString(clang_getCursorSpelling(itB->GetCursor()));
			auto fieldStaticGetTypeFuncName = m_vecFieldResocursorNode[idx].GetStaticGetTypeFuncName(context.m_moduleRegInfo.m_moduleScopeSymbolPrefix);

#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
			auto& fieldCursor = itB->GetCursor();
			auto& fieldResonode = m_vecFieldResocursorNode[idx];
			auto fieldTypeName = fieldResonode.GetResocursorNameForLastTemplateArg();
			CCodeLines linesNata;
			CBuiltinMetadata builtinMetadata;
			itB->WriteCopyNataCodeExtractBuiltinMetadata(linesNata, builtinMetadata, context.m_log);
			auto& gs = data.m_vecGetSetData[idx];
			if (builtinMetadata.m_getterName.empty())
				WriteGetterSetter(HardCodedTemplate::GetterFunc, "Get", fieldTypeName, fieldName, gs.m_linesGetter, builtinMetadata.m_getterName);
#ifdef DEPRECATED_ACCESSMETHOD_MACRO_TAG
#else
			else
				ErrorIfSpecifiedButNotTagged(builtinMetadata.m_getterName, NiflectGenDefinition::NiflectFramework::BuiltinMetadata::Getter, m_vecAccessMethod, context.m_log);
#endif
			if (builtinMetadata.m_setterName.empty())
				WriteGetterSetter(HardCodedTemplate::SetterFunc, "Set", fieldTypeName, fieldName, gs.m_linesSetter, builtinMetadata.m_setterName);
#ifdef DEPRECATED_ACCESSMETHOD_MACRO_TAG
#else
			else
				ErrorIfSpecifiedButNotTagged(builtinMetadata.m_setterName, NiflectGenDefinition::NiflectFramework::BuiltinMetadata::Setter, m_vecAccessMethod, context.m_log);
#endif
			if (builtinMetadata.m_defaultValue.empty())
				TryParseInClassInitializer(fieldCursor, builtinMetadata.m_defaultValue);

			{
				//现未使用 Getter, Setter, DefaultValue 关键字, Field 的 In-class 初始化, 仅打印解析结果预留功能
				Niflect::CString strNata;
				for (auto& it1 : linesNata)
					strNata += it1 += EscapeChar::NewLine;
				printf("%s, %s, %s, %s\n", builtinMetadata.m_getterName.c_str(), builtinMetadata.m_setterName.c_str(), builtinMetadata.m_defaultValue.c_str(), strNata.c_str());
			}
#else
			CCodeLines linesNata;
			itB->WriteCopyNataCode(linesNata);
#endif

			WriteNextInitChildAccessor2(m_bindingTypeIndexedRoot->m_resocursorName, fieldStaticGetTypeFuncName, fieldName, linesNata, data.m_linesResoBodyCode);
		}
		for (uint32 idx = 0; idx < m_vecResomethod.size(); ++idx)
		{

		}
	}
#endif
	void CInheritableTypeRegCodeWriter2::CollectDependencyHeaderFilePathAddrs(CDependencyHeaderFilePathAddrs& dependencyHeaderFilePathAddrs) const
	{
		//还差基类的include
		dependencyHeaderFilePathAddrs.m_vecDecl.push_back(m_bindingTypeIndexedRoot->GetHeaderFilePathAddrForTaggedType());
		for (auto& it0 : m_vecFieldResocursorNode)
			it0.GetHeaderFilePathAddrs(dependencyHeaderFilePathAddrs.m_vecImpl);
		for (auto& it0 : m_vecResomethod)
		{
			for (auto& it1 : it0.m_vecArgument)
				it1.GetHeaderFilePathAddrs(dependencyHeaderFilePathAddrs.m_vecImpl);
			it0.m_resultType.GetHeaderFilePathAddrs(dependencyHeaderFilePathAddrs.m_vecImpl);
		}
	}
	void CInheritableTypeRegCodeWriter2::CollectDataForGenH(SCollectingGeneratedBodyWritingData& data) const
	{
		data.m_generatedBodyLineNumber = m_generatedBodyLineNumber;
	}
	void CInheritableTypeRegCodeWriter2::WriteStaticInitType(const STypeRegClassWritingContext& context, CCodeLines& lines) const
	{
		ASSERT(false);
//		if (m_baseTaggedType != NULL)
//		{
//			static const char* aaaaaaaaaaaaaaaaa =
//R"(static void StaticInitType()
//{
//	auto type = (Cast)StaticGetType<${Nihao}>();
//	type->InitInheritableTypeMeta(StaticGetType<${Bucuo}>);
//})";
//			CCodeTemplate tpl1;
//			tpl1.ReadFromRawData(aaaaaaaaaaaaaaaaa);
//			CLabelToCodeMapping map;
//			MapLabelToText(map, "Nihao", m_bindingTypeIndexedRoot->m_resocursorName);
//			MapLabelToText(map, "Bucuo", m_baseTaggedType->m_taggedResoRoot.m_resocursorName);
//			Niflect::TSet<Niflect::CString> setReplacedLabel;
//			tpl1.ReplaceLabels(map, lines, &setReplacedLabel);
//		}
	}
	void CInheritableTypeRegCodeWriter2::WriteInvokeInitType(const STypeRegClassWritingContext& context, CTypeRegTaggedTypeInitWritingData2& data) const
	{
		if (m_baseTaggedType != NULL)
		{
			CCodeTemplate tpl1;
			ReadTemplateFromRawData(tpl1, HardCodedTemplate::InitTypeCode);
			CLabelToCodeMapping map;
			auto& resocursorName = m_bindingTypeIndexedRoot->m_resocursorName;
			MapLabelToText(map, LABEL_0, resocursorName);
			Niflect::CString codeGetType;
			{
				Niflect::CString infoTypeName;
				if (m_resolvedData->m_taggedMapping.GetDerivedInfoTypeName(m_bindingTypeIndexedRoot->m_taggedTypeIndex, infoTypeName))
				{
					codeGetType = ReplaceLabelToText2(HardCodedTemplate::CastGetTypeCode, LABEL_0, LABEL_1, resocursorName, infoTypeName);
				}
				else
				{
					ASSERT(false);//CNiflectType 不支持 InitInheritableTypeMeta, 因此是一种不应出现的错误, 仅留作备用, 未来可移除
					codeGetType = ReplaceLabelToText1(HardCodedTemplate::GetTypeCode, LABEL_0, resocursorName);
				}
			}
			MapLabelToText(map, LABEL_2, codeGetType);
			MapLabelToText(map, LABEL_3, m_baseTaggedType->m_resocursorName);
			{
				//有必要不支持并提示 struct 从 class 或反之的继承?
				Niflect::CString baseInfoTypeName;
				if (GetInfoTypeNameFromCursor(m_baseTaggedType->GetCursor(), baseInfoTypeName))
					MapLabelToText(map, LABEL_7, baseInfoTypeName);
				else
					ASSERT(false);
			}
			Niflect::TSet<Niflect::CString> setReplacedLabel;
			tpl1.ReplaceLabels(map, data.m_lines, &setReplacedLabel);
		}
	}
}
