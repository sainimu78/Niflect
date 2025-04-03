#pragma once
#include "NiflectGen/CodeWriter/CodeWriter.h"
#include "NiflectGen/CodeWriter/TypeReg/TypeRegCode.h"
#include "clang-c/Index.h"
#include "NiflectGen/CodeWriter/CodeRequirement.h"
#include "NiflectGen/Resolver/ResocursorNode.h"
#include "NiflectGen/CodeWriter/MacroDefinitionWriter.h"
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCode.h"

namespace NiflectGen
{
	class CResolvedData;

	class CTypeRegDeclWrittingData
	{
	public:
		CTypeRegDeclWrittingData(CCodeLines& lines)
			: m_lines(lines)
		{

		}

		CCodeLines& m_lines;
	};

	class CTypeRegImplWrittingData
	{
	public:
		CTypeRegImplWrittingData(CCodeLines& lines)
			: m_lines(lines)
		{

		}

		CCodeLines& m_lines;
	};

	class CTypeRegClassWrittingData
	{
	public:
		CTypeRegClassWrittingData(CCodeLines& lines, CNoDupPathCollector& includePathRequirement)
			: m_lines(lines)
			, m_includePathRequirement(includePathRequirement)
		{

		}

		CCodeLines& m_lines;
		CNoDupPathCollector& m_includePathRequirement;
	};

	class CResolvedMapping;
	class CResolvingDependenciesContext;

	struct STypeRegClassWritingSetting
	{
		const Niflect::TArrayNif<Niflect::CString>& m_vecHeaderSearchPath;
		const CResolvedMapping& deprecated_m_mapping;
		const CResolvingDependenciesContext& m_resolvedMappings;
	};

	class CTypeRegCodeWriter
	{
	public:
		CTypeRegCodeWriter(const CXCursor& cursor, const STypeRegClassWritingSetting& writingSetting);

	public:
		virtual void Deprecated_Init() {}
		virtual void WriteDecl(const CWritingContext& context, CTypeRegDeclWrittingData& data) const
		{
		}
		virtual void WriteImpl(const CWritingContext& context, CTypeRegImplWrittingData& data) const
		{
		}
		virtual void WriteClass(const CWritingContext& context, CTypeRegClassWrittingData& data) const = 0;
		void WriteGenHeaderDeclaration(const CWritingContext& context, STypeRegGenHeaderDeclarationWritingData& data) const;
		void WritePrivateHeaderIncludePath(const CWritingContext& context, CNoDupPathCollector& includePathRequirement) const;
		virtual Niflect::CString WriteTypeRegClassName() const = 0;
		virtual void GetInvokationRequirement(CTypeRegInvokationData& data) const = 0;
		virtual Niflect::CString GetInfoTypeName() const = 0;
		const Niflect::TArrayNif<Niflect::CString>& GetNamespaces() const
		{
			return m_vecNamespace;
		}

	protected:
		void CollectIncludePathFromCursor(const CWritingContext& context, const CXCursor& cursor, CNoDupPathCollector& includePathRequirement) const;

	protected:
		const CXCursor& m_cursor;
		Niflect::CString m_typeName;
		Niflect::CString m_typeNameWithScope;
		Niflect::TArrayNif<Niflect::CString> m_vecNamespace;
		const STypeRegClassWritingSetting m_writingSetting;
	};

	using CSharedTypeRegCodeWriter = Niflect::TSharedPtr<CTypeRegCodeWriter>;

	class CTypeRegClassWritingData2
	{
	public:
		CCodeLines m_linesImpl;
	};

	class CDependencyHeaderFilePathAddrs
	{
	public:
		Niflect::TArrayNif<const Niflect::CString*> m_vecDecl;
		Niflect::TArrayNif<const Niflect::CString*> m_vecImpl;
	};

	class CTypeRegRegisterAndFieldLayoutWritingData
	{
	public:
		CTypeRegRegisterAndFieldLayoutWritingData()
			: m_taggedTypeHeaderFilePathAddr(NULL)
		{

		}
		CCodeLines m_linesInvokeRegisterType;
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		CCodeLines m_linesBuildTypeMetaFuncDecl;
		CCodeLines m_linesBuildTypeMetaFuncImpl;
#else
		CCodeLines m_linesCreateFieldLayoutOfTypeDecl;
		CCodeLines m_linesCreateFieldLayoutOfTypeImpl;
#endif
		Niflect::CString m_fieldLayoutFuncName;
		const Niflect::CString* m_taggedTypeHeaderFilePathAddr;//只需要引用缓存在IndexedNode中的路径地址, 在生成无重复includes时才需要获取实际字符串
		CDependencyHeaderFilePathAddrs m_dependencyHeaderFilePathAddrs;
	};

	class CTypeRegTaggedTypeInitWritingData2
	{
	public:
		CCodeLines m_lines;
	};

	class CTypeRegTaggedTypeGeneratedHeaderData
	{
	public:
		CTypeRegTaggedTypeGeneratedHeaderData()
			: m_vecScopeNameAddr(NULL)
		{
		}
		Niflect::CString m_specTemplateRequiredTypeDecl;
		const Niflect::TArrayNif<Niflect::CString>* m_vecScopeNameAddr;
		CCodeLines m_linesStaticGetTypeSpecDecl;
		CCodeLines m_linesStaticGetTypeSpecImpl;
		CTypeLineNumberMacroData m_lineNumberMacroData;
	};

	class CTypeRegWritingData2
	{
	public:
		CTypeRegRegisterAndFieldLayoutWritingData m_registerTypeAndfieldLayout;
		CTypeRegTaggedTypeInitWritingData2 m_taggedTypeInit;
		CTypeRegTaggedTypeGeneratedHeaderData m_taggedTypeGeneratedBody;
	};

