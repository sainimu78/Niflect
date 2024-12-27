#pragma once
#include "NiflectGen/CodeWriter/ModuleReg/SplittedModuleRegCode.h"

namespace NiflectGen
{
    void WriteSplittedSpec(const SSplittedCreateTypeAccessorSpecWritingContext& context, CTypeRegCreateTypeAccessorSpecData& createTypeAccessorSpecGendata, CTypeRegStaticGetTypeSpecData& staticGetTypeSpecGenData);
    void WriteSplittedModuleRegs3(const SSplittedModuleRegWritingContext2& context, SSplittedModuleRegWritingData2& data);
}