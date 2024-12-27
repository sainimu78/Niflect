#pragma once
#include "NiflectGen/CodeWriter/TypeReg/TypeRegCodeWriter.h"
#include "NiflectGen/Collector/CollectionData.h"
#include "NiflectGen/Resolver/ResolvedData.h"
#include "NiflectGen/TaggedType/TaggedInheritableType.h"

namespace NiflectGen
{
	class CInheritableTypeRegCodeWriter : public CTypeRegCodeWriter
	{
		typedef CTypeRegCodeWriter inherited;
	public:
		CInheritableTypeRegCodeWriter(const CXCursor& cursor, const STypeRegClassWritingSetting& writingSetting);

	public:
		virtual void WriteDecl(const CWritingContext& context, CTypeRegDeclWrittingData& data) const override;
		virtual void WriteImpl(const CWritingContext& context, CTypeRegImplWrittingData& data) const override;
		virtual void WriteClass(const CWritingContext& context, CTypeRegClassWrittingData& data) const override;
		virtual Niflect::CString WriteTypeRegClassName() const override;

	private:
		void WriteRegisterType(const CWritingContext& context, CTypeRegClassWrittingData& data) const;
		void WriteCreateAccessorTree(const CWritingContext& context, CTypeRegClassWrittingData& data) const;

	protected:
		virtual void WriteDeclInitType(CCodeLines& lines) const {}
		virtual void WriteImplInitType(CCodeLines& lines) const {}
		virtual void WriteInitAccessor(const CWritingContext& context, CTypeRegClassWrittingData& data) const {}
		virtual void WriteDeclInitMethod(CCodeLines& lines) const {}
		virtual void WriteImplInitMethod(CCodeLines& lines) const {}
		virtual void WriteRootFieldTypeName(Niflect::CString& typeName, CNoDupPathCollector& includePathRequirement) const = 0;
		virtual void WriteTypeNameForHash(const CWritingContext& context, Niflect::CString& typeName, CNoDupPathCollector& includePathRequirement) const = 0;
	};

	class CInheritableTypeRegCodeWriter_FieldAccessor : public CInheritableTypeRegCodeWriter
	{
		typedef CInheritableTypeRegCodeWriter inherited;
	public:
		CInheritableTypeRegCodeWriter_FieldAccessor(const CXCursor& cursor, const STypeRegClassWritingSetting& writingSetting, const CXCursor& actualDeclCursor, const CTypeDecl& fieldAccessorBindingType);

	public:
		virtual void GetInvokationRequirement(CTypeRegInvokationData& data) const override;

	protected:
		virtual void WriteRootFieldTypeName(Niflect::CString& typeName, CNoDupPathCollector& includePathRequirement) const override;
		virtual void WriteTypeNameForHash(const CWritingContext& context, Niflect::CString& typeName, CNoDupPathCollector& includePathRequirement) const override;
		virtual Niflect::CString GetInfoTypeName() const override;

	private:
		const CXCursor& m_actualDeclCursor;
		const CTypeDecl& m_fieldAccessorBindingType;
	};

	struct SMemberSSSSSSSSSSS
	{
		CTypeDecl m_typeDecl;
		Niflect::CString m_memberName;
		bool m_trueField_falseMethod;
		const Niflect::TArrayNif<CXCursor>& m_vecDetailCursor;
		uint32& m_detailCursorsArrayIndex;
	};

	class CTypeRegInitFieldLayoutWrittingData : public CTypeRegClassWrittingData
	{
		typedef CTypeRegClassWrittingData inherited;
	public:
		CTypeRegInitFieldLayoutWrittingData(CCodeLines& lines, CNoDupPathCollector& includePathRequirement, Niflect::CString* fieldTypeName_reserved)
			: inherited(lines, includePathRequirement)
			, m_fieldTypeName_reserved(NULL)
		{

		}

		Niflect::CString* m_fieldTypeName_reserved;
	};

