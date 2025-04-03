#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCodeWriter.h"
#include "NiflectGen/Util/CursorUtil.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/CodeWriter/TypeReg/InheritableTypeRegCodeWriter.h"
#include "NiflectGen/CodeWriter/ModuleReg/SplittedModuleRegCodeWriter.h"
#include <algorithm>
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/CodeWriter/TypeReg/TypeRegPrivateHeaderCodeWriter.h"
#include "NiflectGen/CodeWriter/TypeReg/TypeRegGenHeaderCodeWriter.h"
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegPrivateHeaderCodeWriter.h"
#include "NiflectGen/CodeWriter/TypeReg/MiscTypeRegCodeWriter.h"
#include "NiflectGen/CodeWriter/ModuleReg/SplittedModuleRegCodeWriter2.h"
#include "NiflectGen/CodeWriter/HardCoded/ModuleRegTemplate.h"
#include "NiflectGen/CodeWriter/TypeReg/ModuleRegisteredTypeHeaderCodeWriter.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/ModuleApiMacroHeaderWriter.h"

#include "Niflect/Util/SystemUtil.h"//临时写文件测试用

namespace NiflectGen
{
    class CScopedTypeCode
    {
    public:
        Niflect::TArrayNif<Niflect::CString> m_vecDeclScope;
        Niflect::TArrayNif<Niflect::CString> m_vecNamespace;
    };
    CTemplateBasedCppWriter::CTemplateBasedCppWriter(const CResolvedData& resolvedData, const CModuleRegInfoValidated& moduleRegInfo)
        : m_resolvedData(resolvedData)
        , m_moduleRegInfo(moduleRegInfo)
    {
        //printf("%s\n", HardCodedTemplate::InheritableTypeReg_ClassImpl);
        //printf("");
    }
    //void CTemplateBasedCppWriter::CreateWriters(const CWritingContext& context, Niflect::TArrayNif<CTypeRegGenFileInfo>& vecTypeRegGenFileInfo)
    //{
    //    ASSERT(m_vecWriter.size() == 0);

    //    {
    //        auto& userProvided = m_moduleRegInfo.m_userProvided;
    //        for (auto& it1 : userProvided.m_vecOriginalHeader)
    //        {
    //            auto ret = m_mapping.m_mapOriginalFilePathToModuleRegIndicesAndIncPath.insert({ it1, SModuleRegIndicesAndIncludePath()});
    //            auto& item = ret.first->second;
    //            if (ret.second)
    //            {
    //                auto incPath = CIncludesHelper::ConvertToIncludePath(it1, userProvided.m_vecWritingHeaderSearchPath);
    //                item.m_includePath_reserved = incPath;
    //                auto orgIncPathPrivateH = NiflectUtil::ReplaceFilePathExt(incPath, NiflectGenDefinition::FileExt::H, NiflectGenDefinition::FileExt::PrivateH);
    //                auto genIncPathPrivateH = NiflectUtil::ConcatPath(m_moduleRegInfo.m_typeRegBasePath, orgIncPathPrivateH);
    //                auto orgIncPathGenH = NiflectUtil::ReplaceFilePathExt(incPath, NiflectGenDefinition::FileExt::H, NiflectGenDefinition::FileExt::GenH);
    //                auto genIncPathGenH = NiflectUtil::ConcatPath(m_moduleRegInfo.m_typeRegBasePath, orgIncPathGenH);
    //                item.m_includePathPrivateHIndex = static_cast<uint32>(vecTypeRegGenFileInfo.size());
    //                vecTypeRegGenFileInfo.push_back(CTypeRegGenFileInfo(genIncPathPrivateH, genIncPathGenH));
    //            }
    //            else
    //            {
    //                ASSERT(false);//todo: 一次调用只能输出一个ModuleReg, 计划废弃ModuleReg数组, 如果将ModuleReg混在一起处理可能导致一些Module依赖配错却能输出正确结果
    //            }
    //        }
    //    }

