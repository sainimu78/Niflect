#pragma once
#include "NiflectGen/Generator/SourceInMemory.h"
#include "NiflectGen/Log/Log.h"

namespace NiflectGen
{
	struct SGenTimeNiflectMacroHeaderWritingContext
	{
		const Niflect::TArray<Niflect::CString>& m_vecToolHeaderSearchPath;
		const Niflect::CString& m_genTimeBasePath;
		CGenLog* m_log;
	};
	void WriteGenTimeNiflectMacroHeader(const SGenTimeNiflectMacroHeaderWritingContext& context);
}