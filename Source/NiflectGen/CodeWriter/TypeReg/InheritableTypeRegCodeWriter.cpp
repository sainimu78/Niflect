#include "NiflectGen/CodeWriter/TypeReg/InheritableTypeRegCodeWriter.h"
#include "NiflectGen/Util/CursorUtil.h"
#include "NiflectGen/CodeWriter/HardCoded/InheritableTypeTemplate.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"

namespace NiflectGen
{
	CInheritableTypeRegCodeWriter::CInheritableTypeRegCodeWriter(const CXCursor& cursor, const STypeRegClassWritingSetting& writingSetting)
		: inherited(cursor, writingSetting)
	{
	}
	void CInheritableTypeRegCodeWriter::WriteDecl(const CWritingContext& context, CTypeRegDeclWrittingData& data) const
	{
		CCodeTemplate tpl;
		ReadTemplateFromRawData(tpl, HardCodedTemplate::Deprecated_InheritableTypeReg_ClassDecl);
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_SHARED_3, m_typeName.c_str());
		MapLabelToText(map, LABEL_9, this->GetInfoTypeName().c_str());
		CCodeLines linesInitType;
		this->WriteDeclInitType(linesInitType);
		if (linesInitType.size() > 0)
			MapLabelToLines(map, LABEL_28, linesInitType);
		CCodeLines linesInitMethod;
		this->WriteDeclInitMethod(linesInitMethod);
		if (linesInitMethod.size() > 0)
			MapLabelToLines(map, LABEL_29, linesInitMethod);
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl.ReplaceLabels(map, data.m_lines, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
	}
	void CInheritableTypeRegCodeWriter::WriteImpl(const CWritingContext& context, CTypeRegImplWrittingData& data) const
	{
		CCodeTemplate tpl;
		ReadTemplateFromRawData(tpl, HardCodedTemplate::Deprecated_InheritableTypeReg_ClassImpl);
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_SHARED_3, m_typeName.c_str());
		CCodeLines linesRegisterType;
		CNoDupPathCollector placeholder;
		CTypeRegClassWrittingData dataRegisterType(linesRegisterType, placeholder);
		this->WriteRegisterType(context, dataRegisterType);
		MapLabelToLines(map, LABEL_4, linesRegisterType);
		CCodeLines linesInitType;
		{
			CCodeLines lines;
			this->WriteImplInitType(lines);
			if (lines.size() > 0)
			{
				CCodeWriter writer;
				writer.WriteLine(HardCodedTemplate::Deprecated_TypeReg_ImplStaticInitType);
				CCodeTemplate tpl;
				ReadTemplateFromRawData(tpl, writer.m_code.c_str());
				CLabelToCodeMapping map;
				MapLabelToText(map, LABEL_SHARED_3, m_typeName);
				MapLabelToLines(map, LABEL_5, lines);
				tpl.ReplaceLabels(map, linesInitType);
			}
		}
		MapLabelToLines(map, LABEL_30, linesInitType);
		CCodeLines linesCreateAccessorTree;
		CTypeRegClassWrittingData dataCreateFieldLayout(linesCreateAccessorTree, placeholder);
		this->WriteCreateAccessorTree(context, dataCreateFieldLayout);
		MapLabelToLines(map, LABEL_6, linesCreateAccessorTree);

