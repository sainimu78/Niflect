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
        : m_log(NULL)
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
            ASSERT(false);//todo: 未实现.cpp文件中使用反射标记
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
    bool CGenerator::Init(const CModuleRegInfo& userProvied, CGenLog* log)
    {
        m_log = log;
        return m_moduleRegInfo.Init(userProvied, m_log);
    }
    void CGenerator::Generate(CCodeGenData& genData, TestInterfaceFunc TestFunc)
    {
        //#1, Cleanup & Prepare
        //todo: 路径有效性检查, 重复路径剔除等
        const auto& userProvided = m_moduleRegInfo.m_userProvided;

        //预留清理module, 从module输出目录中的缓存可获取生成的所有文件

        CMemSourceReferenceCache memSrcCache;
        auto& memSrcMain = memSrcCache.CreateTempMemSource();
        {
            auto& memSrc = memSrcMain;
            memSrc.m_filePath = "memSrcMain.cpp";
            CSimpleCppWriter writer(memSrc.m_data);
            writer.AddHeaderFirstLine();
            writer.AddInclude(m_moduleRegInfo.m_macroTagHeaderFilePath);//使_gen.h未生成时, 可解析标记
            //begin
            // 此文件列表非一个构建目标只需要 1 份 的 PCH, PCH 是编译器生成的二进制文件, 不同编译器的 PCH 格式不同, 正由于此, 解析时只能以 PCH 的所有来源头文件传给 libclang
            // 用于支持目标项目中使用 PCH 的编码方式, 缺陷为无 PCH 的作用, 每次都须解析, 可考虑通过 Cursor 序列化实现一种缓存, 或者解析后转换为 Bypass Header
            for (auto& it1 : userProvided.m_vecModulePrecompileHeader)
                writer.AddInclude(it1);
            //end
            for (auto& it : userProvided.m_vecResolverCustomizationHeader)
                writer.AddInclude(it);
            if (!userProvided.m_specifiedModuleApiMacroHeaderFilePath.empty())//仅用户指定的头文件须加入, 生成的头文件假定不会被直接使用因此无须加入
                writer.AddInclude(userProvided.m_specifiedModuleApiMacroHeaderFilePath);
            for (auto& it1 : userProvided.m_vecAccessorSettingHeader)
                writer.AddInclude(it1);
            for (auto& it1 : userProvided.m_vecModuleHeader2)
                writer.AddInclude(it1);
            memSrcCache.AddMemSourceRef(memSrc);
        }

        CParserOption opt;
        opt.InitDefault();
        opt.AddIncludePaths(m_moduleRegInfo.m_vecParsingHeaderSearchPath);

        const bool displayDiagnostics = false;//为 true 时, clang_parseTranslationUnit 遇到语法, 类型未定义等错误时会直接输出信息, 无关错误导致无法构建. 应对信息分类后再确定是否输出, 计划如下 clang_getDiagnostic 位置以此方法修改
        auto index = clang_createIndex(true, displayDiagnostics);

        auto translation_unit = clang_parseTranslationUnit(index, memSrcMain.m_filePath.c_str(), opt.GetArgV(),
            opt.GetArgC(), memSrcCache.m_vecHandle.data(),
            static_cast<uint32>(memSrcCache.m_vecHandle.size()), CXTranslationUnit_DetailedPreprocessingRecord | CXTranslationUnit_SkipFunctionBodies
        );

        const bool printParsingDiagnostics = false;
        if (printParsingDiagnostics)
        {
            unsigned numDiags = clang_getNumDiagnostics(translation_unit);
            for (unsigned i = 0; i < numDiags; ++i) {
                CXDiagnostic diag = clang_getDiagnostic(translation_unit, i);

                //unsigned diagID = clang_getDiagnosticNumber(diagnostic);

                //// 示例：Clang 中 unknown type 的典型错误号为 3 (扩展编号需参考具体版本)
                //if (diagID == 3 || (diagID >= 1000 && diagID <= 1999)) {
                //    handleClangBuiltinError(diagID);
                //}

                // 获取诊断详细信息
                CXString text = clang_getDiagnosticSpelling(diag);
                CXSourceLocation loc = clang_getDiagnosticLocation(diag);

                // 解析位置信息
                CXString fname;
                unsigned line, column;
                clang_getPresumedLocation(loc, &fname, &line, &column);

                //// 构造错误对象（自定义数据结构）
                //ErrorInfo err;
                //err.message = std::string(clang_getCString(text));
                //err.file = std::string(clang_getCString(fname));
                //err.line = line;
                //err.column = column;

                //// 手动控制是否打印
                //if (shouldPrintError(err)) { // 自定义判断逻辑
                //    std::cerr << "[" << err.file << ":" << line << ":" << column << "] "
                //        << err.message << std::endl;
                //}

                GenLogInfo(m_log, CLogLocationInfo(clang_getCString(fname), line, column), clang_getCString(text));


                // 释放资源
                clang_disposeString(text);
                clang_disposeString(fname);
                clang_disposeDiagnostic(diag);
            }
        }

        if (false)//if (true)//
        {
            auto cursor = clang_getTranslationUnitCursor(translation_unit);
#pragma warning( disable : 4996 )
            FILE* fp = fopen("I:/E/a.txt", "w");
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
                CCollectingContext context(m_log);
                CVisitingDebugData debugData;
                debugData.Init(cursor, fp);
                context.m_debugData = &debugData;
                CCollectionData collectionData;
                CDataCollector collector(m_moduleRegInfo);
                collector.Collect(cursor, &taggedRoot, context, collectionData);
                if (true)
                {
                    CResolvingContext resolvingContext(m_log);
                    CResolver resolver(collectionData, m_moduleRegInfo);
                    CResolvedData resolvedData;
                    resolver.Resolve4(&taggedRoot, resolvingContext, resolvedData);

                    //#3, Generate code
                    CTemplateBasedCppWriter writer(resolvedData, m_moduleRegInfo);
                    CWritingContext writingContext(m_log);
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
    //            //todo: 未实现 GenH 代码生成, 包括模板实例的声明代码生成, 生成的代码模板须相应调整, 见 TestRegistration.cpp 中的 CMyRegClass
    //            //  不支持 GenH 意味着标记的 Field 只能在 public 中
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
    void CGenerator::SaveCodeToFile(const CCodeLines& linesCode, const Niflect::CString& relativeFilePath, CSavingData& saving) const
    {
        const auto* outputDirPath = &m_moduleRegInfo.m_userProvided.m_genOutputDirPath;
        if (!m_moduleRegInfo.m_userProvided.m_genSourceOutputDirPath.empty())
            outputDirPath = &m_moduleRegInfo.m_userProvided.m_genSourceOutputDirPath;
        saving.m_vecFileInfo.push_back(SFilePathAndContent());
        auto& item = saving.m_vecFileInfo.back();
        auto filePath = NiflectUtil::ConcatPath(m_moduleRegInfo.m_genSourceRootParentDir, relativeFilePath);
        item.m_filePath = NiflectUtil::ConcatPath(*outputDirPath, filePath);
        item.m_writer.WriteLines(linesCode);
    }
    void CGenerator::SaveFileToGenSource(const CCodeLines& linesCode, const Niflect::CString& relativeFilePath, CSavingData& saving) const
    {
        auto relativeToGenSource = NiflectUtil::ConcatPath(m_moduleRegInfo.m_moduleGenSourceRoot, relativeFilePath);
        this->SaveCodeToFile(linesCode, relativeToGenSource, saving);
    }
    static bool fileContentEquals(const std::string& filePath, const std::string& newContent) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            return false;
        }

        std::string existingContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return existingContent == newContent;
    }
    const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
    static bool WriteToDisk(const SFilePathAndContent& info)
    {
        NiflectUtil::MakeDirectories(info.m_filePath);
        std::ofstream ofs;
        if (NiflectUtil::OpenFileStream(ofs, info.m_filePath))
        {
            // 写入BOM（Byte Order Mark），以明确表示文件是UTF-8编码
            if (!info.m_writer.m_code.empty())
                ofs.write(reinterpret_cast<const char*>(bom), sizeof(bom));
            ofs << info.m_writer.m_code;
            return true;
        }
        return false;
    }
    void CGenerator::Save2(const CCodeGenData& genData) const
    {
        CSavingData saving;
        for (auto& it0 : genData.m_typeRegCreateTypeAccessorSpecGenData.m_vecCreateTypeAccessorSpecData)
        {
            this->SaveFileToGenSource(it0.m_decl, it0.m_declHeaderFilePath, saving);
            this->SaveFileToGenSource(it0.m_impl, it0.m_implSourceFilePath, saving);
        }
        for (auto& it0 : genData.m_typeRegStaticGetTypeSpecGenData.m_vecStaticGetTypeSpecData)
        {
            if (it0.m_genH.size() > 0)
                this->SaveFileToGenSource(it0.m_genH, it0.m_genHHeaderFilePath, saving);
            if (it0.m_impl.size() > 0)
                this->SaveFileToGenSource(it0.m_impl, it0.m_implSourceFilePath, saving);
        }
        for (auto& it0 : genData.m_vecSplittedModuleRegGenData)
        {
            this->SaveFileToGenSource(it0.m_h, it0.m_headerFilePath, saving);
            this->SaveFileToGenSource(it0.m_cpp, it0.m_sourceFilePath, saving);
        }
        if (genData.m_moduleRegGenData.m_genH.size() > 0)
            this->SaveFileToGenSource(genData.m_moduleRegGenData.m_genH, genData.m_moduleRegGenData.m_genHIncludePath, saving);
        if (genData.m_moduleRegGenData.m_privateH.size() > 0)
        {
            this->SaveFileToGenSource(genData.m_moduleRegGenData.m_privateH, genData.m_moduleRegGenData.m_privateHIncludePath, saving);
            this->SaveFileToGenSource(genData.m_moduleRegisteredTypeHeaderGenData.m_linesHeader, m_moduleRegInfo.m_moduleRegisteredTypeHeaderFilePath, saving);
        }
        if (genData.m_moduleApiMacroHeaderGenData.m_linesHeader.size() > 0)
            this->SaveFileToGenSource(genData.m_moduleApiMacroHeaderGenData.m_linesHeader, genData.m_moduleApiMacroHeaderGenData.m_filePath, saving);

        size_t offset = 0;
        const bool writeEncodingMark = true;
        if (writeEncodingMark)
            offset = sizeof(bom);

        Niflect::TArray<uint32> vecIdxToSave;
        for (uint32 idx0 = 0; idx0 < saving.m_vecFileInfo.size(); ++idx0)
        {
            bool canWrite = false;
            auto& it0 = saving.m_vecFileInfo[idx0];
            std::ifstream ifs;
            if (NiflectUtil::OpenFileStream(ifs, it0.m_filePath))
            {
                ifs.seekg(0, std::ios::end);
                auto fileSize = static_cast<size_t>(ifs.tellg());
                if (fileSize >= offset)
                    fileSize -= offset;
                ifs.seekg(offset, std::ios::beg);
                Niflect::CString fileContent(fileSize, '\0');
                ifs.read(&fileContent[0], fileSize);
                canWrite = fileContent != it0.m_writer.m_code;
            }
            else
            {
                canWrite = true;
            }
            if (canWrite)
                vecIdxToSave.push_back(idx0);
        }

        //当生成内容全须写入时, 清空生成目录以清理残留文件
        if (saving.m_vecFileInfo.size() == vecIdxToSave.size())
            NiflectUtil::DeleteDirectory(m_moduleRegInfo.m_moduleGenDirPath);

        uint32 cnt = static_cast<uint32>(vecIdxToSave.size());
        if (cnt > 0)
            printf("Written %u file%s:\n", cnt, cnt > 1 ? "s" : "");
        for (uint32 idx0 = 0; idx0 < cnt; ++idx0)
        {
            auto& savingIdx = vecIdxToSave[idx0];
            auto& it0 = saving.m_vecFileInfo[savingIdx];
            if (WriteToDisk(it0))
                GenLogInfo(m_log, it0.m_filePath.c_str());
        }

        Niflect::CString summary = "Finished";
        if (vecIdxToSave.size() == 0)
            summary = "No changes";
        GenLogInfo(m_log, NiflectUtil::FormatString("NiflectGenTool of %s, %s", m_moduleRegInfo.m_userProvided.m_moduleName.c_str(), summary.c_str()));

        {
            if (!m_moduleRegInfo.m_userProvided.m_genSourceOutputDirPath.empty())
                ASSERT(false);//经检查, 此选项可能废弃

            //此文件为接入 cmake 所依赖的文件, 如无变化不写入则 cmake 无法确认是否工具已正确执行, 导致每次构建都会执行工具
            //接入 cmake 相关的代码见 IntegrateNiflectGenTool.cmake 中 add_custom_command(OUTPUT "${GeneratedModulePrivateH}"
            auto finishedFlagFilePath = NiflectUtil::ConcatPath(m_moduleRegInfo.m_userProvided.m_genOutputDirPath, NiflectGenDefinition::NiflectFramework::FileName::FinishedFlag);
            if (!WriteToDisk(SFilePathAndContent{ finishedFlagFilePath }))
                GenLogError(m_log, NiflectUtil::FormatString("Failed to write %s", saving.m_vecFileInfo.back().m_filePath.c_str()));
        }
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
