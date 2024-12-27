#pragma once
#include "NiflectGen/CodeWriter/TypeReg/TypeRegCode.h"

namespace NiflectGen
{
	struct STypeRegPrivateHeaderWritingContext
	{
		Niflect::TArrayNif<CTypeRegWritingData*> m_vecWritingData;
	};

	struct STypeRegPrivateHeaderWritingData
	{
		CCodeLines& m_privateH;
	};

	void WriteTypeRegsPrivateHeader(const CWritingContext& context, const STypeRegPrivateHeaderWritingContext& genCtx, STypeRegPrivateHeaderWritingData& data);
}