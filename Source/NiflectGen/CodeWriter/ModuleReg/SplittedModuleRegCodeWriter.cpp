#include "NiflectGen/CodeWriter/ModuleReg/SplittedModuleRegCodeWriter.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/HardCoded/SplittedModuleRegistrationTemplate.h"
#include "Niflect/Util/StringUtil.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"

namespace NiflectGen
{
    void DetermineSplitting(const Niflect::TArrayNif<SSplittedModuleRefInfo>& vecTypeRegRefInfo, Niflect::TArrayNif<CSplittedModuleRegInvokationData>& vecSplittedModuleRegInvokationData, Niflect::TArrayNif<CSplittedModuleRegGenData>& vecSplittedModuleRegData, Niflect::TArrayNif<Niflect::TArrayNif<uint32> >& vecSplittedModuleRegToTypeRegs)
    {
        vecSplittedModuleRegInvokationData.resize(1);
        vecSplittedModuleRegData.resize(1);
        vecSplittedModuleRegToTypeRegs.resize(vecSplittedModuleRegInvokationData.size());
        for (uint32 idx = 0; idx < vecTypeRegRefInfo.size(); ++idx)
        {
            vecSplittedModuleRegToTypeRegs[0].push_back(idx);
        }
    }
	void WriteSplittedModuleRegs(const CWritingContext& context, const CSplittedModuleRegWritingContext& writingCtx, Niflect::TArrayNif<CSplittedModuleRegInvokationData>& vecSplittedModuleRegInvokationData, Niflect::TArrayNif<CSplittedModuleRegGenData>& vecSplittedModuleRegData)
	{
        ASSERT(false);
        Niflect::TArrayNif<Niflect::TArrayNif<uint32> > vecSplittedModuleRegToTypeRegs;
        DetermineSplitting(writingCtx.m_vecTypeRegRefInfo, vecSplittedModuleRegInvokationData, vecSplittedModuleRegData, vecSplittedModuleRegToTypeRegs);
        ASSERT(vecSplittedModuleRegInvokationData.size() == vecSplittedModuleRegData.size());

        for (uint32 idx0 = 0; idx0 < vecSplittedModuleRegInvokationData.size(); ++idx0)
        {
            auto& dataForInvokation = vecSplittedModuleRegInvokationData[idx0];
            auto& data = vecSplittedModuleRegData[idx0];

            auto splittedModuleName = ReplaceLabelToText2(HardCodedTemplate::SplittedModuleName, LABEL_SHARED_0, LABEL_0, writingCtx.m_moduleName, NiflectUtil::FormatString("_%u", idx0));
            {
                CCodeTemplate tpl;
                ReadTemplateFromRawData(tpl, HardCodedTemplate::SplittedModuleRegH);
                CLabelToCodeMapping map;
                MapLabelToText(map, LABEL_1, splittedModuleName);
                tpl.ReplaceLabels(map, data.m_h);
            }
            dataForInvokation.m_invokationRegisterTypes.push_back(ReplaceLabelToText1(HardCodedTemplate::SplittedModuleRegInvokationRegisterTypes, LABEL_1, splittedModuleName));
            dataForInvokation.m_invokationInitTypes.push_back(ReplaceLabelToText1(HardCodedTemplate::SplittedModuleRegInvokationInitTypes, LABEL_1, splittedModuleName));
            auto filePathNoExt = NiflectUtil::FormatString("%s/Splitted/_%s", writingCtx.m_moduleRegBasePath.c_str(), splittedModuleName.c_str());
            data.m_headerFilePath = filePathNoExt + NiflectGenDefinition::FileExt::H;
            data.m_sourceFilePath = filePathNoExt + NiflectGenDefinition::FileExt::Cpp;

            {
                CCodeTemplate tpl;
                ReadTemplateFromRawData(tpl, HardCodedTemplate::ModuleReg_TypeRegInitTable);
                CLabelToCodeMapping map;
                MapLabelToText(map, LABEL_1, splittedModuleName);
                tpl.ReplaceLabels(map, dataForInvokation.m_codeInitTables);
            }

            auto& vecTypeRegIndex = vecSplittedModuleRegToTypeRegs[idx0];
            CCodeLines linesInvokationRegisterType;
            CCodeLines linesInvokationInitType;
            CNoDupPathCollector includesNoDup;
            includesNoDup.Add(data.m_headerFilePath);
            for (uint32 idx1 = 0; idx1 < vecTypeRegIndex.size(); ++idx1)
            {
                auto& refInfo = writingCtx.m_vecTypeRegRefInfo[vecTypeRegIndex[idx1]];
                auto& it0 = *refInfo.m_typeRegData;
                includesNoDup.Add(*refInfo.m_includePathPrivateH);
#ifdef INVOKE_TYPEREG_WITH_NAMESPACE
                auto regNamespace = "reserved_reg_namespace";//todo: ´Óit»ñÈ¡namespace?
                linesInvokationRegisterType.push_back(ReplaceLabelToText2(HardCodedTemplate::InvokationRegisterType, LABEL_6, LABEL_7, regNamespace, it0.m_regClassName));
                if (it0.m_invokationRequirement.m_requiredInvokationInitType)
                    linesInvokationInitType.push_back(ReplaceLabelToText2(HardCodedTemplate::InvokationInitType, LABEL_6, LABEL_7, regNamespace, it0.m_regClassName));
#else
                linesInvokationRegisterType.push_back(ReplaceLabelToText1(HardCodedTemplate::InvokationRegisterType2, LABEL_7, it0.m_regClassName));
                if (it0.m_invokationRequirement.m_requiredInvokationInitType)
                    linesInvokationInitType.push_back(ReplaceLabelToText1(HardCodedTemplate::InvokationInitType2, LABEL_7, it0.m_regClassName));
#endif

                //data.m_linesInvokingRegisterType.push_back(ReplaceLabelToText2(HardCodedTemplate::InvokationRegisterType, LABEL_6, LABEL_7, regNamespace, it0.m_regClassName));
                //if (it0.m_invokationRequirement.m_requiredInvokationInitType)
                //{
                //    data.m_linesInvokingInitType.push_back(ReplaceLabelToText2(HardCodedTemplate::InvokationInitType, LABEL_6, LABEL_7, regNamespace, it0.m_regClassName));
                //}
            }
            CCodeLines linesIncludes;
            ASSERT(false);
            //CIncludesHelper::Write(includesNoDup, linesIncludes);

            CCodeLines linesInvokationRegisterTypeScope;
            {
                CCodeTemplate tpl;
                ReadTemplateFromRawData(tpl, HardCodedTemplate::TypeRegRegsterTypeScope);
                CLabelToCodeMapping map;
                MapLabelToLines(map, LABEL_3, linesInvokationRegisterType);
                tpl.ReplaceLabels(map, linesInvokationRegisterTypeScope);
            }
            {
                CCodeTemplate tpl;
                ReadTemplateFromRawData(tpl, HardCodedTemplate::SplittedModuleRegCpp);
                CLabelToCodeMapping map;
                MapLabelToText(map, LABEL_1, splittedModuleName);
                MapLabelToLines(map, LABEL_2, linesInvokationRegisterTypeScope);
                MapLabelToLines(map, LABEL_4, linesInvokationInitType);
                MapLabelToLines(map, LABEL_5, linesIncludes);
                tpl.ReplaceLabels(map, data.m_cpp);
            }
        }
	}
}