    //    auto& accessorBindingMapping = m_resolvedData.deprecated_m_mapping.m_accessorBindingMapping;
    //    for (auto& it0 : accessorBindingMapping.m_vecAccessorBinding2)
    //    {
    //        if (it0.m_accessorData.m_isNotATemplate)
    //        {
    //            auto filePath = GetCursorFilePath(it0.m_accessorSubcursor.m_cursorDecl);
    //            auto itFound = m_mapping.m_mapOriginalFilePathToModuleRegIndicesAndIncPath.find(filePath);
    //            if (itFound != m_mapping.m_mapOriginalFilePathToModuleRegIndicesAndIncPath.end())
    //            {
    //                auto& item = itFound->second;
    //                uint32 writerIndex = static_cast<uint32>(m_vecWriter.size());
    //                m_mapping.m_vecTypeRegIndices.push_back(writerIndex);
    //                auto& privateHeaderData = vecTypeRegGenFileInfo[item.m_includePathPrivateHIndex];
    //                privateHeaderData.m_vecTypeRegDataIndex.push_back(writerIndex);
    //                ASSERT(it0.Is1D());//不支持模板, 因此只能为1D, 对应的Binding类型可能为builtin, 类型Decl或别名
    //                CAccessorBindingMapping2 placeholder0;
    //                CTaggedTypesMapping placeholder1;
    //                CUntaggedTemplatesMapping placeholder2;
    //                SResolvedMappings placeholderMappings{ placeholder0, placeholder1, placeholder2 };
    //                CResolvingDependenciesContext placeholderCtx(placeholderMappings, NULL);
    //                STypeRegClassWritingSetting setting = { m_moduleRegInfo.m_userProvided.m_vecHeaderSearchPath, m_resolvedData.deprecated_m_mapping, placeholderCtx };
    //                m_vecWriter.push_back(Niflect::MakeShared<CInheritableTypeRegCodeWriter_FieldAccessor>(it0.m_accessorSubcursor.m_cursorDecl, setting, it0.m_actualFieldDeclCursor, it0.m_vecWWWW[0].m_subcursor));
    //                m_mapping.m_vecTypeRegIncludePathPrivateHRef.push_back(&privateHeaderData.m_prevateHIncludePath);
    //            }
    //        }
    //    }

    //    for (auto& it0 : m_resolvedData.m_vecResolvedTypes2)
    //    {
    //        auto& cursor = it0.m_taggedType->GetCursor();
    //        auto filePath = GetCursorFilePath(cursor);
    //        //在此处挑选实际需要生成的类型是为避免在Resolve或之前的流程中可能出现的大量路径查找, 在此处虽查找量未减少, 但可通过如并行实现一定优化
    //        auto itFound = m_mapping.m_mapOriginalFilePathToModuleRegIndicesAndIncPath.find(filePath);
    //        if (itFound != m_mapping.m_mapOriginalFilePathToModuleRegIndicesAndIncPath.end())
    //        {
    //            auto& item = itFound->second;
    //            uint32 writerIndex = static_cast<uint32>(m_vecWriter.size());
    //            m_mapping.m_vecTypeRegIndices.push_back(writerIndex);
    //            auto& privateHeaderData = vecTypeRegGenFileInfo[item.m_includePathPrivateHIndex];
    //            privateHeaderData.m_vecTypeRegDataIndex.push_back(writerIndex);
    //            CAccessorBindingMapping2 placeholder0;
    //            CTaggedTypesMapping placeholder1;
    //            CUntaggedTemplatesMapping placeholder2;
    //            SResolvedMappings placeholderMappings{ placeholder0, placeholder1, placeholder2 };
    //            CResolvingDependenciesContext placeholderCtx(placeholderMappings, NULL);
    //            STypeRegClassWritingSetting setting = { m_moduleRegInfo.m_userProvided.m_vecHeaderSearchPath, m_resolvedData.deprecated_m_mapping, placeholderCtx };
    //            m_vecWriter.push_back(it0.m_taggedType->CreateCodeWriter(setting));
    //            m_mapping.m_vecTypeRegIncludePathPrivateHRef.push_back(&privateHeaderData.m_prevateHIncludePath);
    //        }
    //    }
    //}
    void CTemplateBasedCppWriter::CreateWriters3(const CWritingContext& context, Niflect::TArrayNif<CTypeRegGenFileInfo>& vecTypeRegGenFileInfo)
    {
        for (auto& it0 : m_resolvedData.m_signatureMapping.m_vecItem)
        {
            CSharedTypeRegCodeWriter2 writer;
            if (it0.m_resoRoot.m_accessorBindingIndex != INDEX_NONE)
            {
                writer = Niflect::MakeShared<CMiscTypeRegCodeWriter>();
            }
            else if (it0.m_resoRoot.m_taggedTypeIndex != INDEX_NONE)
            {
                auto& taggedType = m_resolvedData.m_taggedMapping.m_vecType[it0.m_resoRoot.m_taggedTypeIndex];
                if (!taggedType->m_isImportType)
                    writer = taggedType->CreateCodeWriter2();
            }
            else
            {
                ASSERT(!it0.m_resoRoot.m_resocursorName.empty());
            }
            if (writer != NULL)
            {
                writer->Init(&m_resolvedData, &it0.m_resoRoot);
                m_vecWriter2.push_back(writer);
            }
        }
        std::sort(m_vecWriter2.begin(), m_vecWriter2.end(), [](const CSharedTypeRegCodeWriter2& a, const CSharedTypeRegCodeWriter2& b)
            {
                return CTypeRegCodeWriter2::CompareLess(*a, *b);
            });
    }
    void CTemplateBasedCppWriter::WriteTypeRegs3(const SSLKDJFSLKJFContext& context, Niflect::TArrayNif<CTypeRegWritingData2>& vecTypeRegData)
    {
        vecTypeRegData.resize(m_vecWriter2.size());
        for (uint32 idx0 = 0; idx0 < m_vecWriter2.size(); ++idx0)
        {
            auto& data = vecTypeRegData[idx0];
            auto& it0 = m_vecWriter2[idx0];

            STypeRegRegisterTypeContext invokeRegisterTypeCtx{ context.m_moduleRegInfo, context.m_log };
            STypeRegInvokeRegisterTypeWritingData invokeRegisterTypeData{
                data.m_registerTypeAndfieldLayout.m_linesInvokeRegisterType, 
                data.m_registerTypeAndfieldLayout.m_taggedTypeHeaderFilePathAddr
            };
            it0->WriteInvokeRegisterType(invokeRegisterTypeCtx, invokeRegisterTypeData);

#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING

            STypeRegBuildTypeMetaFuncWritingInput btmWritinginput{ context.m_moduleRegInfo, context.m_log };
            STypeRegBuildTypeMetaFuncWritingOutput btmWritingOutput{data.m_registerTypeAndfieldLayout.m_linesBuildTypeMetaFuncDecl
                , data.m_registerTypeAndfieldLayout.m_linesBuildTypeMetaFuncImpl
                , data.m_registerTypeAndfieldLayout.m_dependencyHeaderFilePathAddrs
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
                , vecGetSetData
#endif
            };
            it0->WriteBuildTypeMetaFunc(btmWritinginput, btmWritingOutput);
#else
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
            Niflect::TArrayNif<SGetterSetterData> vecGetSetData;
#endif
            STypeRegCreateTypeAccessorWritingContext createTypeAccessorCtx{ context.m_moduleRegInfo, context.m_log };
            STypeRegCreateTypeAccessorWritingData createTypeAccessorData{data.m_registerTypeAndfieldLayout.m_linesCreateFieldLayoutOfTypeDecl
                , data.m_registerTypeAndfieldLayout.m_linesCreateFieldLayoutOfTypeImpl
                , data.m_registerTypeAndfieldLayout.m_dependencyHeaderFilePathAddrs
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
                , vecGetSetData
#endif
            };
            it0->WriteWriteCreateTypeAccessorFunc(createTypeAccessorCtx, createTypeAccessorData);
#endif

            {
                STypeRegClassGenHWritingContext regClassCtx{ context.m_moduleRegInfo, context.m_log };
                it0->WriteGeneratedBody(regClassCtx, data.m_taggedTypeGeneratedBody);
            }
            {
                STypeRegClassWritingContext regClassCtx{ data.m_registerTypeAndfieldLayout.m_linesInvokeRegisterType, data.m_registerTypeAndfieldLayout.m_fieldLayoutFuncName, context.m_log };
                it0->WriteInvokeInitType(regClassCtx, data.m_taggedTypeInit);
            }

            //DebugPrintCodeLines(data.m_registerTypeAndfieldLayout.m_linesRegisterType);
            //DebugPrintCodeLines(data.m_registerTypeAndfieldLayout.m_linesFieldLayoutImpl);
            //DebugPrintCodeLines(data.m_regClass.m_linesImpl);
            //printf("--------------------\n");
        }
    }

