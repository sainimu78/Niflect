#include "NiflectGen/Generator/Generator.h"
#include "NiflectGen/Util/ParserUtil.h"
#include "clang-c/Index.h"
#include "NiflectGen/Generator/CursorNode.h"
#include <iostream>
#include "NiflectGen/Util/CursorUtil.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "Niflect/Util/StringUtil.h"
#include "NiflectGen/Collector/Collector.h"
#include "NiflectGen/Log/Log.h"
#include "Niflect/Util/DebugUtil.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "NiflectGen/CodeWriter/ModuleReg/ModuleRegCodeWriter.h"
#include "NiflectGen/Resolver/Resolver.h"
#include "Niflect/Util/SystemUtil.h"
#include "NiflectGen/Generator/SourceInMemory.h"

//#include <fstream>//std::getline
//#include <stack>

namespace NiflectGen
{
    using namespace Niflect;

    Niflect::TSharedPtr<CGenerator> CreateGenerator()
    {
        return MakeShared<CGenerator>();
    }

    //void CModuleReg::Validate()
    //{
    //    m_moduleRegPath = "";
    //}

    CGenerator::CGenerator()
        : m_collectorHolder(MakeShared<CDataCollector>())
        , m_collector(*m_collectorHolder)
    {
    }
    void CGenerator::AddTypeBindingSettingHeader(const Niflect::CString& filePath)
    {
        ASSERT(NiflectGenDefinition::FileExt::IsH(filePath));
        m_vecTypeBindingSettingFilePath.push_back(filePath);
    }
    static void AddSourceFile(const CString& filePath, TArrayNif<CString>& vecCpp, TArrayNif<CString>& vecH)
    {
        if (NiflectGenDefinition::FileExt::IsCpp(filePath))
        {
            ASSERT(false);//todo: δʵ��.cpp�ļ���ʹ�÷�����
            vecCpp.push_back(filePath);
        }
        else
        {
            vecH.push_back(filePath);
        }
    }
    void CGenerator::AddFileForSearching(const CString& filePath)
    {
        AddSourceFile(filePath, m_vecFileForSearchingCpp, m_vecFileForSearchingH);
    }
    void CGenerator::AddIncludePath(const Niflect::CString& dirPath)
    {
        m_vecHeaderSearchPath.push_back(dirPath);
    }
    bool CGenerator::InitModuleRegInfo(const CModuleRegInfo& userProvied)
    {
        return m_moduleRegInfo.Init(userProvied);
    }
    void CGenerator::Generate(CCodeGenData& genData, TestInterfaceFunc TestFunc)
    {
        //#1, Cleanup & Prepare
        //todo: ·����Ч�Լ��, �ظ�·���޳���
        const auto& userProvided = m_moduleRegInfo.m_userProvided;

        //Ԥ������module, ��module���Ŀ¼�еĻ���ɻ�ȡ���ɵ������ļ�

        CMemSourceReferenceCache memSrcCache;
        auto& memSrcMain = memSrcCache.CreateTempMemSource();
        {
            auto& memSrc = memSrcMain;
            memSrc.m_filePath = "memSrcMain.cpp";
            CSimpleCppWriter writer(memSrc.m_data);
            writer.AddHeaderFirstLine();
            writer.AddInclude(NiflectGenDefinition::NiflectFramework::FilePath::NiflectMacroHeader);//_gen.hδ����ʱ, �ɽ������
            for (auto& it : userProvided.m_vecResolverCustomizationHeader)
                writer.AddInclude(it);
            if (!userProvided.m_moduleApiMacroHeader.empty())
                writer.AddInclude(userProvided.m_moduleApiMacroHeader);
            for (auto& it1 : userProvided.m_vecAccessorSettingHeader)
                writer.AddInclude(it1);
            for (auto& it1 : userProvided.m_vecModuleHeader2)
                writer.AddInclude(it1);
            memSrcCache.AddMemSourceRef(memSrc);
        }

        CParserOption opt;
        opt.InitDefault();
        opt.AddIncludePaths(m_moduleRegInfo.m_vecParsingHeaderSearchPath);

        const bool displayDiagnostics = true;
        auto index = clang_createIndex(true, displayDiagnostics);

        auto translation_unit = clang_parseTranslationUnit(index, memSrcMain.m_filePath.c_str(), opt.GetArgV(),
            opt.GetArgC(), memSrcCache.m_vecHandle.data(),
            static_cast<uint32>(memSrcCache.m_vecHandle.size()), CXTranslationUnit_DetailedPreprocessingRecord | CXTranslationUnit_SkipFunctionBodies
        );

        if (false)//if (true)//
        {
            auto cursor = clang_getTranslationUnitCursor(translation_unit);
#pragma warning( disable : 4996 )
            FILE* fp = fopen("E:/a.txt", "w");
#pragma warning( default : 4996 )
            int level = 0;
            CVisitCallbacks callbacks;
            callbacks.m_EnterFunc = [&fp, &level](const CXCursor& cursor)
                {
                    DebugPrintCursor(fp, cursor, level);
                    level++;
                };
            callbacks.m_LeaveFunc = [&level](const CXCursor& cursor)
                {
                    level--;
                };
            VisitCursorRecurs(cursor, callbacks);
            fclose(fp);
            printf("");
        }

        if (true)//if (false)//
        {
            auto cursor = clang_getTranslationUnitCursor(translation_unit);

            if (TestFunc == NULL)
            {
                CTaggedNode2 taggedRoot;
#pragma warning( disable : 4996 )
                FILE* fp = NULL;
                if (false)
                {
                    Niflect::CString absFilePath;
                    ASSERT(false);//NiflectUtil::ResolvePath("../../../../../../../Generated/NiflectGenerated/b.txt", absFilePath);
                    NiflectUtil::MakeDirectories(absFilePath);
                    fp = fopen(absFilePath.c_str(), "w");
                }
#pragma warning( default : 4996 )
                CGenLog log;
                CCollectingContext context(&log);
                CVisitingDebugData debugData;
                debugData.Init(cursor, fp);
                context.m_debugData = &debugData;
                CCollectionData collectionData;
                m_collector.Collect(cursor, &taggedRoot, context, collectionData);
                if (true)
                {
                    CResolvingContext resolvingContext(&log);
                    CResolver resolver(collectionData, m_moduleRegInfo);
                    CResolvedData resolvedData;
                    resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);

                    //#3, Generate code
                    CTemplateBasedCppWriter writer(resolvedData, m_moduleRegInfo);
                    CWritingContext writingContext(&log);
                    writer.Write3(writingContext, genData);
                }
                debugData.Check();
                //m_collector.DebugFinish2(&taggedRoot, collectionData);
                if (fp != NULL)
                    fclose(fp);
            }
            else
            {
                TestFunc(&cursor);
            }
        }