		//todo: method可能需要合并到InitType中
		CCodeLines linesInitMethod;
		{
			CCodeLines lines;
			this->WriteImplInitMethod(lines);
			if (lines.size() > 0)
			{
				CCodeWriter writerInitType;
				writerInitType.WriteLine(HardCodedTemplate::TypeReg_ImplStaticInitMethod);
				CCodeTemplate tpl;
				ReadTemplateFromRawData(tpl, writerInitType.m_code.c_str());
				CLabelToCodeMapping map;
				MapLabelToText(map, LABEL_SHARED_3, m_typeName);
				MapLabelToLines(map, LABEL_32, lines);
				tpl.ReplaceLabels(map, linesInitMethod);
			}
		}
		MapLabelToLines(map, LABEL_30, linesInitType);

		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl.ReplaceLabels(map, data.m_lines, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() <= map.size());
	}

	static Niflect::CString FindNamespaceAndTypeScope(const CXCursor& cursor, const Niflect::TArrayNif<Niflect::CString>& vecNamespaceEntered)
	{
		Niflect::TArrayNif<Niflect::CString> vecFieldNamespace;
		Niflect::TArrayNif<Niflect::CString> vecTypeScope;
		FindNamespaceAndScopeNameRecursOld(cursor, vecFieldNamespace, vecTypeScope);
		RemoveUnnessesaryNamespacees(vecNamespaceEntered, vecFieldNamespace);
		Niflect::CString strScope;
		for (uint32 idx2 = 0; idx2 < vecFieldNamespace.size(); ++idx2)
			strScope += vecFieldNamespace[idx2] + "::";
		for (uint32 idx2 = 0; idx2 < vecTypeScope.size(); ++idx2)
			strScope += vecTypeScope[idx2] + "::";
		return strScope;
	}
	static Niflect::CString GetFieldTypeNameWithScope(const CXCursor& cursor, const Niflect::TArrayNif<Niflect::CString>& vecNamespaceEntered)
	{
		if (true)
		{
			auto fieldTypeName = CXStringToCString(clang_getCursorSpelling(cursor));
			auto strScope = FindNamespaceAndTypeScope(cursor, vecNamespaceEntered);
			return strScope + fieldTypeName;
		}
		else
		{
			//全namespace
			auto fieldTypeNameScoped = CXStringToCString(clang_getTypeSpelling(clang_getCursorType(cursor)));
			return fieldTypeNameScoped;
		}
	}

	void CInheritableTypeRegCodeWriter::WriteClass(const CWritingContext& context, CTypeRegClassWrittingData& data) const
	{
		CCodeTemplate tpl;
		ReadTemplateFromRawData(tpl, HardCodedTemplate::InheritableTypeReg_Class);
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_SHARED_3, m_typeName.c_str());
		MapLabelToText(map, LABEL_9, this->GetInfoTypeName().c_str());
		CCodeLines linesRegisterType;
		CTypeRegClassWrittingData dataRegisterType(linesRegisterType, data.m_includePathRequirement);
		this->WriteRegisterType(context, dataRegisterType);
		MapLabelToLines(map, LABEL_4, linesRegisterType);
		CCodeLines linesInitType;
		{
			CCodeLines lines;
			this->WriteImplInitType(lines);
			if (lines.size() > 0)
			{
				CCodeWriter writer;
				writer.WriteLine(HardCodedTemplate::TypeReg_StaticInitType);
				CCodeTemplate tpl;
				ReadTemplateFromRawData(tpl, writer.m_code.c_str());
				CLabelToCodeMapping map;
				MapLabelToLines(map, LABEL_5, lines);
				tpl.ReplaceLabels(map, linesInitType);
			}
		}
		MapLabelToLines(map, LABEL_30, linesInitType);
		auto typeNameWithTypeScopeOnly = GetFieldTypeNameWithScope(m_cursor, m_vecNamespace);
		MapLabelToText(map, LABEL_33, typeNameWithTypeScopeOnly);
		CCodeLines linesCreateAccessorTree;
		CTypeRegClassWrittingData dataCreateFieldLayout(linesCreateAccessorTree, data.m_includePathRequirement);
		this->WriteCreateAccessorTree(context, dataCreateFieldLayout);
		MapLabelToLines(map, LABEL_6, linesCreateAccessorTree);

		//todo: method可能需要合并到InitType中
		CCodeLines linesInitMethod;
		{
			CCodeLines lines;
			this->WriteImplInitMethod(lines);
			if (lines.size() > 0)
			{
				CCodeWriter writerInitType;
				writerInitType.WriteLine(HardCodedTemplate::TypeReg_ImplStaticInitMethod);
				CCodeTemplate tpl;
				ReadTemplateFromRawData(tpl, writerInitType.m_code.c_str());
				CLabelToCodeMapping map;
				MapLabelToText(map, LABEL_SHARED_3, m_typeName);
				MapLabelToLines(map, LABEL_32, lines);
				tpl.ReplaceLabels(map, linesInitMethod);
			}
		}
		MapLabelToLines(map, LABEL_30, linesInitType);

		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl.ReplaceLabels(map, data.m_lines, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() <= map.size());
	}
	Niflect::CString CInheritableTypeRegCodeWriter::WriteTypeRegClassName() const
	{
		return ReplaceLabelToText1(HardCodedTemplate::TypeRegClassScopeName_InheritableType, LABEL_SHARED_3, m_typeName);
	}
	void CInheritableTypeRegCodeWriter::WriteRegisterType(const CWritingContext& context, CTypeRegClassWrittingData& data) const
	{
		CCodeTemplate tpl;
		ReadTemplateFromRawData(tpl, HardCodedTemplate::InheritableTypeReg_RegisterType_Class2);//todo: struct另有模板, 因为不需要构造析构, 相应的也需要提示不支持的用法
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_SHARED_3, m_typeName.c_str());
		//Niflect::CString typeNameForHash;
		//this->WriteTypeNameForHash(context, typeNameForHash, data.m_includePathRequirement);
		//MapLabelToText(map, LABEL_26, typeNameForHash);//todo: namespace & scope
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl.ReplaceLabels(map, data.m_lines, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
	}
	static const char* pszAccessorLevel0 = "0";
	void CInheritableTypeRegCodeWriter::WriteCreateAccessorTree(const CWritingContext& context, CTypeRegClassWrittingData& data) const
	{
		CCodeWriter tplWriter;
		tplWriter.WriteLine(HardCodedTemplate::CreateFieldLayout_CreateField);
		tplWriter.WriteLine(HardCodedTemplate::CreateFieldLayout_GetNodeFromShared2);
		tplWriter.WriteLine(HardCodedTemplate::CreateFieldLayout_InitType222222222);
		tplWriter.WriteLine(MAKELABEL(LABEL_13));
		tplWriter.WriteLine(HardCodedTemplate::CreateFieldLayout_Return2);
		CCodeTemplate tpl;
		ReadTemplateFromRawData(tpl, tplWriter.m_code.c_str());
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_7, pszAccessorLevel0);
		CCodeLines linesInitAccessor;
		CTypeRegClassWrittingData dataInitFieldLayout(linesInitAccessor, data.m_includePathRequirement);
		this->WriteInitAccessor(context, dataInitFieldLayout);
		MapLabelToLines(map, LABEL_13, linesInitAccessor);
		Niflect::CString rootFieldTypeName;
		this->WriteRootFieldTypeName(rootFieldTypeName, data.m_includePathRequirement);
		MapLabelToText(map, LABEL_24, rootFieldTypeName);
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl.ReplaceLabels(map, data.m_lines, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
	}

	CInheritableTypeRegCodeWriter_FieldAccessor::CInheritableTypeRegCodeWriter_FieldAccessor(const CXCursor& cursor, const STypeRegClassWritingSetting& writingSetting, const CXCursor& actualDeclCursor, const CTypeDecl& fieldAccessorBindingType)
		: inherited(cursor, writingSetting)
		, m_actualDeclCursor(actualDeclCursor)
		, m_fieldAccessorBindingType(fieldAccessorBindingType)
	{
	}
	void CInheritableTypeRegCodeWriter_FieldAccessor::GetInvokationRequirement(CTypeRegInvokationData& data) const
	{
		data.m_requiredInvokationInitType = false;
	}
	void CInheritableTypeRegCodeWriter_FieldAccessor::WriteRootFieldTypeName(Niflect::CString& typeName, CNoDupPathCollector& includePathRequirement) const
	{
		typeName = m_typeName;
	}
	void CInheritableTypeRegCodeWriter_FieldAccessor::WriteTypeNameForHash(const CWritingContext& context, Niflect::CString& typeName, CNoDupPathCollector& includePathRequirement) const
	{
		typeName = CXStringToCString(clang_getTypeSpelling(m_fieldAccessorBindingType.m_CXType));
		if (clang_getCursorKind(m_fieldAccessorBindingType.m_cursorDecl) != CXCursor_NoDeclFound)
			this->CollectIncludePathFromCursor(context, m_fieldAccessorBindingType.m_cursorDecl, includePathRequirement);
	}

	Niflect::CString CInheritableTypeRegCodeWriter_FieldAccessor::GetInfoTypeName() const
	{
		return GetCursorInfoTypeName(m_actualDeclCursor);
	}

	CInheritableTypeRegCodeWriter_ObjectAccessor::CInheritableTypeRegCodeWriter_ObjectAccessor(const CXCursor& cursor, const STypeRegClassWritingSetting& writingSetting, const CXCursor& baseTypeCursor, const Niflect::TArrayNif<CTaggedInheritableTypeMember*>& vecMember)
		: inherited(cursor, writingSetting)
		, m_vecMember(vecMember)
	{
		if (!clang_Cursor_isNull(baseTypeCursor))
		{
			m_baseTypeName = CXStringToCString(clang_getCursorSpelling(baseTypeCursor));//todo: namespace
			m_baseTypeNameWithScope = GetFieldTypeNameWithScope(baseTypeCursor, m_vecNamespace);
			m_infoTypeNameOfBaseType = GetCursorInfoTypeName(baseTypeCursor);
		}
	}
	void CInheritableTypeRegCodeWriter_ObjectAccessor::GetInvokationRequirement(CTypeRegInvokationData& data) const
	{
		//todo: 如果method合并到InitType中生成, 则此处为或关系
		data.m_requiredInvokationInitType = this->HasTaggedBaseType();
	}
	void CInheritableTypeRegCodeWriter_ObjectAccessor::WriteDeclInitType(CCodeLines& lines) const
	{
		if (this->HasTaggedBaseType())
			lines.push_back(HardCodedTemplate::TypeReg_DeclStaticInitType);
	}
	void CInheritableTypeRegCodeWriter_ObjectAccessor::WriteImplInitType(CCodeLines& lines) const
	{
		if (this->HasTaggedBaseType())
		{
			CCodeTemplate tpl;
			ReadTemplateFromRawData(tpl, HardCodedTemplate::InheritableTypeReg_InitInheritance2);
			CLabelToCodeMapping map;
			MapLabelToText(map, LABEL_10, m_infoTypeNameOfBaseType.c_str());
			//todo: baseType的namespace
			MapLabelToText(map, LABEL_11, m_baseTypeNameWithScope.c_str());
			Niflect::TSet<Niflect::CString> setReplacedLabel;
			tpl.ReplaceLabels(map, lines, &setReplacedLabel);
			ASSERT(setReplacedLabel.size() == map.size());
		}
	}
	void CInheritableTypeRegCodeWriter_ObjectAccessor::WriteInitAccessor(const CWritingContext& context, CTypeRegClassWrittingData& data) const
	{
		Niflect::TArrayNif<SMemberSSSSSSSSSSS> vecMemberInfo;
		Niflect::TArrayNif<uint32> vecIndex;
		vecIndex.resize(m_vecMember.size());
		for (uint32 idx = 0; idx < m_vecMember.size(); ++idx)
		{
			auto& it = m_vecMember[idx];
			auto& cursorField = it->GetCursor();
			auto type = clang_getCursorType(cursorField);
			CTypeDecl typeDecl(clang_getTypeDeclaration(type), type);
			auto kind = clang_getCursorKind(cursorField);
			bool trueField_falseMethod = kind == CXCursor_FieldDecl;
			vecMemberInfo.push_back({ typeDecl, CXStringToCString(clang_getCursorSpelling(cursorField)), trueField_falseMethod, it->m_vecDetailCursor, vecIndex[idx]});
		}
		Niflect::CString textForTemplateInstance;
		this->WriteInitFieldLayoutCompoundType(m_typeName, 0, INDEX_NONE, textForTemplateInstance, vecMemberInfo, context, data);
	}
	void CInheritableTypeRegCodeWriter_ObjectAccessor::WriteDeclInitMethod(CCodeLines& lines) const
	{

	}
	void CInheritableTypeRegCodeWriter_ObjectAccessor::WriteImplInitMethod(CCodeLines& lines) const
	{

	}
	void CInheritableTypeRegCodeWriter_ObjectAccessor::WriteInitFieldLayoutCompoundType(const Niflect::CString& fieldsOwnerTypeName, uint32 parentAccessorLevel, uint32 parentDimension, Niflect::CString& parentTextForTemplateInstance, const Niflect::TArrayNif<SMemberSSSSSSSSSSS>& vecMemberInfo, const CWritingContext& context, CTypeRegClassWrittingData& data) const
	{
		for (uint32 idx = 0; idx < vecMemberInfo.size(); ++idx)
		{
			CCodeLines linesMember;
			auto& it = vecMemberInfo[idx];
			if (it.m_trueField_falseMethod)
			{

				////auto aaaaaaaaaaa = clang_getCursorType(cursorDecl);
				////auto dddddddddddddd = clang_getTypeDeclaration(aaaaaaaaaaa);

				//auto a = CXStringToCString(clang_getTypeSpelling(type));
				//clang_type_
				//auto b = CXStringToCString(clang_getCursorSpelling(cursorDecl));

				//CSubcursor rrrrr;
				//uint32 ssssss = 0;
				//Niflect::TArrayNif<CXCursor> vecCCCCC;
				//BuildSubcursorRecurs(type, vecCCCCC, ssssss, rrrrr);
				//PrintSubcursorRecurs(rrrrr);

				CSubcursor subcursorRoot;
				Niflect::CString textForTemplateInstance;
				bool withRightAngleBracket = false;
				uint32 dimension = parentDimension;
				CTypeRegClassWrittingData dataInitFieldLayoutRecurs(linesMember, data.m_includePathRequirement);
				if (!this->WriteInitAccessorRecurs(fieldsOwnerTypeName, subcursorRoot, textForTemplateInstance, withRightAngleBracket, true, it.m_typeDecl, it.m_vecDetailCursor, it.m_detailCursorsArrayIndex, it.m_typeDecl, dimension, EOwnerAccessorType::Object, parentAccessorLevel, it.m_memberName, context, dataInitFieldLayoutRecurs))
				{
					//GenLogError(context.m_log, NiflectUtil::FormatString("The type of field %s was not tagged", CXStringToCString(clang_getCursorSpelling(cursor)).c_str()));
					break;
				}
				parentTextForTemplateInstance += textForTemplateInstance;
				if (idx != vecMemberInfo.size() - 1)
					parentTextForTemplateInstance += ", ";
			}
			else
			{
				linesMember.push_back("todo: Method writer\n");
			}

			for (auto& it1 : linesMember)
				data.m_lines.push_back(it1);
		}
	}
	static void TakeNamespaces(CSubcursor& parentSubcursor, Niflect::TArrayNif<CXCursor>& vecTaken)
	{
		const uint32 detailCursorsArrayIndex = 0;
		bool takingDetail = false;
		//bool foundAliasCursor = false;
		if ((clang_getCursorKind(parentSubcursor.m_cursorDecl) != CXCursor_NoDeclFound) || (parentSubcursor.m_CXType.kind == CXType_Unexposed))
		{
			takingDetail = true;
		}
		else if (parentSubcursor.m_CXType.kind == CXType_Invalid)
		{
			takingDetail = true;
			//foundAliasCursor = true;
		}
		if (takingDetail)
		{
			while (clang_getCursorKind(parentSubcursor.m_vecAaaaaaaaaa[detailCursorsArrayIndex]) == CXCursor_NamespaceRef)
			{
				vecTaken.push_back(parentSubcursor.m_vecAaaaaaaaaa[detailCursorsArrayIndex]);
				parentSubcursor.m_vecAaaaaaaaaa.erase(parentSubcursor.m_vecAaaaaaaaaa.begin());
			}
		}
	}
	static Niflect::CString ReplaceTemplateTypeNameAndGenerateCode(const CXType& underlyingType, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, const CSubcursor& subcursorSrc)
	{
		CSubcursor qqqqqqqqqq;
		BuildSubcursor(underlyingType, vecDetailCursor, qqqqqqqqqq, true);
		//PrintSubcursorRecurs(qqqqqqqqqq);

		Niflect::TArrayNif<CXCursor> vecNamespaceDst;
		TakeNamespaces(qqqqqqqqqq, vecNamespaceDst);
		qqqqqqqqqq.m_cursorDecl = subcursorSrc.m_cursorDecl;
		qqqqqqqqqq.m_CXType = subcursorSrc.m_CXType;
		qqqqqqqqqq.m_vecAaaaaaaaaa = subcursorSrc.m_vecAaaaaaaaaa;
		Niflect::CString str;
		GenerateTemplateInstanceCode(qqqqqqqqqq, str);
		return str;
	}
	static void AAAAAAAAAAAAAA(const CCodeLines& linesInitAccessor, CCodeLines& lines2)
	{
		CCodeTemplate tpl;
		ReadTemplateFromRawData(tpl, HardCodedTemplate::InitField_Scope);
		CLabelToCodeMapping map;
		MapLabelToLines(map, LABEL_14, linesInitAccessor);
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl.ReplaceLabels(map, lines2, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
	}
	bool CInheritableTypeRegCodeWriter_ObjectAccessor::HasTaggedBaseType() const
	{
		return !m_baseTypeName.empty();
	}
	bool CInheritableTypeRegCodeWriter_ObjectAccessor::WriteInitAccessorRecurs(const Niflect::CString& fieldsOwnerTypeName, CSubcursor& parentSubcursor, Niflect::CString& parentTextForTemplateInstance, bool& withRightAngleBracket, bool isTopLevelField, const CTypeDecl& typeDeclaaaaaaaaaaa, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, uint32& detailCursorsArrayIndex, const CTypeDecl& typeDecl, uint32& dimension, EOwnerAccessorType ownerAccessorType, uint32 parentAccessorLevel, const Niflect::CString& internalName, const CWritingContext& context, CTypeRegClassWrittingData& data) const
	{
		CCodeLines linesInitAccessor;
		bool ok = true;

		parentSubcursor.m_cursorDecl = typeDeclaaaaaaaaaaa.m_cursorDecl;
		parentSubcursor.m_CXType = typeDeclaaaaaaaaaaa.m_CXType;

		if (detailCursorsArrayIndex < vecDetailCursor.size())
		{
			bool takingDetail = false;
			//bool foundAliasCursor = false;
			if ((clang_getCursorKind(parentSubcursor.m_cursorDecl) != CXCursor_NoDeclFound) || (parentSubcursor.m_CXType.kind == CXType_Unexposed))
			{
				takingDetail = true;
			}
			else if (parentSubcursor.m_CXType.kind == CXType_Invalid)
			{
				takingDetail = true;
				//foundAliasCursor = true;
			}
			if (takingDetail)
			{
				while (clang_getCursorKind(vecDetailCursor[detailCursorsArrayIndex]) == CXCursor_NamespaceRef)
				{
					parentSubcursor.m_vecAaaaaaaaaa.push_back(vecDetailCursor[detailCursorsArrayIndex]);
					detailCursorsArrayIndex++;
				}
				parentSubcursor.m_vecAaaaaaaaaa.push_back(vecDetailCursor[detailCursorsArrayIndex]);
				detailCursorsArrayIndex++;

				//if (foundAliasCursor)
				//{
				//	parentSubcursor.m_cursorDecl = parentSubcursor.m_vecAaaaaaaaaa.back();
				//	parentSubcursor.m_CXType = clang_getCursorType(parentSubcursor.m_cursorDecl);
				//}
			}
		}

		auto cursorDecl = parentSubcursor.m_cursorDecl;
		auto underlyingType = parentSubcursor.m_CXType;
		auto cursorDeclKind = clang_getCursorKind(cursorDecl);
		switch (cursorDeclKind)
		{
		case CXCursor_FieldDecl:
			underlyingType = clang_getCursorType(cursorDecl);
			cursorDecl = clang_getTypeDeclaration(underlyingType);
			cursorDeclKind = clang_getCursorKind(cursorDecl);
			break;
		}
		//switch (cursorDeclKind)
		//{
		//case CXCursor_TypedefDecl:
		//case CXCursor_TypeAliasDecl:
		//{
		//	underlyingType = getUnderlyingType(underlyingType);
		//	cursorDecl = clang_getTypeDeclaration(underlyingType);
		//	cursorDeclKind = clang_getCursorKind(cursorDecl);
		//}
		//break;
		//default:
		//	break;
		//}
		//parentSubcursor.m_cursorDecl = cursorDecl;
		//parentSubcursor.m_CXType = underlyingType;

		const CAccessorBinding* accessorBinding = NULL;
		if (cursorDeclKind != CXCursor_NoDeclFound)
		{
			accessorBinding = m_writingSetting.deprecated_m_mapping.m_accessorBindingMapping.FindByCursorDecl(cursorDecl);
			if (accessorBinding == NULL)
			{
				if (parentSubcursor.m_vecAaaaaaaaaa.size() > 0)
				{
					//别名
					//todo: namespace在m_vecAaaaaaaaaa的其它item中
					auto refDecl = clang_getCursorReferenced(parentSubcursor.m_vecAaaaaaaaaa.back());
					if (clang_getCursorKind(refDecl) != CXCursor_NoDeclFound)
					{
						accessorBinding = m_writingSetting.deprecated_m_mapping.m_accessorBindingMapping.FindByCursorDecl(refDecl);
						if (accessorBinding != NULL)
							cursorDecl = refDecl;
					}
				}
				if (accessorBinding == NULL)
				{
					if (cursorDeclKind == CXCursor_ClassDecl)
					{
						auto templateDecl = clang_getSpecializedCursorTemplate(cursorDecl);
						if ((!clang_Cursor_isNull(templateDecl)) && clang_getCursorKind(templateDecl) != CXCursor_NoDeclFound)
							accessorBinding = m_writingSetting.deprecated_m_mapping.m_accessorBindingMapping.FindByCursorDecl(templateDecl);
					}
				}
			}
		}
		else
		{
			auto& underlyingTypeKind = underlyingType.kind;
			if (underlyingTypeKind == CXType_Pointer)
			{
				GenLogError(context.m_log, "Pointer field is not supported");
				ok = false;
			}
			accessorBinding = m_writingSetting.deprecated_m_mapping.m_accessorBindingMapping.FindByCXType(underlyingType);
		}

		Niflect::CString myTypeName;
		Niflect::CString textForTemplateInstance;

		CCodeLines linesScope;
		if (accessorBinding != NULL)
		{
			//不需要为成员类型添加include, 仅指针类型成员可只声明不引用include, 且已为owner类添加include, 因此只要被解析的代码本身是正确的, 就不需要为成员添加include
			//this->CollectIncludePathFromCursor(context, cursorDecl, data.m_includePathRequirement);

			if (IsCursorTemplateDecl(cursorDecl))
			{
				if (accessorBinding->Is1D())
				{
					auto templateArgsCount = clang_Type_getNumTemplateArguments(underlyingType);
					if (templateArgsCount > 0)
					{
						parentSubcursor.m_vecChild.resize(templateArgsCount);
						bool isLastChildWithRightAngleBracket = false;
						//myTypeName += GetNameFromCursorOrTypeDeclaration(cursorDecl, underlyingType);
						auto templateFieldTypeName = GetFieldTypeNameWithScope(cursorDecl, m_vecNamespace);
						myTypeName += templateFieldTypeName;
						dimension++;
						for (int idx = 0; idx < templateArgsCount; ++idx)
						{
							myTypeName += "<";
							CXType argType = clang_Type_getTemplateArgumentAsType(underlyingType, idx);
							auto argCursorDecl = clang_getTypeDeclaration(argType);
							Niflect::CString childText;
							CTypeRegClassWrittingData dataInitFieldLayoutRecursForChild(linesScope, data.m_includePathRequirement);
							this->WriteInitAccessorRecurs(fieldsOwnerTypeName, parentSubcursor.m_vecChild[idx], childText, isLastChildWithRightAngleBracket, false, CTypeDecl(argCursorDecl, argType), vecDetailCursor, detailCursorsArrayIndex, CTypeDecl(argCursorDecl, argType), dimension, EOwnerAccessorType::Array, parentAccessorLevel + 1, NiflectUtil::FormatString("reserved_dim%u", dimension), context, dataInitFieldLayoutRecursForChild);
							myTypeName += childText;
							if (idx != parentSubcursor.m_vecChild.size() - 1)
								myTypeName += ", ";
						}
						dimension--;
						if (isLastChildWithRightAngleBracket)
							myTypeName += " ";
						myTypeName += ">";
						withRightAngleBracket = true;

						CTypeRegInitFieldLayoutWrittingData dataInitFieldLayoutLines(linesInitAccessor, data.m_includePathRequirement, NULL);
						this->WriteInitAccessorLines(fieldsOwnerTypeName, HardCodedTemplate::StaticGetType_Registered2, linesScope, ownerAccessorType, parentAccessorLevel, internalName, accessorBinding->m_accessorSubcursor, myTypeName, context, dataInitFieldLayoutLines);
					}
					else
					{
						GenLogError(context.m_log, "Fail to retrieve template instance arguments");
						ok = false;
					}
				}
				else if (accessorBinding->Is2D())
				{
					auto& wwwwww = accessorBinding->m_vecWWWW[1];
					auto& aaaaaa = wwwwww.m_subcursor.m_cursorDecl;
					//auto a = CXStringToCString(clang_getTypeSpelling(wwwwww.m_subcursor.m_CXType));
					//auto b = CXStringToCString(clang_getCursorSpelling(aaaaaa));
					auto itFound = m_writingSetting.deprecated_m_mapping.m_mapCursorDeclToUntaggedTemplate.find(aaaaaa);
					if (itFound != m_writingSetting.deprecated_m_mapping.m_mapCursorDeclToUntaggedTemplate.end())
					{
						auto elemFieldsOwnerTypeName = ReplaceTemplateTypeNameAndGenerateCode(underlyingType, vecDetailCursor, wwwwww.m_subcursor);

						uint32 templateArgsCount = clang_Type_getNumTemplateArguments(underlyingType);
						auto templateArgsCountOriginalDecl = clang_Type_getNumTemplateArguments(wwwwww.m_subcursor.m_CXType);
						if (templateArgsCount == templateArgsCountOriginalDecl && templateArgsCount == itFound->second->DebugGetChildren().size())
						{
							CCodeLines linesScope2;
							Niflect::TArrayNif<SMemberSSSSSSSSSSS> vecMemberInfo;
							auto& vecChild = itFound->second->DebugGetChildren();
							for (uint32 idx = 0; idx < templateArgsCount; ++idx)
							{
								ASSERT(false);
								//auto member = CTaggedInheritableTypeMember::Cast(vecChild[idx].Get());
								//auto& cursorField = member->GetCursor();
								//if (clang_getCXXAccessSpecifier(cursorField) != CX_CXXAccessSpecifier::CX_CXXPublic)
								//{
								//	//此处 template type 并不是指上处 m_mapCursorDeclToUntaggedTemplate 中对应的 Untagged Template
								//	//框架概念中并没有 Tagged Template, 即本就不支持模板类型反射, 因此提示信息中, 描述的是
								//	//对于所有 Template 类型, 被解析需要子 Accessor 时, 只支持对应的为 Template 的 BindingType 其成员定义在 Public 作用域
								//	GenLogError(context.m_log, "Field access scope must be public for a template type");
								//	ok = false;
								//	break;
								//}
								//auto type = clang_getCursorType(cursorField);
								//CXType argType = clang_Type_getTemplateArgumentAsType(underlyingType, idx);
								//auto argCursorDecl = clang_getTypeDeclaration(argType);
								//CTypeDecl typeDecl(argCursorDecl, argType);
								//auto kind = clang_getCursorKind(cursorField);
								//bool trueField_falseMethod = kind == CXCursor_FieldDecl;
								//ASSERT(trueField_falseMethod);
								//vecMemberInfo.push_back({ typeDecl, CXStringToCString(clang_getCursorSpelling(cursorField)), trueField_falseMethod, vecDetailCursor, detailCursorsArrayIndex });
							}
							dimension++;
							CTypeRegClassWrittingData dataInitFieldLayout2ForCompoundType(linesScope2, data.m_includePathRequirement);
							this->WriteInitFieldLayoutCompoundType(elemFieldsOwnerTypeName, parentAccessorLevel + 2, dimension, textForTemplateInstance, vecMemberInfo, context, dataInitFieldLayout2ForCompoundType);

							if (auto sssssssssss = m_writingSetting.deprecated_m_mapping.m_accessorBindingMapping.FindByCursorDecl(wwwwww.m_subcursor.m_cursorDecl))
							{
								CCodeLines linesScope1;
								//auto a = CXStringToCString(clang_getCursorSpelling(sssssssssss->m_accessorCursorDecl));
								//auto b = CXStringToCString(clang_getCursorSpelling(sssssssssss->m_vecWWWW[0].m_subcursor.m_cursorDecl));
								//printf("");

								{
									CCodeLines linesScope1111111;
									CTypeRegInitFieldLayoutWrittingData dataInitFieldLayout1Lines(linesScope1111111, data.m_includePathRequirement, NULL);
									this->WriteInitAccessorLines(fieldsOwnerTypeName, HardCodedTemplate::StaticGetType_Registered2, linesScope2, EOwnerAccessorType::Array, parentAccessorLevel + 1, NiflectUtil::FormatString("reserved_dim%u", dimension), sssssssssss->m_accessorSubcursor, elemFieldsOwnerTypeName, context, dataInitFieldLayout1Lines);
									AAAAAAAAAAAAAA(linesScope1111111, linesScope1);
								}

								CSubcursor qqqqqqqqqq;
								BuildSubcursor(underlyingType, vecDetailCursor, qqqqqqqqqq, true);
								Niflect::CString str;
								GenerateTemplateInstanceCode(qqqqqqqqqq, str);
								CTypeRegInitFieldLayoutWrittingData dataInitFieldLayoutLines(linesInitAccessor, data.m_includePathRequirement, NULL);
								this->WriteInitAccessorLines(fieldsOwnerTypeName, HardCodedTemplate::StaticGetType_Registered2, linesScope1, ownerAccessorType, parentAccessorLevel, internalName, accessorBinding->m_accessorSubcursor, str, context, dataInitFieldLayoutLines);
							}
							dimension--;
						}
						else
						{
							GenLogError(context.m_log, NiflectUtil::FormatString("The element binding type's template arguments must match the container's"));
							ok = false;
						}
					}
				}
				else
				{
					GenLogError(context.m_log, NiflectUtil::FormatString("Dimensional binding types must not be more than 2D"));
					ok = false;
				}
			}
			else
			{
				Niflect::CString str = CXStringToCString(clang_getTypeSpelling(underlyingType));
				Niflect::CString sdasdf = HardCodedTemplate::StaticGetType_Registered2;
				//auto kind = clang_getCursorKind(accessorBinding->m_accessorSubcursor.m_cursorDecl);
				//if (accessorBinding->m_accessorSubcursor.m_vecChild.size() == 0
				//	|| (kind == CXCursor_TypeAliasDecl)//todo: 不确定如何区分别名是否为模板的别名, 因此应急用此特殊检查认定为直接使用此别名
				//	)
				//{
				//}
				//else
				//{
				//	sdasdf = HardCodedTemplate::StaticGetType_Misc;
				//}
				CTypeRegInitFieldLayoutWrittingData dataInitFieldLayoutLines(linesInitAccessor, data.m_includePathRequirement, NULL);
				this->WriteInitAccessorLines(fieldsOwnerTypeName, sdasdf, linesScope, ownerAccessorType, parentAccessorLevel, internalName, accessorBinding->m_accessorSubcursor, str, context, dataInitFieldLayoutLines);
			}
		}
		else
		{
			if (cursorDeclKind != CXCursor_NoDeclFound)
			{
				bool canWrite = false;
				if (isTopLevelField)
				{
					auto itFound = m_writingSetting.deprecated_m_mapping.m_mapCursorDeclToTaggedType.find(cursorDecl);
					if (itFound != m_writingSetting.deprecated_m_mapping.m_mapCursorDeclToTaggedType.end())
					{
						canWrite = true;
					}
					else
					{
						GenLogError(context.m_log, NiflectUtil::FormatString("Generating failed for field %s in type %s", parentSubcursor.GetTypeName().c_str(), internalName.c_str()));
						ok = false;
					}
				}
				else
				{
					canWrite = true;
				}
				if (canWrite)
				{
					//由于使用函数模板定义StateGetType, 因此可能不能以别名调用函数模板实例, 否则可能出现找错signature, 如认为有必要用origianlCursorDecl生成代码, 则需要实际测试, 例如生成的类型在其它module中定义, 生成代码link时不出现链接错误即为正确
					CTypeRegInitFieldLayoutWrittingData dataInitFieldLayoutLines(linesInitAccessor, data.m_includePathRequirement, &myTypeName);
					CSubcursor aaaaaaaa;
					aaaaaaaa.m_cursorDecl = cursorDecl;
					this->WriteInitAccessorLines(fieldsOwnerTypeName, HardCodedTemplate::StaticGetType_Registered2, linesScope, ownerAccessorType, parentAccessorLevel, internalName, aaaaaaaa, "", context, dataInitFieldLayoutLines);
				}
			}
			else
			{
				GenLogError(context.m_log, NiflectUtil::FormatString("Generating failed for field %s in type %s", internalName.c_str(), parentSubcursor.GetTypeName().c_str()));
				ok = false;
			}
		}
		if (ok)
		{
			if (myTypeName.empty())
			{
				myTypeName = GetNameFromCursorOrTypeDeclaration(cursorDecl, underlyingType);//todo: namespace & scope
			}
			//todo: 也可以用于其它类型的名称, 并加上namespace与scope
			parentTextForTemplateInstance += myTypeName;

			//if (!textForTemplateInstance.empty())
			//	parentTextForTemplateInstance += NiflectUtil::FormatString("<%s>", textForTemplateInstance.c_str());

			if (linesInitAccessor.size() > 0)
			{
				AAAAAAAAAAAAAA(linesInitAccessor, data.m_lines);
			}
		}
		return ok;
	}
	void CInheritableTypeRegCodeWriter_ObjectAccessor::WriteRootFieldTypeName(Niflect::CString& typeName, CNoDupPathCollector& includePathRequirement) const
	{
		//可通过BindingSetting指定CompooundField, 从而避免依赖常量
		typeName = NiflectGenDefinition::NiflectFramework::AccessorTypeName::CompoundField;
#ifdef USER_PROVIDED_CCOMPONENTFIELD_CLASS_DEFINITION
		includePathRequirement.Add(NiflectGenDefinition::NiflectFramework::FilePath::CompoundFieldHeader);
#endif
	}
	void CInheritableTypeRegCodeWriter_ObjectAccessor::WriteTypeNameForHash(const CWritingContext& context, Niflect::CString& typeName, CNoDupPathCollector& includePathRequirement) const
	{
		typeName = m_typeName;
	}
	Niflect::CString CInheritableTypeRegCodeWriter_ObjectAccessor::GetInfoTypeName() const
	{
		return GetCursorInfoTypeName(m_cursor);
	}
	void CInheritableTypeRegCodeWriter_ObjectAccessor::WriteInitAccessorLines(const Niflect::CString& fieldsOwnerTypeName, const Niflect::CString& templateStaticGetType, const CCodeLines& linesScope, EOwnerAccessorType ownerAccessorType, uint32 parentAccessorLevel, const Niflect::CString& internalName, const CSubcursor& accessorSubcursor, const Niflect::CString& bindingTypeName, const CWritingContext& context, CTypeRegInitFieldLayoutWrittingData& data) const
	{
		const CXCursor& fieldCursorDecl = accessorSubcursor.m_cursorDecl;
		Niflect::CString registeredOrMiscTypeName;
		auto kind = clang_getCursorKind(fieldCursorDecl);
		Niflect::CString bindingTypeForStaticGetTypeMisc;
		//if ((accessorSubcursor.m_vecChild.size() == 0) 
		//	|| (kind == CXCursor_TypeAliasDecl)//todo: 不确定如何区分别名是否为模板的别名, 因此应急用此特殊检查认定为直接使用此别名
		//	)
		//{
		//	//registeredOrMiscTypeName = GetFieldTypeNameWithScope(fieldCursorDecl, m_vecNamespace);
		//	//this->CollectIncludePathFromCursor(context, fieldCursorDecl, data.m_includePathRequirement);
		//	registeredOrMiscTypeName = bindingTypeName;
		//}
		//else
		//{
		//	Niflect::TArrayNif<Niflect::CString> vecTemplateArgReplacementString;
		//	vecTemplateArgReplacementString.push_back(bindingTypeName);
		//	GenerateTemplateInstanceCode(accessorSubcursor, registeredOrMiscTypeName, vecTemplateArgReplacementString);
		//	registeredOrMiscTypeName = FindNamespaceAndTypeScope(accessorSubcursor.m_cursorDecl, m_vecNamespace) + registeredOrMiscTypeName;
		//	bindingTypeForStaticGetTypeMisc = bindingTypeName;
		//}

		registeredOrMiscTypeName = bindingTypeName;
		if (NiflectUtil::EndsWith(registeredOrMiscTypeName, '>'))
			registeredOrMiscTypeName += ' ';

		CCodeWriter tplWriter;
		tplWriter.WriteLine(templateStaticGetType);
		tplWriter.WriteLine(HardCodedTemplate::InitField_CreateForMember);
		tplWriter.WriteLine(MAKELABEL(LABEL_14));
		Niflect::CString templateAssignToOwner;
		Niflect::CString templateAccessorOffset;
		switch (ownerAccessorType)
		{
		case EOwnerAccessorType::Object:
			templateAccessorOffset = HardCodedTemplate::GetMemberVariableOffset2;
			templateAssignToOwner = HardCodedTemplate::InitField_AssignToOwner;
			break;
		case EOwnerAccessorType::Array:
			templateAccessorOffset = HardCodedTemplate::NoOffset2;
			templateAssignToOwner = HardCodedTemplate::InitField_AssignElementToOwner;
			break;
		default:
			ASSERT(false);
			break;
		}
		tplWriter.WriteLine(templateAssignToOwner);
		CCodeWriter writerAccessorOffset;
		{
			CCodeTemplate tpl0;
			ReadTemplateFromRawData(tpl0, templateAccessorOffset.c_str());
			CLabelToCodeMapping map0;
			MapLabelToText(map0, LABEL_21, internalName);
			MapLabelToText(map0, LABEL_25, fieldsOwnerTypeName);
			CCodeLines lines0;
			tpl0.ReplaceLabels(map0, lines0);
			ASSERT(lines0.size() == 1);
			writerAccessorOffset.Write(lines0.back());
		}
		auto accessorLevel = parentAccessorLevel + 1;
		auto strAccessorLevel = NiflectUtil::FormatString("%u", accessorLevel);
		auto textGetNodeFromShared = ReplaceLabelToText1(HardCodedTemplate::CreateFieldLayout_GetNodeFromShared2, LABEL_7, strAccessorLevel);
		CCodeTemplate tpl1;
		ReadTemplateFromRawData(tpl1, tplWriter.m_code.c_str());
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_7, strAccessorLevel);
		MapLabelToText(map, LABEL_12, registeredOrMiscTypeName);
		MapLabelToLines(map, LABEL_14, linesScope);
		MapLabelToText(map, LABEL_19, NiflectUtil::FormatString("%u", parentAccessorLevel));
		MapLabelToText(map, LABEL_20, writerAccessorOffset.m_code);
		MapLabelToText(map, LABEL_21, internalName);
		MapLabelToText(map, LABEL_23, textGetNodeFromShared);
		if (!bindingTypeForStaticGetTypeMisc.empty())
			MapLabelToText(map, LABEL_27, bindingTypeForStaticGetTypeMisc);
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl1.ReplaceLabels(map, data.m_lines, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
	}
}