    //struct SSplittedModuleRegWritingContext2
    //{
    //    const Niflect::CString& m_moduleName;
    //    const Niflect::CString& m_moduleRegBasePath;
    //    CGenLog* m_log;
    //};
    //class CSplittedModuleRegWritingData2
    //{

    //};
    //void CSplittedModuleRegWriter::Write(const SSplittedModuleRegWritingContext2& context, CSplittedModuleRegWritingData2& data) const
    //{

    //}
    struct SModuleSplittingContext
    {
        const CModuleRegInfoValidated& m_moduleRegInfo;
        const Niflect::TArrayNif<CTypeRegWritingData2>& m_vecTypeRegData;
        CGenLog* m_log;
    };
    static void SplitModuleReg(const SModuleSplittingContext& context, Niflect::TArrayNif<CSplittedModuleRegInfo>& vecSplittedModuleRegInfo)
    {
        Niflect::TMap<Niflect::CString, uint32> mapHeaderFilePathToSmIndex;
        Niflect::TArrayNif<bool> vecTaken;
        vecTaken.resize(context.m_vecTypeRegData.size());
        for (uint32 idx0 = 0; idx0 < context.m_vecTypeRegData.size(); ++idx0)
        {
            auto& it0 = context.m_vecTypeRegData[idx0];
            if (auto filePathAddr = it0.m_registerTypeAndfieldLayout.m_taggedTypeHeaderFilePathAddr)
            {
                auto ret = mapHeaderFilePathToSmIndex.insert({ *filePathAddr, static_cast<uint32>(vecSplittedModuleRegInfo.size()) });
                if (ret.second)
                    vecSplittedModuleRegInfo.push_back(CSplittedModuleRegInfo());
                auto& smr = vecSplittedModuleRegInfo[ret.first->second];
                smr.m_taggedTypeHeaderFilePathAddr = filePathAddr;
                smr.m_vecTypeRegDataRef.push_back(&it0);
                vecTaken[idx0] = true;
            }
        }

        CSplittedModuleRegInfo smr;
        uint32 splitLinesCounter = 0;
        for (uint32 idx0 = 0; idx0 < context.m_vecTypeRegData.size(); ++idx0)
        {
            if (!vecTaken[idx0])
            {
                auto& it0 = context.m_vecTypeRegData[idx0];
                smr.m_vecTypeRegDataRef.push_back(&it0);
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
                splitLinesCounter += static_cast<uint32>(it0.m_registerTypeAndfieldLayout.m_linesBuildTypeMetaFuncImpl.size());
#else
                splitLinesCounter += static_cast<uint32>(it0.m_registerTypeAndfieldLayout.m_linesCreateFieldLayoutOfTypeImpl.size());
#endif
                if (splitLinesCounter > NiflectGenDefinition::NiflectFramework::Setting::ThresholdLinesCountForRegCodeSplitting)
                {
                    vecSplittedModuleRegInfo.push_back(smr);
                    smr.m_vecTypeRegDataRef.clear();
                    splitLinesCounter = 0;
                }
            }
        }
        if (smr.m_vecTypeRegDataRef.size() > 0)
            vecSplittedModuleRegInfo.push_back(smr);
    }
    void CTemplateBasedCppWriter::Write3(const CWritingContext& context, CCodeGenData& data)
    {
        Niflect::TArrayNif<CTypeRegGenFileInfo> vecTypeRegGenFileInfo;
        this->CreateWriters3(context, vecTypeRegGenFileInfo);

        Niflect::TArrayNif<CTypeRegWritingData2> vecTypeRegData;
        SSLKDJFSLKJFContext myTypeRegCtx{ m_moduleRegInfo, context.m_log };
        this->WriteTypeRegs3(myTypeRegCtx, vecTypeRegData);

        //CSplittedTypeRegWritingContext splittedModTypesCtx(m_moduleRegInfo, context.m_log);
        //CSplittedModuleReg2 smr;
        //uint32 splitLinesCounter = 0;
        //for (uint32 idx0 = 0; idx0 < vecTypeRegData.size(); ++idx0)
        //{
        //    auto& it0 = vecTypeRegData[idx0];
        //    smr.m_vecTypeRegDataRef.push_back(&it0);
        //    splitLinesCounter += static_cast<uint32>(it0.m_registerTypeAndfieldLayout.m_linesFieldLayoutImpl.size());
        //    if (splitLinesCounter > NiflectGenDefinition::NiflectFramework::Setting::ThresholdLinesCountForModuleRegSplitting)
        //    {
        //        splittedModTypesCtx.m_vecItem.push_back(smr);
        //        smr.m_vecTypeRegDataRef.clear();
        //        splitLinesCounter = 0;
        //    }
        //}
        //if (smr.m_vecTypeRegDataRef.size() > 0)
        //    splittedModTypesCtx.m_vecItem.push_back(smr);



        //Niflect::TArrayNif<CSplittedModuleRegInfo> vecSplittedModuleRegInfo;
        //SSplittedTypeRegWritingContext splittedModTypesCtx{ m_moduleRegInfo, vecSplittedModuleRegInfo, context.m_log };
        //Niflect::TMap<Niflect::CString, uint32> mapHeaderFilePathToSmIndex;
        //Niflect::TArrayNif<bool> vecTaken;
        //vecTaken.resize(vecTypeRegData.size());
        //for (uint32 idx0 = 0; idx0 < vecTypeRegData.size(); ++idx0)
        //{
        //    auto& it0 = vecTypeRegData[idx0];
        //    if (auto filePathRef = it0.m_registerTypeAndfieldLayout.m_taggedTypeHeaderFilePathRef)
        //    {
        //        auto ret = mapHeaderFilePathToSmIndex.insert({ *filePathRef, static_cast<uint32>(splittedModTypesCtx.m_vecItem.size())});
        //        if (ret.second)
        //            vecSplittedModuleRegInfo.push_back(CSplittedModuleRegInfo());
        //        auto& smr = vecSplittedModuleRegInfo[ret.first->second];
        //        smr.m_taggedTypeHeaderFilePathRef = filePathRef;
        //        smr.m_vecTypeRegDataRef.push_back(&it0);
        //        vecTaken[idx0] = true;
        //    }
        //}

        //CSplittedModuleRegInfo smr;
        //uint32 splitLinesCounter = 0;
        //for (uint32 idx0 = 0; idx0 < vecTypeRegData.size(); ++idx0)
        //{
        //    if (!vecTaken[idx0])
        //    {
        //        auto& it0 = vecTypeRegData[idx0];
        //        smr.m_vecTypeRegDataRef.push_back(&it0);
        //        splitLinesCounter += static_cast<uint32>(it0.m_registerTypeAndfieldLayout.m_linesFieldLayoutImpl.size());
        //        if (splitLinesCounter > NiflectGenDefinition::NiflectFramework::Setting::ThresholdLinesCountForModuleRegSplitting)
        //        {
        //            vecSplittedModuleRegInfo.push_back(smr);
        //            smr.m_vecTypeRegDataRef.clear();
        //            splitLinesCounter = 0;
        //        }
        //    }
        //}
        //if (smr.m_vecTypeRegDataRef.size() > 0)
        //    vecSplittedModuleRegInfo.push_back(smr);


        SModuleSplittingContext moduleSplittingCtx{ m_moduleRegInfo, vecTypeRegData, context.m_log };
        Niflect::TArrayNif<CSplittedModuleRegInfo> vecSplittedModuleRegInfo;
        SplitModuleReg(moduleSplittingCtx, vecSplittedModuleRegInfo);

        SSplittedCreateTypeAccessorSpecWritingContext splittedModTypesCtx{ m_moduleRegInfo, vecSplittedModuleRegInfo, context.m_log };
        WriteSplittedSpec(splittedModTypesCtx, data.m_typeRegCreateTypeAccessorSpecGenData, data.m_typeRegStaticGetTypeSpecGenData);

        SSplittedModuleRegWritingContext2 splittedModulesCtx{ m_moduleRegInfo, splittedModTypesCtx.m_vecItem, data.m_typeRegCreateTypeAccessorSpecGenData, data.m_typeRegStaticGetTypeSpecGenData, context.m_log };
        Niflect::TArrayNif<CSplittedModuleRegFunctionsName> vecSplittedModuleRegFuncsName;
        SSplittedModuleRegWritingData2 splittedModuleRegsData{ data.m_vecSplittedModuleRegGenData, vecSplittedModuleRegFuncsName };
        WriteSplittedModuleRegs3(splittedModulesCtx, splittedModuleRegsData);

        SModuleRegWritingContext2 moduleRegCtx{ m_moduleRegInfo, splittedModulesCtx.m_vecItem, vecSplittedModuleRegFuncsName, data.m_vecSplittedModuleRegGenData, context.m_log };
        SModuleRegWritingData2 moduleRegData{ data.m_moduleRegGenData };
        this->WriteModuleReg(moduleRegCtx, moduleRegData);

        SModuleRegisteredTypeHeaderWritingContext moduleRegisteredTypeHeaderCtx{ m_moduleRegInfo, context.m_log };
        WriteModuleRegisteredTypeHeaderCodeWriter(moduleRegisteredTypeHeaderCtx, data.m_moduleRegisteredTypeHeaderGenData);

        if (m_moduleRegInfo.m_userProvided.m_exportedStaticGetTypeFunctions)
        {
            WriteModuleApiMacroHeader(m_moduleRegInfo.m_userProvided.m_moduleName, m_moduleRegInfo.m_moduleApiMacro, data.m_moduleApiMacroHeaderGenData.m_linesHeader);
            data.m_moduleApiMacroHeaderGenData.m_filePath = NiflectUtil::ConcatPath(NiflectGenDefinition::DirName::GenInclude, GetApiMacroHeaderFileName(m_moduleRegInfo.m_userProvided.m_moduleName));
        }

        this->WriteVerificationCode();
    }
    void CTemplateBasedCppWriter::WriteModuleReg(const SModuleRegWritingContext2& context, SModuleRegWritingData2& data) const
    {
        auto moduleGenPrivateFilePath = NiflectUtil::FormatString("%s%s", m_moduleRegInfo.m_userProvided.m_moduleName.c_str(), NiflectGenDefinition::FileExt::PrivateH);
        data.m_moduleRegGenData.m_privateHIncludePath = NiflectUtil::ConcatPath(m_moduleRegInfo.m_genSrcBasePath, moduleGenPrivateFilePath);
        auto splitsCount = context.m_vecItem.size();
        if (splitsCount > 0)
        {
            Niflect::CString moduleGenHFilePath;
            const bool toGenStaticModuleRegHeader = true;
            if (context.m_moduleRegInfo.m_userProvided.m_toGenStaticModuleReg)
            {
                ASSERT(!context.m_moduleRegInfo.m_userProvided.m_toGenStaticallyRegisterToRegistry);
                if (toGenStaticModuleRegHeader)
                {
                    moduleGenHFilePath = NiflectUtil::FormatString("%s%s", m_moduleRegInfo.m_userProvided.m_moduleName.c_str(), NiflectGenDefinition::FileExt::GenH);
                    data.m_moduleRegGenData.m_genHIncludePath = NiflectUtil::ConcatPath(m_moduleRegInfo.m_genIncludeBasePath, moduleGenHFilePath);
                }
            }
            {
                CCodeTemplate tpl1;
                ReadTemplateFromRawData(tpl1, HardCodedTemplate::ModuleRegImpl);
                CLabelToCodeMapping map;
                CCodeLines linesInclude;
                {
                    Niflect::TArrayNif<CHeaderFilePathData> vecHeaderData;
                    for (auto& it1 : context.m_vecSplittedModuleRegGenData)
                    {
                        vecHeaderData.push_back(it1.m_headerFilePath);
                        if (context.m_moduleRegInfo.m_userProvided.m_genFileMode == EGeneratingHeaderAndSourceFileMode::EHeaderOnly)
                            vecHeaderData.push_back(it1.m_sourceFilePath);
                    }
                    if (context.m_moduleRegInfo.m_userProvided.m_toGenStaticModuleReg)
                    {
                        if (toGenStaticModuleRegHeader)
                            vecHeaderData.push_back(moduleGenHFilePath);
                        else
                            vecHeaderData.push_back(NiflectGenDefinition::NiflectFramework::FilePath::NiflectModuleInfoHeader);
                    }
                    if (context.m_moduleRegInfo.m_userProvided.m_toGenStaticallyRegisterToRegistry)
                    {
                        vecHeaderData.push_back(NiflectGenDefinition::NiflectFramework::FilePath::NiflectModuleRegistryHeader);
                    }
                    CIncludesHelper::ConvertFromHeaderFilePaths(vecHeaderData, m_moduleRegInfo.m_writingHeaderSearchPaths, linesInclude);
                    MapLabelToLines(map, LABEL_0, linesInclude);
                }
                CCodeLines linesInvokeRegisterTypes;
                CCodeLines linesInvokeTaggedTypeInit;
                {
                    for (auto& it1 : context.m_vecSplittedModuleRegFuncsName)
                    {
                        linesInvokeRegisterTypes.push_back(it1.m_registerTypes + "(table);");
                        if (!it1.m_initTypes.empty())
                            linesInvokeTaggedTypeInit.push_back(it1.m_initTypes + "();");
                    }
                    MapLabelToLines(map, LABEL_1, linesInvokeRegisterTypes);
                }
                MapLabelToLines(map, LABEL_2, linesInvokeTaggedTypeInit);
                //CCodeLines linesStaticModuleReg;
                //if (context.m_moduleRegInfo.m_userProvided.m_toGenStaticModuleReg)
                //{
                //    ReplaceLabelToLines1(HardCodedTemplate::StaticModuleReg, LABEL_4, context.m_moduleRegInfo.m_userProvided.m_moduleName, linesStaticModuleReg);
                //    MapLabelToLines(map, LABEL_3, linesStaticModuleReg);
                //}
                Niflect::TSet<Niflect::CString> setReplacedLabel;
                tpl1.ReplaceLabels(map, data.m_moduleRegGenData.m_privateH, &setReplacedLabel);
            }
            if (context.m_moduleRegInfo.m_userProvided.m_toGenStaticModuleReg)
            {
                if (toGenStaticModuleRegHeader)
                {
                    CCodeTemplate tpl1;
                    ReadTemplateFromRawData(tpl1, HardCodedTemplate::StaticModuleRegHeader);
                    CLabelToCodeMapping map;
                    CCodeLines linesInclude;
                    {
                        Niflect::TArrayNif<CHeaderFilePathData> vecHeaderData;
                        if (!context.m_moduleRegInfo.m_moduleApiMacroHeaderFilePath.empty())
                            vecHeaderData.push_back(context.m_moduleRegInfo.m_moduleApiMacroHeaderFilePath);//可能使用 PCH, 包含在其中, 因此也可不指定
                        vecHeaderData.push_back(NiflectGenDefinition::NiflectFramework::FilePath::NiflectModuleInfoHeader);
                        CIncludesHelper::ConvertFromHeaderFilePaths(vecHeaderData, m_moduleRegInfo.m_writingHeaderSearchPaths, linesInclude);
                        MapLabelToLines(map, LABEL_0, linesInclude);
                    }
                    MapLabelToText(map, LABEL_4, context.m_moduleRegInfo.m_userProvided.m_moduleName);
                    if (context.m_moduleRegInfo.m_moduleApiMacro.empty())
                        GenLogError(context.m_log, "The API macro is not specified");
                    MapLabelToText(map, LABEL_5, context.m_moduleRegInfo.m_moduleApiMacro);
                    MapLabelToText(map, LABEL_6, NiflectGenDefinition::NiflectFramework::FuncNamePrefix::GeneratedGetModuleInfo);
                    Niflect::TSet<Niflect::CString> setReplacedLabel;
                    tpl1.ReplaceLabels(map, data.m_moduleRegGenData.m_genH, &setReplacedLabel);
                }
                {
                    CCodeLines linesStaticModuleReg;
                    CCodeTemplate tpl1;
                    ReadTemplateFromRawData(tpl1, HardCodedTemplate::StaticModuleRegImpl);
                    CLabelToCodeMapping map;
                    MapLabelToText(map, LABEL_4, context.m_moduleRegInfo.m_userProvided.m_moduleName);
                    if (context.m_moduleRegInfo.m_moduleApiMacro.empty())
                        GenLogError(context.m_log, "The API macro is not specified");
                    MapLabelToText(map, LABEL_5, context.m_moduleRegInfo.m_moduleApiMacro);
                    Niflect::TSet<Niflect::CString> setReplacedLabel;
                    tpl1.ReplaceLabels(map, linesStaticModuleReg, &setReplacedLabel);
                    for (auto& it : linesStaticModuleReg)
                        data.m_moduleRegGenData.m_privateH.push_back(it);
                }
                {
                    CCodeTemplate tpl1;
                    ReadTemplateFromRawData(tpl1, HardCodedTemplate::StaticModuleRegGetModuleC);
                    CLabelToCodeMapping map;
                    MapLabelToText(map, LABEL_4, context.m_moduleRegInfo.m_userProvided.m_moduleName);
                    MapLabelToText(map, LABEL_6, NiflectGenDefinition::NiflectFramework::FuncNamePrefix::GeneratedGetModuleInfo);
                    Niflect::TSet<Niflect::CString> setReplacedLabel;
                    tpl1.ReplaceLabels(map, data.m_moduleRegGenData.m_privateH, &setReplacedLabel);
                }
            }
            if (context.m_moduleRegInfo.m_userProvided.m_toGenStaticallyRegisterToRegistry)
            {
                CCodeLines linesStaticModuleReg;
                CCodeTemplate tpl1;
                ReadTemplateFromRawData(tpl1, HardCodedTemplate::StaticModuleRegImpl2);
                CLabelToCodeMapping map;
                MapLabelToText(map, LABEL_4, context.m_moduleRegInfo.m_userProvided.m_moduleName);
                Niflect::TSet<Niflect::CString> setReplacedLabel;
                tpl1.ReplaceLabels(map, linesStaticModuleReg, &setReplacedLabel);
                for (auto& it : linesStaticModuleReg)
                    data.m_moduleRegGenData.m_privateH.push_back(it);
            }
        }
    }
    void CTemplateBasedCppWriter::WriteVerificationCode()
    {
        //printf("#todo: Code for verification of binding types duplication (auto a = typeid(TA).hash_code();).\n");
    }
    //void CTemplateBasedCppWriter::Deprecated_Write2(const CWritingContext& context, CCodeGenData& data)
    //{
    //    Niflect::TArrayNif<CTypeRegGenFileInfo> vecTypeRegGenFileInfo;
    //    this->CreateWriters(context, vecTypeRegGenFileInfo);

