#pragma once
#include "NiflectGen/Log/Log.h"
#include "NiflectGen/CodeWriter/CodeWriter.h"
#include "NiflectGen/CodeWriter/CodeRequirement.h"

namespace NiflectGen
{
	class CWritingContext
	{
	public:
		CWritingContext(CGenLog* log)
			: m_log(log)
		{
		}

	public:
		CGenLog* m_log;
	};

	class CTypeRegInvokationData
	{
	public:
		CTypeRegInvokationData()
			: m_requiredInvokationInitType(false)
		{

		}

		bool m_requiredInvokationInitType;
	};

	struct STypeRegGenHeaderDeclarationWritingData
	{
		CCodeLines m_linesTypeDeclaration;
		CCodeLines m_linesStaticGetTypeSpecialization;
		CCodeLines m_linesExposedPrivateMacro;
	};

	class CTypeRegWritingData
	{
	public:
		CCodeLines deprected_m_linesRegDecl;
		CCodeLines deprected_m_linesRegImpl;
		CCodeLines m_linesRegClass;
		CTypeRegInvokationData m_invokationRequirement;
		Niflect::CString m_regClassName;
		STypeRegGenHeaderDeclarationWritingData m_genHeaderDeclData;
		CNoDupPathCollector m_privateHeaderincludePath;
		Niflect::TArrayNif<Niflect::CString> m_vecNamespaceRequirement;
	};
}