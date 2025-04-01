#include "NiflectGen/CodeWriter/ModuleReg/CreateTypeAccessorCodeWriter.h"
#include "NiflectGen/CodeWriter/HardCoded/CreateTypeAccessorTemplate.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"

namespace NiflectGen
{
    void WriteSplittedCreateTypeAccessorSpec(const SSplittedCreateTypeAccessorSpecWritingContext& context, CTypeRegCreateTypeAccessorSpecData& data)
    {
        auto splitsCount = context.m_vecItem.size();
        data.m_vecCreateTypeAccessorSpecData.resize(splitsCount);
        for (uint32 idx0 = 0; idx0 < splitsCount; ++idx0)
        {
            auto& it0 = context.m_vecItem[idx0];
            {
                auto& typeRegData = data.m_vecCreateTypeAccessorSpecData[idx0];
                Niflect::CString relativeTypeRegFilePathNoExt;
                if (it0.m_taggedTypeHeaderFilePathAddr != NULL)
                {
                    relativeTypeRegFilePathNoExt = CIncludesHelper::ConvertToIncludePath(*it0.m_taggedTypeHeaderFilePathAddr, context.m_moduleRegInfo.m_writingHeaderSearchPaths.m_vecForRegularConversion);
                    ASSERT(!relativeTypeRegFilePathNoExt.empty());
                    relativeTypeRegFilePathNoExt = NiflectUtil::RemoveFileExt(relativeTypeRegFilePathNoExt);
                }
                else
                {
                    relativeTypeRegFilePathNoExt = NiflectUtil::FormatString("Misc_%u", idx0);
                }

                //auto fileNameNoExt = NiflectUtil::GetFileName(relativeTypeRegFilePathNoExt);
                //auto internalFileNameNoExt = "_" + fileNameNoExt;
                //Niflect::CString dirPath;
                //NiflectUtil::GetParentDirPathSafe(relativeTypeRegFilePathNoExt, dirPath);
                //auto internalRelativeTypeRegFilePathNoExt = internalFileNameNoExt;
                //if (!dirPath.empty())
                //    internalRelativeTypeRegFilePathNoExt = NiflectUtil::ConcatPath(dirPath, internalFileNameNoExt);

                auto internalRelativeTypeRegFilePathNoExt = ConvertToInternalFilePath(relativeTypeRegFilePathNoExt);

                auto typeRegSplittedFilePathNoExt = NiflectUtil::ConcatPath(context.m_moduleRegInfo.m_typeRegBasePath, internalRelativeTypeRegFilePathNoExt);
                typeRegData.m_implSourceFilePath = typeRegSplittedFilePathNoExt + context.m_moduleRegInfo.GetSourceFileExtForGenFileMode();
                typeRegData.m_declHeaderFilePath = typeRegSplittedFilePathNoExt + NiflectGenDefinition::FileExt::H;

                CCodeLines linesH;
                CCodeLines linesCpp;
                for (auto& it1 : it0.m_vecTypeRegDataRef)
                {
                    for (auto& it2 : it1->m_registerTypeAndfieldLayout.m_linesCreateFieldLayoutOfTypeDecl)
                        linesH.push_back(it2);
                    for (auto& it2 : it1->m_registerTypeAndfieldLayout.m_linesCreateFieldLayoutOfTypeImpl)
                        linesCpp.push_back(it2);
                }
                for (auto& it1 : it0.m_vecTypeRegDataRef)
                {
                    for (auto& it2 : it1->m_registerTypeAndfieldLayout.m_linesBuildTypeMetaFuncDecl)
                        linesH.push_back(it2);
                    for (auto& it2 : it1->m_registerTypeAndfieldLayout.m_linesBuildTypeMetaFuncImpl)
                        linesCpp.push_back(it2);
                }
                {
                    CCodeTemplate tpl1;
                    ReadTemplateFromRawData(tpl1, HardCodedTemplate::FieldLayoutSpecH);
                    CLabelToCodeMapping map;
                    CCodeLines linesHInclude;
                    {
                        Niflect::TArrayNif<CHeaderFilePathData> vecHeaderData;
                        if (it0.m_taggedTypeHeaderFilePathAddr != NULL)
                            vecHeaderData.push_back({ NiflectGenDefinition::NiflectFramework::FilePath::NiflectTypeHeader, true });//含 CreateTypeAccessor 的函数模板声明
                        else
                            vecHeaderData.push_back({ context.m_moduleRegInfo.m_moduleRegisteredTypeHeaderFilePath, true });
                        for (auto& it1 : it0.m_vecTypeRegDataRef)
                        {
                            for (auto& it2 : it1->m_registerTypeAndfieldLayout.m_dependencyHeaderFilePathAddrs.m_vecDecl)
                                vecHeaderData.push_back(*it2);
                        }
                        CIncludesHelper::ConvertFromHeaderFilePaths(vecHeaderData, context.m_moduleRegInfo.m_writingHeaderSearchPaths, linesHInclude);
                    }
                    MapLabelToLines(map, LABEL_4, linesHInclude);
                    MapLabelToLines(map, LABEL_6, linesH);
                    Niflect::TSet<Niflect::CString> setReplacedLabel;
                    tpl1.ReplaceLabels(map, typeRegData.m_decl, &setReplacedLabel);
                }
                {
                    CCodeTemplate tpl1;
                    ReadTemplateFromRawData(tpl1, HardCodedTemplate::FieldLayoutSpecCpp);
                    CLabelToCodeMapping map;

                    CCodeLines linesCppInclude;
                    {
                        Niflect::TArrayNif<CHeaderFilePathData> vecHeaderData;
                        vecHeaderData.push_back({ typeRegData.m_declHeaderFilePath, true });
                        if (it0.m_taggedTypeHeaderFilePathAddr != NULL)
                            vecHeaderData.push_back({ context.m_moduleRegInfo.m_moduleRegisteredTypeHeaderFilePath, true });//仅当所有 field 都无 Misc 类型无须此 include, 因此略冗余, 但此情况罕见, 因此可接受此简化方式
                        for (auto& it1 : it0.m_vecTypeRegDataRef)
                        {
                            for (auto& it2 : it1->m_registerTypeAndfieldLayout.m_dependencyHeaderFilePathAddrs.m_vecImpl)
                                vecHeaderData.push_back(*it2);
                        }
                        CIncludesHelper::ConvertFromHeaderFilePaths(vecHeaderData, context.m_moduleRegInfo.m_writingHeaderSearchPaths, linesCppInclude);
                        //linesCppInclude = linesFilePath;
                    }
                    MapLabelToLines(map, LABEL_4, linesCppInclude);
                    MapLabelToLines(map, LABEL_5, linesCpp);
                    Niflect::TSet<Niflect::CString> setReplacedLabel;
                    tpl1.ReplaceLabels(map, typeRegData.m_impl, &setReplacedLabel);
                }

                //printf("[%u]---------------------------------\n", idx0);
                //DebugPrintCodeLines(typeRegData.m_impl);
            }
        }
    }
}