    //    Niflect::TArrayNif<CTypeRegWritingData> vecTypeRegData;
    //    this->WriteTypeRegs(context, vecTypeRegData);

    //    {
    //        auto& userProvided = m_moduleRegInfo.m_userProvided;
    //        Niflect::TArrayNif<SSplittedModuleRefInfo> vecRefInfo;
    //        for (auto& it1 : m_mapping.m_vecTypeRegIndices)
    //        {
    //            SSplittedModuleRefInfo ref{&vecTypeRegData[it1], m_mapping.m_vecTypeRegIncludePathPrivateHRef[it1]};
    //            vecRefInfo.push_back(ref);
    //        }
    //        CSplittedModuleRegWritingContext spliitedModuleRegWritingCtx = { userProvided.m_moduleName, m_moduleRegInfo.m_moduleRegBasePath, vecRefInfo };
    //        Niflect::TArrayNif<CSplittedModuleRegInvokationData> vecSplittedModuleRegInvokationData;
    //        WriteSplittedModuleRegs(context, spliitedModuleRegWritingCtx, vecSplittedModuleRegInvokationData, data.m_vecSplittedModuleRegGenData);

    //        SModuleRegWritingContext moduleRegWritngCtx = { userProvided.m_moduleName, vecSplittedModuleRegInvokationData, data.m_vecSplittedModuleRegGenData };
    //        auto genIncPathPrivateH = NiflectUtil::ConcatPath(m_moduleRegInfo.m_moduleRegBasePath, userProvided.m_moduleName) + NiflectGenDefinition::FileExt::PrivateH;
    //        data.m_moduleRegGenData.m_privateHIncludePath = genIncPathPrivateH;
    //        SModuleRegWritingData moduleRegWrtingData = { data.m_moduleRegGenData.m_privateH };
    //        WriteModuleRegPrivateHeader(context, moduleRegWritngCtx, moduleRegWrtingData);
    //    }

