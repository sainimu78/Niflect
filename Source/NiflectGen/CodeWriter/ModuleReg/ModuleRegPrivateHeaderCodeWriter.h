#pragma once
#include "NiflectGen/CodeWriter/TypeReg/TypeRegCode.h"
#include "NiflectGen/CodeWriter/ModuleReg/SplittedModuleRegCode.h"

namespace NiflectGen
{
    struct SModuleRegWritingContext
    {
        const Niflect::CString& m_moduleName;
        const Niflect::TArrayNif<CSplittedModuleRegInvokationData>& m_vecSplittedModuleRegInvokationData;
        const Niflect::TArrayNif<CSplittedModuleRegGenData>& m_vecSplittedModuleRegData;
    };

    struct SModuleRegWritingData
    {
        CCodeLines& m_privateH;
    };

    void WriteModuleRegPrivateHeader(const CWritingContext& context, const SModuleRegWritingContext& writingCtx, SModuleRegWritingData& writingData);
}