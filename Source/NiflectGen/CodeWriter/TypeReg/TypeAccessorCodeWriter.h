#pragma once
#include "NiflectGen/CodeWriter/CodeWriter.h"
#include "NiflectGen/Resolver/ResocursorNode.h"

namespace NiflectGen
{
//	class CFieldLayoutBasedCodeWriter : public CTypeRegCodeWriter2
//	{
//		typedef CTypeRegCodeWriter2 inherited;
//	public:
//		CFieldLayoutBasedCodeWriter();
//
//	public:
//		virtual void WriteForFieldLayout(const STypeRegCreateFieldLayoutOfTypeWritingContext& context, STypeRegCreateFieldLayoutOfTypeWritingData& data) const final override;
//
//	protected:
//		virtual void WriteResocursorNodeBodyCode(CCodeLines& linesResoBodyCode) const {}
//		virtual void CollectDependencyHeaderFilePaths(CDependencyHeaderFilePathRefs& dependencyHeaderFilePathRefs) const = 0;
//
//	private:
//		void WriteCreateFieldLayoutOfType(const STypeRegCreateFieldLayoutOfTypeWritingContext& context, CCodeLines& dataDecl, CCodeLines& dataImpl) const;
//	};


	void WriteNextInitChildAccessor2(const Niflect::CString& fieldOwnerResocursorName, bool isGlobalVar, const Niflect::CString& scopeName, const Niflect::CString& funcName, const Niflect::CString& fieldName, const CCodeLines& linesNata, CCodeLines& linesResoBodyCode);
	void WriteNextInitElementAccessor2(const Niflect::CString& funcName, CCodeLines& linesResoBodyCode);
	void WriteInvokeConstructorBody(const Niflect::CString& typeName, const Niflect::CString& typeNameForLastTemplateArg, uint32 methodIdx, const Niflect::TArray<CResolvedCursorNode>& vecArgument, Niflect::CString& typeBodyFuncName, CCodeLines& linesImpl);
	void WriteMethodRegConstructorInfo(const Niflect::CString& typeBodyFuncName, const CCodeLines& linesNata, const Niflect::CString& moduleScopeSymbolPrefix, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, CCodeLines& linesReg);
	void WriteInvokeMethodBody(const Niflect::CString& typeName, uint32 methodIdx, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, const Niflect::CString& methodName, Niflect::CString& typeBodyFuncName, CCodeLines& linesImpl);
	void WriteInvokeStaticMemberFunctionBody(const Niflect::CString& typeName, const Niflect::CString& scopeName, uint32 methodIdx, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, const Niflect::CString& methodName, Niflect::CString& typeBodyFuncName, CCodeLines& linesImpl);
	void WriteMethodRegMethodInfo(const Niflect::CString& typeBodyFuncName, const CCodeLines& linesNata, const Niflect::CString& moduleScopeSymbolPrefix, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, const Niflect::CString& methodName, CCodeLines& linesReg);
	void WriteMethodRegStaticMemberFunctionInfo(const Niflect::CString& typeBodyFuncName, const CCodeLines& linesNata, const Niflect::CString& moduleScopeSymbolPrefix, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, const Niflect::CString& methodName, CCodeLines& linesReg);
}