	class CInheritableTypeRegCodeWriter_ObjectAccessor : public CInheritableTypeRegCodeWriter
	{
		typedef CInheritableTypeRegCodeWriter inherited;
	public:
		CInheritableTypeRegCodeWriter_ObjectAccessor(const CXCursor& cursor, const STypeRegClassWritingSetting& writingSetting, const CXCursor& baseTypeCursor, const Niflect::TArrayNif<CTaggedInheritableTypeMember*>& vecMember);

	public:
		virtual void GetInvokationRequirement(CTypeRegInvokationData& data) const override;

	protected:
		virtual void WriteDeclInitType(CCodeLines& lines) const override;
		virtual void WriteImplInitType(CCodeLines& lines) const override;
		virtual void WriteInitAccessor(const CWritingContext& context, CTypeRegClassWrittingData& data) const override;
		virtual void WriteDeclInitMethod(CCodeLines& lines) const override;
		virtual void WriteImplInitMethod(CCodeLines& lines) const override;
		virtual void WriteRootFieldTypeName(Niflect::CString& typeName, CNoDupPathCollector& includePathRequirement) const override;
		virtual void WriteTypeNameForHash(const CWritingContext& context, Niflect::CString& typeName, CNoDupPathCollector& includePathRequirement) const override;
		virtual Niflect::CString GetInfoTypeName() const override;

	private:
		enum class EOwnerAccessorType
		{
			Object,
			Array,
		};

	private:
		bool HasTaggedBaseType() const;
		bool WriteInitAccessorRecurs(const Niflect::CString& fieldsOwnerTypeName, CSubcursor& parentSubcursor, Niflect::CString& parentTextForTemplateInstance, bool& withRightAngleBracket, bool isTopLevelField, const CTypeDecl& typeDeclaaaaaaaaaaa, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, uint32& detailCursorsArrayIndex, const CTypeDecl& typeDecl, uint32& dimension, EOwnerAccessorType ownerAccessorType, uint32 parentAccessorLevel, const Niflect::CString& internalName, const CWritingContext& context, CTypeRegClassWrittingData& data) const;
		void WriteInitAccessorLines(const Niflect::CString& fieldsOwnerTypeName, const Niflect::CString& templateStaticGetType, const CCodeLines& linesScope, EOwnerAccessorType ownerAccessorType, uint32 parentAccessorLevel, const Niflect::CString& internalName, const CSubcursor& accessorSubcursor, const Niflect::CString& bindingTypeName, const CWritingContext& context, CTypeRegInitFieldLayoutWrittingData& data) const;
		void WriteInitFieldLayoutCompoundType(const Niflect::CString& fieldsOwnerTypeName, uint32 parentAccessorLevel, uint32 parentDimension, Niflect::CString& parentTextForTemplateInstance, const Niflect::TArrayNif<SMemberSSSSSSSSSSS>& vecMemberInfo, const CWritingContext& context, CTypeRegClassWrittingData& data) const;
		//bool WriteInitAccessorRecurs2(CSubcursor& parentSubcursor, Niflect::CString& parentTextForTemplateInstance, bool& withRightAngleBracket, bool isTopLevelField, const CTypeDecl& typeDeclaaaaaaaaaaa, const Niflect::TArrayNif<CXCursor>& vecDetailCursor, uint32& detailCursorsArrayIndex, const CTypeDecl& typeDecl, uint32& dimension, EOwnerAccessorType ownerAccessorType, uint32 parentAccessorLevel, const Niflect::CString& internalName, const CWritingContext& context, CCodeLines& lines2) const;

	private:
		Niflect::TArrayNif<CTaggedInheritableTypeMember*> m_vecMember;
		Niflect::CString m_baseTypeName;
		Niflect::CString m_baseTypeNameWithScope;
		Niflect::CString m_infoTypeNameOfBaseType;
	};
}