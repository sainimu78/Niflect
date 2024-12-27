#include "NiflectGen/CodeWriter/ModuleReg/SplittedModuleRegCodeWriter2.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/HardCoded/SplittedModuleRegTemplate.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/CodeWriter/ModuleReg/CreateTypeAccessorCodeWriter.h"
#include "NiflectGen/CodeWriter/ModuleReg/StaticGetTypeSpecCodeWriter.h"

namespace NiflectGen
{
    void WriteSplittedSpec(const SSplittedCreateTypeAccessorSpecWritingContext& context, CTypeRegCreateTypeAccessorSpecData& createTypeAccessorSpecGendata, CTypeRegStaticGetTypeSpecData& staticGetTypeSpecGenData)
    {
        WriteSplittedStaticGetTypeSpec(context, staticGetTypeSpecGenData);
        WriteSplittedCreateTypeAccessorSpec(context, createTypeAccessorSpecGendata);
    }
    void WriteSplittedModuleRegs3(const SSplittedModuleRegWritingContext2& context, SSplittedModuleRegWritingData2& data)
    {
        auto splittedModuleRegDirPath = NiflectUtil::ConcatPath(context.m_moduleRegInfo.m_moduleRegBasePath, NiflectGenDefinition::DirName::Splitted);
        auto splitsCount = context.m_vecItem.size();
        data.m_vecSplittedModuleRegGenData.resize(splitsCount);
        data.m_vecSplittedModuleRegFuncsName.resize(splitsCount);
        for (uint32 idx0 = 0; idx0 < splitsCount; ++idx0)
        {
            auto& it0 = context.m_vecItem[idx0];

            {
                auto& splittedRegData = data.m_vecSplittedModuleRegGenData[idx0];

                {
                    auto filePathNoExt = NiflectUtil::FormatString("%s/_%s_%u", splittedModuleRegDirPath.c_str(), context.m_moduleRegInfo.m_userProvided.m_moduleName.c_str(), idx0);
                    splittedRegData.m_headerFilePath = filePathNoExt + NiflectGenDefinition::FileExt::H;
                    splittedRegData.m_sourceFilePath = filePathNoExt + context.m_moduleRegInfo.GetSourceFileExtForGenFileMode();
                }

                CCodeLines linesInvokeInitTypes;
                for (auto& it1 : it0.m_vecTypeRegDataRef)
                {
                    if (it1->m_taggedTypeInit.m_lines.size() > 0)
                    {
                        CCodeTemplate tpl1;
                        ReadTemplateFromRawData(tpl1, HardCodedTemplate::ImplScope);
                        CLabelToCodeMapping map;
                        MapLabelToLines(map, LABEL_3, it1->m_taggedTypeInit.m_lines);
                        Niflect::TSet<Niflect::CString> setReplacedLabel;
                        tpl1.ReplaceLabels(map, linesInvokeInitTypes, &setReplacedLabel);
                    }
                }

                Niflect::CString registerTypesFuncName;
                Niflect::CString initTypesFuncName;
                {
                    CCodeLines linesDecl;
                    auto strIdx = NiflectUtil::FormatString("%u", idx0);
                    {
                        registerTypesFuncName = ReplaceLabelToText2(HardCodedTemplate::RegisterTypesFuncName, LABEL_0, LABEL_1, context.m_moduleRegInfo.m_userProvided.m_moduleName, strIdx);
                        auto funcSignature = ReplaceLabelToText1(HardCodedTemplate::SplittedModuleRegFuncDecl, LABEL_2, registerTypesFuncName);
                        linesDecl.push_back(funcSignature);
                    }
                    if (linesInvokeInitTypes.size() > 0)
                    {
                        initTypesFuncName = ReplaceLabelToText2(HardCodedTemplate::InitTypesFuncName, LABEL_0, LABEL_1, context.m_moduleRegInfo.m_userProvided.m_moduleName, strIdx);
                        auto funcSignature = ReplaceLabelToText1(HardCodedTemplate::InitTypesFuncDecl, LABEL_2, initTypesFuncName);
                        linesDecl.push_back(funcSignature);
                    }
                    {
                        CCodeTemplate tpl1;
                        ReadTemplateFromRawData(tpl1, HardCodedTemplate::SplittedModuleRegH);
                        CLabelToCodeMapping map;
                        CCodeLines linesInclude;
                        {
                            Niflect::TArrayNif<CHeaderFilePathData> vecHeaderData;
                            //vecHeaderData.push_back(NiflectGenDefinition::NiflectFramework::FilePath::NiflectTableHeader);
                            CIncludesHelper::ConvertFromHeaderFilePaths(vecHeaderData, context.m_moduleRegInfo.m_writingHeaderSearchPaths, linesInclude);
                        }
                        MapLabelToLines(map, LABEL_4, linesInclude);
                        MapLabelToLines(map, LABEL_6, linesDecl);
                        Niflect::TSet<Niflect::CString> setReplacedLabel;
                        tpl1.ReplaceLabels(map, splittedRegData.m_h, &setReplacedLabel);
                    }
                }
                {
                    CCodeLines linesImpl;
                    {
                        CCodeTemplate tpl1;
                        ReadTemplateFromRawData(tpl1, HardCodedTemplate::SplittedModuleRegFuncImpl);
                        CLabelToCodeMapping map;
                        MapLabelToText(map, LABEL_2, registerTypesFuncName);
                        CCodeLines linesInvoke;
                        for (auto& it1 : it0.m_vecTypeRegDataRef)
                        {
                            for (auto& it2 : it1->m_registerTypeAndfieldLayout.m_linesInvokeRegisterType)
                                linesInvoke.push_back(it2);
                        }
                        MapLabelToLines(map, LABEL_3, linesInvoke);
                        Niflect::TSet<Niflect::CString> setReplacedLabel;
                        tpl1.ReplaceLabels(map, linesImpl, &setReplacedLabel);
                    }
                    if (linesInvokeInitTypes.size() > 0)
                    {
                        CCodeTemplate tpl1;
                        ReadTemplateFromRawData(tpl1, HardCodedTemplate::InitTypesFuncImpl);
                        CLabelToCodeMapping map;
                        MapLabelToText(map, LABEL_2, initTypesFuncName);
                        MapLabelToLines(map, LABEL_3, linesInvokeInitTypes);
                        Niflect::TSet<Niflect::CString> setReplacedLabel;
                        tpl1.ReplaceLabels(map, linesImpl, &setReplacedLabel);
                    }
                    {
                        CCodeTemplate tpl1;
                        ReadTemplateFromRawData(tpl1, HardCodedTemplate::SplittedModuleRegCpp);
                        CLabelToCodeMapping map;
                        CCodeLines linesInclude;
                        auto& createTypeAccessorSpecData = context.m_typeRegCreateTypeAccessorSpecGenData.m_vecCreateTypeAccessorSpecData[idx0];
                        {
                            Niflect::TArrayNif<CHeaderFilePathData> vecHeaderData;
                            vecHeaderData.push_back({ splittedRegData.m_headerFilePath, true });
                            vecHeaderData.push_back(NiflectGenDefinition::NiflectFramework::FilePath::NiflectTableHeader);
                            if (context.m_moduleRegInfo.m_userProvided.m_genFileMode == EGeneratingHeaderAndSourceFileMode::EHeaderOnly)
                            {
                                auto genCppFilePath = context.m_typeRegStaticGetTypeSpecGenData.m_vecStaticGetTypeSpecData[idx0].m_implSourceFilePath;
                                if (!genCppFilePath.empty())
                                    vecHeaderData.push_back(genCppFilePath);
                            }
                            vecHeaderData.push_back(createTypeAccessorSpecData.m_declHeaderFilePath);
                            if (context.m_moduleRegInfo.m_userProvided.m_genFileMode == EGeneratingHeaderAndSourceFileMode::EHeaderOnly)
                                vecHeaderData.push_back(createTypeAccessorSpecData.m_implSourceFilePath);
                            CIncludesHelper::ConvertFromHeaderFilePaths(vecHeaderData, context.m_moduleRegInfo.m_writingHeaderSearchPaths, linesInclude);
                        }
                        MapLabelToLines(map, LABEL_4, linesInclude);
                        MapLabelToLines(map, LABEL_5, linesImpl);
                        Niflect::TSet<Niflect::CString> setReplacedLabel;
                        tpl1.ReplaceLabels(map, splittedRegData.m_cpp, &setReplacedLabel);
                    }
                }
                auto& funcsName = data.m_vecSplittedModuleRegFuncsName[idx0];
                funcsName.m_registerTypes = registerTypesFuncName;
                if (linesInvokeInitTypes.size() > 0)
                    funcsName.m_initTypes = initTypesFuncName;
            }
        }
    }
}