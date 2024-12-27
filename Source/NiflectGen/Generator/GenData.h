#pragma once

namespace NiflectGen
{
	class CSplittedModuleRegGenData
	{
	public:
		CCodeLines m_h;
		CCodeLines m_cpp;
		Niflect::CString m_headerFilePath;
		Niflect::CString m_sourceFilePath;
	};

	class CTypeRegGenData
	{
	public:
		CCodeLines m_genH;
		Niflect::CString m_genHIncludePath;
		CCodeLines m_privateH;
		Niflect::CString m_privateHIncludePath;
	};

	class CCreateTypeAccessorSpecData
	{
	public:
		CCodeLines m_decl;
		Niflect::CString m_declHeaderFilePath;
		CCodeLines m_impl;
		Niflect::CString m_implSourceFilePath;
	};

	class CTypeRegCreateTypeAccessorSpecData
	{
	public:
		Niflect::TArrayNif<CCreateTypeAccessorSpecData> m_vecCreateTypeAccessorSpecData;
	};

	class CStaticGetTypeSpecData
	{
	public:
		CCodeLines m_genH;
		Niflect::CString m_genHHeaderFilePath;
		CCodeLines m_impl;
		Niflect::CString m_implSourceFilePath;
	};

	class CTypeRegStaticGetTypeSpecData
	{
	public:
		Niflect::TArrayNif<CStaticGetTypeSpecData> m_vecStaticGetTypeSpecData;
	};

	class CModuleRegGenData
	{
	public:
		Niflect::CString m_genHIncludePath;
		CCodeLines m_genH;
		Niflect::CString m_privateHIncludePath;
		CCodeLines m_privateH;
	};

	class SModuleRegisteredTypeHeaderGenData
	{
	public:
		CCodeLines m_linesHeader;
	};

	class CCodeGenData
	{
	public:
		CModuleRegGenData m_moduleRegGenData;
		Niflect::TArrayNif<CSplittedModuleRegGenData> m_vecSplittedModuleRegGenData;
		//Niflect::TArrayNif<CSplittedGenData> m_vecSplittedTypeRegGenData;
		CTypeRegCreateTypeAccessorSpecData m_typeRegCreateTypeAccessorSpecGenData;
		CTypeRegStaticGetTypeSpecData m_typeRegStaticGetTypeSpecGenData;
		SModuleRegisteredTypeHeaderGenData m_moduleRegisteredTypeHeaderGenData;
	};
}