        if (translation_unit)
            clang_disposeTranslationUnit(translation_unit);

        if (index)
            clang_disposeIndex(index);
    }
    //void CGenerator::Save() const
    //{
    //    const auto& outputRootPath = m_moduleRegInfo.m_outputRootPath;
    //    {
    //        {
    //            for (auto& it0 : m_genData.m_vecTypeRegGenData)
    //            {
    //                CCppWriter writer;
    //                writer.WriteLines(it0.m_privateH);
    //                auto filePath = NiflectUtil::ConcatPath(outputRootPath, it0.m_privateHIncludePath);
    //                NiflectUtil::MakeDirectories(filePath);
    //                NiflectUtil::WriteStringToFile(writer.m_code, filePath);
    //            }
    //            //NiflectUtil::WriteStringToFile(writer.m_code, "E:/NiflectGenTool_Test_TypeRegsPrivateH.cpp");
    //        }
    //        {
    //            //todo: δʵ�� GenH ��������, ����ģ��ʵ����������������, ���ɵĴ���ģ������Ӧ����, �� TestRegistration.cpp �е� CMyRegClass
    //            //  ��֧�� GenH ��ζ�ű�ǵ� Field ֻ���� public ��
    //            CCppWriter writer;
    //            for (auto& it0 : m_genData.m_vecTypeRegGenData)
    //            {
    //                writer.WriteLines(it0.m_genH);
    //            }
    //            NiflectUtil::WriteStringToFile(writer.m_code, "E:/NiflectGenTool_Test_TypeRegsGenH.cpp");
    //        }
    //    }
    //    {
    //        {
    //            {
    //                for (auto& it0 : m_genData.m_vecSplittedModuleRegGenData)
    //                {
    //                    auto filePath = NiflectUtil::ConcatPath(outputRootPath, it0.m_headerFilePath);
    //                    {
    //                        CCppWriter writer;
    //                        writer.WriteLines(it0.m_h);
    //                        NiflectUtil::MakeDirectories(filePath);
    //                        NiflectUtil::WriteStringToFile(writer.m_code, filePath);
    //                    }
    //                    filePath = NiflectUtil::ReplaceFilePathExt(filePath, NiflectGenDefinition::FileExt::H, NiflectGenDefinition::FileExt::Cpp);
    //                    {
    //                        CCppWriter writer;
    //                        writer.WriteLines(it0.m_cpp);
    //                        NiflectUtil::MakeDirectories(filePath);
    //                        NiflectUtil::WriteStringToFile(writer.m_code, filePath);
    //                    }
    //                }
    //            }
    //            //NiflectUtil::WriteStringToFile(writer.m_code, "E:/NiflectGenTool_Test_SplittedModuleRegsCpp.cpp");
    //        }
    //        {
    //            CCppWriter writer;
    //            writer.WriteLines(m_genData.m_moduleRegGenData.m_privateH);
    //            //NiflectUtil::WriteStringToFile(writer.m_code, "E:/NiflectGenTool_Test_ModuleRegPrivateH.cpp");
    //            auto filePath = NiflectUtil::ConcatPath(outputRootPath, m_genData.m_moduleRegGenData.m_privateHIncludePath);
    //            NiflectUtil::MakeDirectories(filePath);
    //            NiflectUtil::WriteStringToFile(writer.m_code, filePath);
    //        }
    //    }
    //    printf("");
    //}
    void CGenerator::SaveCodeToFile(const CCodeLines& linesCode, const Niflect::CString& relativeFilePath) const
    {
        const auto* outputDirPath = &m_moduleRegInfo.m_userProvided.m_genOutputDirPath;
        if (!m_moduleRegInfo.m_userProvided.m_genSourceOutputDirPath.empty())
            outputDirPath = &m_moduleRegInfo.m_userProvided.m_genSourceOutputDirPath;
        auto filePath = NiflectUtil::ConcatPath(m_moduleRegInfo.m_genSourceRootParentDir, relativeFilePath);
        filePath = NiflectUtil::ConcatPath(*outputDirPath, filePath);
        CCodeWriter writer;
        writer.WriteLines(linesCode);
        NiflectUtil::MakeDirectories(filePath);
        std::ofstream ofs;
        if (NiflectUtil::OpenFileStream(ofs, filePath))
        {
            // д��BOM��Byte Order Mark��������ȷ��ʾ�ļ���UTF-8����
            unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
            ofs.write(reinterpret_cast<const char*>(bom), sizeof(bom));
            ofs << writer.m_code;
            ofs.close();
        }
        else
        {
            ASSERT(false);
        }
    }
    void CGenerator::SaveFileToGenSource(const CCodeLines& linesCode, const Niflect::CString& relativeFilePath) const
    {
        auto relativeToGenSource = NiflectUtil::ConcatPath(m_moduleRegInfo.m_moduleGenSourceRoot, relativeFilePath);
        this->SaveCodeToFile(linesCode, relativeToGenSource);
    }
    void CGenerator::Save2(const CCodeGenData& genData) const
    {
        for (auto& it0 : genData.m_typeRegCreateTypeAccessorSpecGenData.m_vecCreateTypeAccessorSpecData)
        {
            this->SaveFileToGenSource(it0.m_decl, it0.m_declHeaderFilePath);
            this->SaveFileToGenSource(it0.m_impl, it0.m_implSourceFilePath);
        }
        for (auto& it0 : genData.m_typeRegStaticGetTypeSpecGenData.m_vecStaticGetTypeSpecData)
        {
            if (it0.m_genH.size() > 0)
                this->SaveFileToGenSource(it0.m_genH, it0.m_genHHeaderFilePath);
            if (it0.m_impl.size() > 0)
                this->SaveFileToGenSource(it0.m_impl, it0.m_implSourceFilePath);
        }
        for (auto& it0 : genData.m_vecSplittedModuleRegGenData)
        {
            this->SaveFileToGenSource(it0.m_h, it0.m_headerFilePath);
            this->SaveFileToGenSource(it0.m_cpp, it0.m_sourceFilePath);
        }
        if (genData.m_moduleRegGenData.m_genH.size() > 0)
            this->SaveFileToGenSource(genData.m_moduleRegGenData.m_genH, genData.m_moduleRegGenData.m_genHIncludePath);
        this->SaveFileToGenSource(genData.m_moduleRegGenData.m_privateH, genData.m_moduleRegGenData.m_privateHIncludePath);
        this->SaveFileToGenSource(genData.m_moduleRegisteredTypeHeaderGenData.m_linesHeader, m_moduleRegInfo.m_moduleRegisteredTypeHeaderFilePath);
    }
    void CGenerator::Cleanup() const
    {
        NiflectUtil::DeleteDirectory(m_moduleRegInfo.m_genTimeBasePath);
    }

    CXChildVisitResult visitAST(CXCursor cursor, CXCursor parent, CXClientData data)
    {
        if (clang_getCursorKind(cursor) == CXCursor_MacroExpansion)
        {
            std::cout << "Macro: " << clang_getCString(clang_getCursorSpelling(cursor)) << std::endl;
            CXTranslationUnit unit = clang_Cursor_getTranslationUnit(cursor);
            CXSourceRange range = clang_getCursorExtent(cursor);
            CXToken* tokens = nullptr;
            unsigned int nTokens = 0;
            clang_tokenize(unit, range, &tokens, &nTokens);
            if (nTokens > 0)
            {
                for (unsigned int i = 1; i < nTokens; ++i)
                {
                    CXToken token = tokens[i];
                    CXString spelling = clang_getTokenSpelling(unit, token);
                    std::string param = clang_getCString(spelling);
                    std::cout << "Argument: " << param << std::endl;
                    clang_disposeString(spelling);
                }
            }
            if (tokens)
                clang_disposeTokens(unit, tokens, nTokens);

            //CXSourceRange range = clang_getCursorExtent(cursor);
            //CXString source_text = clang_getRangeText(clang_Cursor_getTranslationUnit(cursor), range);
            //std::string param = clang_getCString(source_text);
            //param = param.substr(param.find_first_of('(') + 1);
            //param = param.substr(0, param.find_last_of(')'));
            //std::cout << "Arguments: " << param << std::endl;
            //clang_disposeString(source_text);
        }
        return CXChildVisit_Continue;
    }
}