    //    data.m_vecTypeRegGenData.resize(vecTypeRegGenFileInfo.size());
    //    for (uint32 idx0 = 0; idx0 < vecTypeRegGenFileInfo.size(); ++idx0)
    //    {
    //        STypeRegPrivateHeaderWritingContext writingCtx;
    //        for (auto& it1 : vecTypeRegGenFileInfo[idx0].m_vecTypeRegDataIndex)
    //            writingCtx.m_vecWritingData.push_back(&vecTypeRegData[it1]);
    //        auto& typeRegGenData = data.m_vecTypeRegGenData[idx0];
    //        typeRegGenData.m_privateHIncludePath = vecTypeRegGenFileInfo[idx0].m_prevateHIncludePath;
    //        STypeRegPrivateHeaderWritingData writingData = { typeRegGenData.m_privateH };
    //        WriteTypeRegsPrivateHeader(context, writingCtx, writingData);
    //    }
    //    for (uint32 idx0 = 0; idx0 < vecTypeRegGenFileInfo.size(); ++idx0)
    //    {
    //        STypeRegGenHeaderWritingContext writingCtx;
    //        for (auto& it1 : vecTypeRegGenFileInfo[idx0].m_vecTypeRegDataIndex)
    //            writingCtx.m_vecWritingData.push_back(&vecTypeRegData[it1]);
    //        auto& typeRegGenData = data.m_vecTypeRegGenData[idx0];
    //        typeRegGenData.m_genHIncludePath = vecTypeRegGenFileInfo[idx0].m_genHIncludePath;
    //        STypeRegGenHeaderWritingData writingData = { typeRegGenData.m_genH };
    //        WriteTypeRegsGenHeader(context, writingCtx, writingData);
    //    }
    //}
    void CTemplateBasedCppWriter::WriteTypeRegs(const CWritingContext& context, Niflect::TArrayNif<CTypeRegWritingData>& vecTypeRegData)
    {
        for (auto& it : m_vecWriter)
            it->Deprecated_Init();

        vecTypeRegData.resize(m_vecWriter.size());
        for (uint32 idx0 = 0; idx0 < m_vecWriter.size(); ++idx0)
        {
            auto& it = m_vecWriter[idx0];
            auto& data = vecTypeRegData[idx0];

            it->WritePrivateHeaderIncludePath(context, data.m_privateHeaderincludePath);

            if (false)
            {
                CTypeRegDeclWrittingData dataClassDecl(data.deprected_m_linesRegDecl);
                it->WriteDecl(context, dataClassDecl);
                CCodeLines linesClassImpl;
                CTypeRegImplWrittingData dataClassImpl(data.deprected_m_linesRegImpl);
                it->WriteImpl(context, dataClassImpl);

                DebugPrintCodeLines(dataClassDecl.m_lines);
                DebugPrintCodeLines(dataClassImpl.m_lines);
            }
            if (true)
            {
                CTypeRegClassWrittingData dataClass(data.m_linesRegClass, data.m_privateHeaderincludePath);
                it->WriteClass(context, dataClass);

                DebugPrintCodeLines(dataClass.m_lines);
                printf("");
            }

            data.m_regClassName = it->WriteTypeRegClassName();
            //auto regNamespace = "reserved_reg_namespace";//todo: 从it获取namespace?

            //data.m_linesInvokingRegisterType.push_back(ReplaceLabelToText2(HardCodedTemplate::ModuleReg_InvokationRegisterType, LABEL_5, LABEL_6, regNamespace, regScopeName));

            it->GetInvokationRequirement(data.m_invokationRequirement);
            //if (dataInvocationRequirement.m_requiredInvokationInitType)
            //{
                //data.m_linesInvokingInitType.push_back(ReplaceLabelToText2(HardCodedTemplate::ModuleReg_InvokationInitType, LABEL_5, LABEL_6, regNamespace, regScopeName));
            //}

            it->WriteGenHeaderDeclaration(context, data.m_genHeaderDeclData);

            data.m_vecNamespaceRequirement = it->GetNamespaces();
        }
    }
    //void CTemplateBasedCppWriter::WriteModuleRegs(const CWritingContext& context, const Niflect::TArrayNif<CTypeRegWritingData>& vecTypeRegData, Niflect::TArrayNif<CModuleRegWritingData>& vecModuleRegData)
    //{

    //}
}