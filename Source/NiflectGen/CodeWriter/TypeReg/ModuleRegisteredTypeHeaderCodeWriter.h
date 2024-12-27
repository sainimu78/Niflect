#pragma once
#include "NiflectGen/CodeWriter/CodeWriter.h"
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCode.h"
#include "NiflectGen/Log/Log.h"
#include "NiflectGen/Generator/GenData.h"

namespace NiflectGen
{
	class SModuleRegisteredTypeHeaderWritingContext
	{
	public:
		const CModuleRegInfoValidated& m_moduleRegInfo;
		CGenLog* m_log;
	};

	void WriteModuleRegisteredTypeHeaderCodeWriter(const SModuleRegisteredTypeHeaderWritingContext& context, SModuleRegisteredTypeHeaderGenData& data);
}