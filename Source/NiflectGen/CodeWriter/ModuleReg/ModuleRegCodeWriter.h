#pragma once
#include "NiflectGen/Resolver/Resolver.h"
#include "NiflectGen/CodeWriter/CodeTemplate.h"
#include "NiflectGen/CodeWriter/TypeReg/TypeRegCode.h"
#include "NiflectGen/CodeWriter/ModuleReg/SplittedModuleRegCode.h"
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCode.h"
#include "NiflectGen/Generator/GenData.h"

namespace NiflectGen
{
	struct SModuleRegWritingContext2
	{
		const CModuleRegInfoValidated& m_moduleRegInfo;
		const Niflect::TArrayNif<CSplittedModuleRegInfo>& m_vecItem;
		const Niflect::TArrayNif<CSplittedModuleRegFunctionsName>& m_vecSplittedModuleRegFuncsName;
		const Niflect::TArrayNif<CSplittedModuleRegGenData>& m_vecSplittedModuleRegGenData;
		CGenLog* m_log;
	};
	struct SModuleRegWritingData2
	{
		CModuleRegGenData& m_moduleRegGenData;
	};
	struct SSLKDJFSLKJFContext
	{
		const CModuleRegInfoValidated& m_moduleRegInfo;
		CGenLog* m_log;
	};

	class CTemplateBasedCppWriter
	{
	public:
		CTemplateBasedCppWriter(const CResolvedData& resolvedData, const CModuleRegInfoValidated& moduleRegInfo);

	public:
		//void Write();
		//void Deprecated_Write2(const CWritingContext& context, CCodeGenData& data);
		void Write3(const CWritingContext& context, CCodeGenData& data);

	private:
		void WriteTypeRegs(const CWritingContext& context, Niflect::TArrayNif<CTypeRegWritingData>& vecTypeRegData);
		//void WriteModuleRegs(const CWritingContext& context, const Niflect::TArrayNif<CTypeRegWritingData>& vecTypeRegData, Niflect::TArrayNif<CModuleRegWritingData>& vecModuleRegData);
		//void CreateWriters(const CWritingContext& context, Niflect::TArrayNif<CTypeRegGenFileInfo>& vecTypeRegGenFileInfo);
		void CreateWriters3(const CWritingContext& context, Niflect::TArrayNif<CTypeRegGenFileInfo>& vecTypeRegGenFileInfo);
		void WriteTypeRegs3(const SSLKDJFSLKJFContext& context, Niflect::TArrayNif<CTypeRegWritingData2>& vecTypeRegData);
		void WriteModuleReg(const SModuleRegWritingContext2& context, SModuleRegWritingData2& data) const;
		void WriteVerificationCode();

	private:
		const CResolvedData& m_resolvedData;
		const CModuleRegInfoValidated& m_moduleRegInfo;
		Niflect::TArrayNif<Niflect::TSharedPtr<CTypeRegCodeWriter> > m_vecWriter;
		SModuleRegAndTypeRegMapping m_mapping;
		Niflect::TArrayNif<CSharedTypeRegCodeWriter2> m_vecWriter2;
	};
}