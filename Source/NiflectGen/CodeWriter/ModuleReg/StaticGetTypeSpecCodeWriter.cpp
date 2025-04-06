#include "NiflectGen/CodeWriter/ModuleReg/StaticGetTypeSpecCodeWriter.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/HardCoded/StaticGetTypeTemplate.h"

namespace NiflectGen
{
    static Niflect::CString ConvertHeaderFilePathToUnderscoredStyle(const Niflect::CString& filePath)
    {
        Niflect::CString result;
        for (auto& it : filePath)
        {
            if (std::isalnum(it))
                result += it;
            else
                result += '_';
        }
        return result;
    }
    class CScopedDeclNode
    {
    public:
        Niflect::CString m_scopeName;
        CCodeLines m_lines;
        Niflect::TArrayNif<Niflect::TSharedPtr<CScopedDeclNode> > m_vecSub;
    };
    static void BuildScopedDeclTreeRecurs(CScopedDeclNode& parent, const Niflect::TArrayNif<Niflect::CString>& vecScopeName, const Niflect::CString& typeDeclCode, uint32 frontIdx = 0)
    {
        if (vecScopeName.size() == frontIdx)
        {
            parent.m_lines.push_back(typeDeclCode);
            return;
        }
        auto& scopeName = vecScopeName[frontIdx];
        CScopedDeclNode* next = NULL;
        for (int32 idx = static_cast<int32>(parent.m_vecSub.size()) - 1; idx >= 0; --idx)
        {
            auto& it = parent.m_vecSub[idx];
            if (it->m_scopeName == scopeName)
            {
                next = it.Get();
                break;
            }
        }
        if (next == NULL)
        {
            auto shared = Niflect::MakeShared<CScopedDeclNode>();
            parent.m_vecSub.push_back(shared);
            next = shared.Get();
            next->m_scopeName = scopeName;
        }
        BuildScopedDeclTreeRecurs(*next, vecScopeName, typeDeclCode, frontIdx + 1);
    }
    static void WriteFullScopedTypeDeclCodeRecurs(const CScopedDeclNode& parent, CCodeLines& linesDecls)
    {
        CCodeLines linesNext;
        for (auto& it : parent.m_vecSub)
            WriteFullScopedTypeDeclCodeRecurs(*it, linesNext);
        for (auto& it : parent.m_lines)
            linesNext.push_back(it);
        WriteLinesIntoNamespaceScope(parent.m_scopeName, linesNext, linesDecls);
    }
    static void WriteFullScopedTypeDeclCode(const CScopedDeclNode& parent, CCodeLines& linesDecls)
    {
        for (auto& it : parent.m_vecSub)
            WriteFullScopedTypeDeclCodeRecurs(*it, linesDecls);
        for (auto& it : parent.m_lines)
            linesDecls.push_back(it);
    }
    void WriteSplittedStaticGetTypeSpec(const SSplittedCreateTypeAccessorSpecWritingContext& context, CTypeRegStaticGetTypeSpecData& data)
    {
        auto splitsCount = context.m_vecItem.size();
        data.m_vecStaticGetTypeSpecData.resize(splitsCount);
        for (uint32 idx0 = 0; idx0 < splitsCount; ++idx0)
        {
            auto& it0 = context.m_vecItem[idx0];
            if (it0.m_taggedTypeHeaderFilePathAddr != NULL)
            {
                auto headerFilePath = CIncludesHelper::ConvertToIncludePath(*it0.m_taggedTypeHeaderFilePathAddr, context.m_moduleRegInfo.m_writingHeaderSearchPaths.m_vecForRegularConversion);
                auto relativeTypeRegFilePathNoExt = NiflectUtil::RemoveFileExt(headerFilePath);
                auto& staticGetTypeSpecData = data.m_vecStaticGetTypeSpecData[idx0];
                auto relativeStaticGetTypeGenHFilePath = relativeTypeRegFilePathNoExt + NiflectGenDefinition::FileExt::GenH;
                staticGetTypeSpecData.m_genHHeaderFilePath = NiflectUtil::ConcatPath(context.m_moduleRegInfo.m_genIncludeBasePath, relativeStaticGetTypeGenHFilePath);
                auto internalRelativeTypeRegFilePathNoExt = NiflectUtil::ConcatPath(context.m_moduleRegInfo.m_typeRegBasePath, ConvertToInternalFilePath(relativeTypeRegFilePathNoExt));
                staticGetTypeSpecData.m_implSourceFilePath = internalRelativeTypeRegFilePathNoExt + NiflectGenDefinition::FileNamePostfix::Gen + context.m_moduleRegInfo.GetSourceFileExtForGenFileMode();

                const bool requiredExportedStaticGetType = !context.m_moduleRegInfo.m_moduleApiMacro.empty();
                {
                    CCodeLines linesGenHInclude;
                    CHeaderFilePathDataArray vecHeaderData;
                    if (requiredExportedStaticGetType && (!context.m_moduleRegInfo.m_moduleApiMacroHeaderFilePath.empty()))
                        vecHeaderData.push_back({ context.m_moduleRegInfo.m_moduleApiMacroHeaderFilePath, true });
                    vecHeaderData.push_back({ NiflectGenDefinition::NiflectFramework::FilePath::NiflectTypeHeader, true });
                    CIncludesHelper::ConvertFromHeaderFilePaths(vecHeaderData, context.m_moduleRegInfo.m_writingHeaderSearchPaths, linesGenHInclude);

                    CCodeTemplate tpl1;
                    ReadTemplateFromRawData(tpl1, HardCodedTemplate::GenH);
                    CLabelToCodeMapping map;
                    MapLabelToLines(map, LABEL_0, linesGenHInclude);
                    Niflect::CString genHHeaderMacro = ConvertHeaderFilePathToUnderscoredStyle(relativeStaticGetTypeGenHFilePath);
                    {
                        //_gen.h 改为无 pragma once, 并增加宏 #error 检查, _gen.h 是特殊写法, 
                        // 由于定义 CURRENT_FILE_ID, 因此不能加 pragma once, 否则 CURRENT_FILE_ID 不能在 _gen.h 作为依赖时被包含解析, 导致 CURRENT_FILE_ID 预处理错误.
                        // 现象为 CURRENT_FILE_ID 被解析为展开的 include 顺序中最先出现的 FID 定义.
                        // 如 DerivedObject_gen.h 的 GENERATED_NON_STORAGE_DECLS 被解析为 TestModule1_gen.h 中的 FID_xxx_GENERATED_NON_STORAGE_DECLS
                        MapLabelToText(map, LABEL_11, genHHeaderMacro);
                        MapLabelToText(map, LABEL_12, relativeStaticGetTypeGenHFilePath);
                        MapLabelToText(map, LABEL_13, headerFilePath);
                    }
                    CCodeLines linesTypeDecls;
                    CCodeLines linesStaticGetTypeSpecDecl;
                    //if (requiredExportedStaticGetType)
                    {
                        {
                            CScopedDeclNode root;
                            for (auto& it1 : it0.m_vecTypeRegDataRef)
                                BuildScopedDeclTreeRecurs(root, *it1->m_taggedTypeGeneratedBody.m_vecScopeNameAddr, it1->m_taggedTypeGeneratedBody.m_specTemplateRequiredTypeDecl);
                            WriteFullScopedTypeDeclCode(root, linesTypeDecls);
                            MapLabelToLines(map, LABEL_1, linesTypeDecls);
                        }
                        {
                            CCodeLines lines;
                            for (auto& it1 : it0.m_vecTypeRegDataRef)
                            {
                                for (auto& it2 : it1->m_taggedTypeGeneratedBody.m_linesStaticGetTypeSpecDecl)
                                    lines.push_back(it2);
                            }
                            WriteLinesIntoNamespaceScope(NiflectGenDefinition::NiflectFramework::ScopeName::Niflect, lines, linesStaticGetTypeSpecDecl);
                            MapLabelToLines(map, LABEL_2, linesStaticGetTypeSpecDecl);
                        }
                    }
                    Niflect::CString genHFileId = NiflectGenDefinition::NiflectFramework::LineNumberMacro::FID + genHHeaderMacro;
                    MapLabelToText(map, LABEL_4, genHFileId);

                    CCodeLines linesMacros;
                    {
                        Niflect::TArrayNif<const CTypeLineNumberMacroData*> vecValidLineNumberMacroData;
                        for (uint32 idx1 = 0; idx1 < it0.m_vecTypeRegDataRef.size(); ++idx1)
                        {
                            auto& lineNumberMacroData = it0.m_vecTypeRegDataRef[idx1]->m_taggedTypeGeneratedBody.m_lineNumberMacroData;
                            auto& lineNumber = lineNumberMacroData.m_generatedBodyLineNumber;
                            if (lineNumber != INDEX_NONE)
                                vecValidLineNumberMacroData.push_back(&lineNumberMacroData);
                        }
                        auto cnt1 = vecValidLineNumberMacroData.size();
                        for (uint32 idx1 = 0; idx1 < cnt1; ++idx1)
                        {
                            auto& lineNumberMacroData = *vecValidLineNumberMacroData[idx1];
                            auto& lineNumber = lineNumberMacroData.m_generatedBodyLineNumber;
                            CCodeLines linesItemsDefinition;
                            CCodeLines linesRootBody;
                            auto cnt2 = lineNumberMacroData.m_vecMacroDefinitionData.size();
                            for (uint32 idx2 = 0; idx2 < cnt2; ++idx2)
                            {
                                auto& it2 = lineNumberMacroData.m_vecMacroDefinitionData[idx2];
                                auto macroName = GenerateLineNumberMacroName(genHFileId, lineNumber, it2.m_namePostfix);
                                WriteLineNumberMacroDefinition(macroName, it2.m_linesBody, linesItemsDefinition);
                                if (idx2 != cnt2 - 1)
                                    macroName += '\\';
                                linesRootBody.push_back(macroName);
                            }
                            CCodeLines linesRootDefinition;
                            {
                                auto macroName = GenerateLineNumberMacroName(genHFileId, lineNumber, NIFLECTFRAMEWORK_MACROPOSTFIX_GENERATED_NON_STORAGE_DECLS);
                                WriteLineNumberMacroDefinition(macroName, linesRootBody, linesRootDefinition);
                                if (idx1 != cnt1 - 1)
                                    linesRootDefinition.push_back(EscapeChar::EmptyLine);
                            }
                            {
                                CCodeTemplate tpl1;
                                ReadTemplateFromRawData(tpl1, HardCodedTemplate::LineNumberMacros);
                                CLabelToCodeMapping map;
                                MapLabelToText(map, LABEL_7, NiflectUtil::FormatString("%u", lineNumber));
                                MapLabelToText(map, LABEL_8, headerFilePath);
                                MapLabelToLines(map, LABEL_9, linesItemsDefinition);
                                MapLabelToLines(map, LABEL_10, linesRootDefinition);
                                Niflect::TSet<Niflect::CString> setReplacedLabel;
                                tpl1.ReplaceLabels(map, linesMacros, &setReplacedLabel);
                            }
                        }
                    }
                    MapLabelToLines(map, LABEL_6, linesMacros);

                    Niflect::TSet<Niflect::CString> setReplacedLabel;
                    tpl1.ReplaceLabels(map, staticGetTypeSpecData.m_genH, &setReplacedLabel);
                }
                {
                    CCodeLines linesGenCppInclude;
                    CHeaderFilePathDataArray vecHeaderData;
                    vecHeaderData.push_back(headerFilePath);
                    CIncludesHelper::ConvertFromHeaderFilePaths(vecHeaderData, context.m_moduleRegInfo.m_writingHeaderSearchPaths, linesGenCppInclude);

                    CCodeTemplate tpl1;
                    ReadTemplateFromRawData(tpl1, HardCodedTemplate::GenCpp);
                    CLabelToCodeMapping map;
                    MapLabelToLines(map, LABEL_0, linesGenCppInclude);
                    CCodeLines linesImpl;
                    //if (requiredExportedStaticGetType)
                    {
                        CCodeLines lines;
                        for (auto& it1 : it0.m_vecTypeRegDataRef)
                        {
                            for (auto& it2 : it1->m_taggedTypeGeneratedBody.m_linesStaticGetTypeSpecImpl)
                                lines.push_back(it2);
                        }
                        WriteLinesIntoNamespaceScope(NiflectGenDefinition::NiflectFramework::ScopeName::Niflect, lines, linesImpl);
                    }
                    MapLabelToLines(map, LABEL_3, linesImpl);
                    Niflect::TSet<Niflect::CString> setReplacedLabel;
                    tpl1.ReplaceLabels(map, staticGetTypeSpecData.m_impl, &setReplacedLabel);
                }
            }
        }
    }
}