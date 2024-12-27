#pragma once
#include "NiflectGen/CodeWriter/TypeReg/TypeRegCode.h"

namespace NiflectGen
{
	struct STypeRegGenHeaderWritingContext
	{
		Niflect::TArrayNif<CTypeRegWritingData*> m_vecWritingData;
	};

	struct STypeRegGenHeaderWritingData
	{
		CCodeLines& m_genH;
	};

	void WriteTypeRegsGenHeader(const CWritingContext& context, const STypeRegGenHeaderWritingContext& genCtx, STypeRegGenHeaderWritingData& data);
}