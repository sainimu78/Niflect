#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegPrivateHeaderCodeWriter.h"
#include "NiflectGen/CodeWriter/HardCoded/ModuleRegPrivateHeaderTemplate.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"

namespace NiflectGen
{
    void WriteModuleRegPrivateHeader(const CWritingContext& context, const SModuleRegWritingContext& writingCtx, SModuleRegWritingData& writingData)
    {
        CCodeLines linesMergedInitTables;
        CCodeLines linesMergedRegisterTypes;
        CCodeLines linesMergedInitTypes;
        CCodeLines linesIncludes;
        Niflect::TArrayNif<Niflect::CString> vecIncludePath;
        ASSERT(writingCtx.m_vecSplittedModuleRegInvokationData.size() == writingCtx.m_vecSplittedModuleRegData.size());
        for (uint32 idx0 = 0; idx0 < writingCtx.m_vecSplittedModuleRegInvokationData.size(); ++idx0)
        {
            auto& dataForInvokation = writingCtx.m_vecSplittedModuleRegInvokationData[idx0];
            auto& genData = writingCtx.m_vecSplittedModuleRegData[idx0];
            for (auto& it1 : dataForInvokation.m_codeInitTables)
                linesMergedInitTables.push_back(it1);
            for (auto& it1 : dataForInvokation.m_invokationRegisterTypes)
                linesMergedRegisterTypes.push_back(it1);
            for (auto& it1 : dataForInvokation.m_invokationInitTypes)
                linesMergedInitTypes.push_back(it1);
            vecIncludePath.push_back(genData.m_headerFilePath);
        }
        ASSERT(false);
        //CIncludesHelper::WriteIncludeDirectives(vecIncludePath, linesIncludes);
        {
            CCodeTemplate tpl;
            ReadTemplateFromRawData(tpl, HardCodedTemplate::ModuleRegPrivateH);
            CLabelToCodeMapping map;
            MapLabelToText(map, LABEL_SHARED_0, writingCtx.m_moduleName);
            MapLabelToLines(map, LABEL_2, linesMergedRegisterTypes);
            MapLabelToLines(map, LABEL_3, linesMergedInitTypes);
            MapLabelToLines(map, LABEL_7, linesIncludes);
            tpl.ReplaceLabels(map, writingData.m_privateH);
        }
    }
}