	struct STypeRegClassWritingContext
	{
		const CCodeLines& m_linesRegisterType;
		const Niflect::CString& m_fieldLayoutFuncName;
		CGenLog* m_log;
	};

	struct STypeRegInvokeRegisterTypeWritingData
	{
		CCodeLines& m_linesInvokeRegisterType;
		const Niflect::CString*& m_taggedTypeHeaderFilePathAddr;
	};

	struct STypeRegClassGenHWritingContext
	{
		const CModuleRegInfoValidated& m_moduleRegInfo;
		CGenLog* m_log;
	};

#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
	struct SGetterSetterData
	{
		CCodeLines m_linesGetter;
		//std::string m_getterName;
		CCodeLines m_linesSetter;
		//std::string m_setterName;
	};
#endif

#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
	struct STypeRegBuildTypeMetaFuncWritingInput
	{
		const CModuleRegInfoValidated& m_moduleRegInfo;
		CGenLog* m_log;
	};

	struct STypeRegBuildTypeMetaFuncWritingOutput
	{
		CCodeLines& m_linesDecl;
		CCodeLines& m_linesImpl;
		CDependencyHeaderFilePathAddrs& m_dependencyHeaderFilePathAddrs;
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
		Niflect::TArrayNif<SGetterSetterData>& m_vecGetSetData;
#endif
	};
#else
	struct STypeRegCreateTypeAccessorWritingContext
	{
		const CModuleRegInfoValidated& m_moduleRegInfo;
		CGenLog* m_log;
	};

	struct STypeRegCreateTypeAccessorWritingData
	{
		CCodeLines& m_linesCreateTypeAccessorDecl;
		CCodeLines& m_linesCreateTypeAccessorImpl;
		CDependencyHeaderFilePathAddrs& m_dependencyHeaderFilePathAddrs;
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
		Niflect::TArrayNif<SGetterSetterData>& m_vecGetSetData;
#endif
	};
#endif

	struct STypeRegRegisterTypeContext
	{
		const CModuleRegInfoValidated& m_moduleRegInfo;
		CGenLog* m_log;
	};

	struct SCollectingGeneratedBodyWritingData
	{
	public:
		uint32& m_generatedBodyLineNumber;
	};

	struct SResocursorNodeBodyCodeWritingContext
	{
		const CModuleRegInfoValidated& m_moduleRegInfo;
		CGenLog* m_log;
	};

	struct SGetterSetterWritingData
	{
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
		Niflect::TArrayNif<SGetterSetterData>& m_vecGetSetData;
#endif
		CCodeLines m_linesResoBodyCode;
	};

	class CTypeRegCodeWriter2
	{
	public:
		CTypeRegCodeWriter2();
		void Init(const CResolvedData* resolvedData, const CResolvedCursorNode* bindingTypeIndexedRoot);
		virtual void WriteInvokeRegisterType(const STypeRegRegisterTypeContext& context, STypeRegInvokeRegisterTypeWritingData& data) const;
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		void WriteBuildTypeMetaFunc(const STypeRegBuildTypeMetaFuncWritingInput& input, STypeRegBuildTypeMetaFuncWritingOutput& output) const;
#else
		void WriteWriteCreateTypeAccessorFunc(const STypeRegCreateTypeAccessorWritingContext& context, STypeRegCreateTypeAccessorWritingData& data) const;
#endif
		virtual void Deprecated_WriteTypeRegClass(const STypeRegClassWritingContext& context, CTypeRegClassWritingData2& data) const {}
		virtual void WriteInvokeInitType(const STypeRegClassWritingContext& context, CTypeRegTaggedTypeInitWritingData2& data) const {}
		void WriteGeneratedBody(const STypeRegClassGenHWritingContext& context, CTypeRegTaggedTypeGeneratedHeaderData& data) const;

	protected:
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		virtual void WriteResocursorElementsBodyCode(const SResocursorNodeBodyCodeWritingContext& context, SGetterSetterWritingData& data) const {}
		virtual void WriteResocursorChildrenBodyCode(const SResocursorNodeBodyCodeWritingContext& context, SGetterSetterWritingData& data) const {}
#else
		virtual void WriteResocursorNodeBodyCode(const SResocursorNodeBodyCodeWritingContext& context, SGetterSetterWritingData& data) const {}
#endif
		virtual void CollectDependencyHeaderFilePathAddrs(CDependencyHeaderFilePathAddrs& dependencyHeaderFilePathAddrs) const = 0;
		virtual void CollectDataForGenH(SCollectingGeneratedBodyWritingData& data) const {}

	private:
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		void WriteAddFields(const STypeRegBuildTypeMetaFuncWritingInput& input, CCodeLines& linesBody, CDependencyHeaderFilePathAddrs& dependencyHeaderFilePathAddrs
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
			, Niflect::TArrayNif<SGetterSetterData>& vecGetSetData
#endif
		) const;
#else
		void WriteCreateTypeAccessor(const STypeRegCreateTypeAccessorWritingContext& context, CCodeLines& dataDecl, CCodeLines& dataImpl, CDependencyHeaderFilePathAddrs& dependencyHeaderFilePathAddrs
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
			, Niflect::TArrayNif<SGetterSetterData>& vecGetSetData
#endif
		) const;
#endif

	public:
		static bool CompareLess(const CTypeRegCodeWriter2& a, const CTypeRegCodeWriter2& b);

	protected:
		const CResolvedCursorNode* m_bindingTypeIndexedRoot;
		const CResolvedData* m_resolvedData;
	};
	using CSharedTypeRegCodeWriter2 = Niflect::TSharedPtr<CTypeRegCodeWriter2>;
}