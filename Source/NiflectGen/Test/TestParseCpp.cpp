#include "NiflectGen/Test/TestParseCpp.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include <stdio.h>
#include "clang-c/Index.h"
#include <vector>
#include <string>
#include <assert.h>
#include "clang-c/BuildSystem.h"
#include "clang-c/CXCompilationDatabase.h"
#include "clang-c/CXErrorCode.h"
#include "clang-c/CXSourceLocation.h"
#include "clang-c/CXString.h"
#include "clang-c/Documentation.h"
#include "clang-c/Index.h"
//#include "clang/Config/config.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef CLANG_HAVE_LIBXML
#include <libxml/parser.h>
#include <libxml/relaxng.h>
#include <libxml/xmlerror.h>
#endif

#ifdef _WIN32
#  include <direct.h>
#else
#  include <unistd.h>
#include <libgen.h>
#endif
/******************************************************************************/
/* Utility functions.                                                         */
/******************************************************************************/

char* basename(const char* path)
{
    char* base1 = (char*)strrchr(path, '/');
    char* base2 = (char*)strrchr(path, '\\');
    if (base1 && base2)
        return((base1 > base2) ? base1 + 1 : base2 + 1);
    else if (base1)
        return(base1 + 1);
    else if (base2)
        return(base2 + 1);

    return((char*)path);
}
char* dirname(char* path)
{
    char* base1 = (char*)strrchr(path, '/');
    char* base2 = (char*)strrchr(path, '\\');
    if (base1 && base2)
        if (base1 > base2)
            *base1 = 0;
        else
            *base2 = 0;
    else if (base1)
        *base1 = 0;
    else if (base2)
        *base2 = 0;

    return path;
}

#pragma warning( disable : 4996 )
CXIndex createIndexWithInvocationEmissionPath(int ExcludeDeclarationsFromPCH,
    int DisplayDiagnostics) {
    CXIndex Idx;

    CXIndexOptions Opts;
    memset(&Opts, 0, sizeof(Opts));
    Opts.Size = sizeof(CXIndexOptions);
    Opts.ExcludeDeclarationsFromPCH = ExcludeDeclarationsFromPCH;
    Opts.DisplayDiagnostics = DisplayDiagnostics;
    Opts.InvocationEmissionPath = getenv("CINDEXTEST_INVOCATION_EMISSION_PATH");

    Idx = clang_createIndexWithOptions(&Opts);
    if (!Idx) {
        fprintf(stderr,
            "clang_createIndexWithOptions() failed. "
            "CINDEX_VERSION_MINOR = %d, sizeof(CXIndexOptions) = %u\n",
            CINDEX_VERSION_MINOR, Opts.Size);
    }
    return Idx;
}

/** Return the default parsing options. */
static unsigned getDefaultParsingOptions(void) {
    unsigned options = CXTranslationUnit_DetailedPreprocessingRecord;

    if (getenv("CINDEXTEST_EDITING"))
        options |= clang_defaultEditingTranslationUnitOptions();
    if (getenv("CINDEXTEST_COMPLETION_CACHING"))
        options |= CXTranslationUnit_CacheCompletionResults;
    if (getenv("CINDEXTEST_COMPLETION_NO_CACHING"))
        options &= ~CXTranslationUnit_CacheCompletionResults;
    if (getenv("CINDEXTEST_SKIP_FUNCTION_BODIES"))
        options |= CXTranslationUnit_SkipFunctionBodies;
    if (getenv("CINDEXTEST_COMPLETION_BRIEF_COMMENTS"))
        options |= CXTranslationUnit_IncludeBriefCommentsInCodeCompletion;
    if (getenv("CINDEXTEST_CREATE_PREAMBLE_ON_FIRST_PARSE"))
        options |= CXTranslationUnit_CreatePreambleOnFirstParse;
    if (getenv("CINDEXTEST_KEEP_GOING"))
        options |= CXTranslationUnit_KeepGoing;
    if (getenv("CINDEXTEST_LIMIT_SKIP_FUNCTION_BODIES_TO_PREAMBLE"))
        options |= CXTranslationUnit_LimitSkipFunctionBodiesToPreamble;
    if (getenv("CINDEXTEST_INCLUDE_ATTRIBUTED_TYPES"))
        options |= CXTranslationUnit_IncludeAttributedTypes;
    if (getenv("CINDEXTEST_VISIT_IMPLICIT_ATTRIBUTES"))
        options |= CXTranslationUnit_VisitImplicitAttributes;
    if (getenv("CINDEXTEST_IGNORE_NONERRORS_FROM_INCLUDED_FILES"))
        options |= CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles;

    return options;
}

static void ModifyPrintingPolicyAccordingToEnv(CXPrintingPolicy Policy) {
    struct Mapping {
        const char* name;
        enum CXPrintingPolicyProperty property;
    };
    struct Mapping mappings[] = {
        {"CINDEXTEST_PRINTINGPOLICY_INDENTATION", CXPrintingPolicy_Indentation},
        {"CINDEXTEST_PRINTINGPOLICY_SUPPRESSSPECIFIERS",
         CXPrintingPolicy_SuppressSpecifiers},
        {"CINDEXTEST_PRINTINGPOLICY_SUPPRESSTAGKEYWORD",
         CXPrintingPolicy_SuppressTagKeyword},
        {"CINDEXTEST_PRINTINGPOLICY_INCLUDETAGDEFINITION",
         CXPrintingPolicy_IncludeTagDefinition},
        {"CINDEXTEST_PRINTINGPOLICY_SUPPRESSSCOPE",
         CXPrintingPolicy_SuppressScope},
        {"CINDEXTEST_PRINTINGPOLICY_SUPPRESSUNWRITTENSCOPE",
         CXPrintingPolicy_SuppressUnwrittenScope},
        {"CINDEXTEST_PRINTINGPOLICY_SUPPRESSINITIALIZERS",
         CXPrintingPolicy_SuppressInitializers},
        {"CINDEXTEST_PRINTINGPOLICY_CONSTANTARRAYSIZEASWRITTEN",
         CXPrintingPolicy_ConstantArraySizeAsWritten},
        {"CINDEXTEST_PRINTINGPOLICY_ANONYMOUSTAGLOCATIONS",
         CXPrintingPolicy_AnonymousTagLocations},
        {"CINDEXTEST_PRINTINGPOLICY_SUPPRESSSTRONGLIFETIME",
         CXPrintingPolicy_SuppressStrongLifetime},
        {"CINDEXTEST_PRINTINGPOLICY_SUPPRESSLIFETIMEQUALIFIERS",
         CXPrintingPolicy_SuppressLifetimeQualifiers},
        {"CINDEXTEST_PRINTINGPOLICY_SUPPRESSTEMPLATEARGSINCXXCONSTRUCTORS",
         CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors},
        {"CINDEXTEST_PRINTINGPOLICY_BOOL", CXPrintingPolicy_Bool},
        {"CINDEXTEST_PRINTINGPOLICY_RESTRICT", CXPrintingPolicy_Restrict},
        {"CINDEXTEST_PRINTINGPOLICY_ALIGNOF", CXPrintingPolicy_Alignof},
        {"CINDEXTEST_PRINTINGPOLICY_UNDERSCOREALIGNOF",
         CXPrintingPolicy_UnderscoreAlignof},
        {"CINDEXTEST_PRINTINGPOLICY_USEVOIDFORZEROPARAMS",
         CXPrintingPolicy_UseVoidForZeroParams},
        {"CINDEXTEST_PRINTINGPOLICY_TERSEOUTPUT", CXPrintingPolicy_TerseOutput},
        {"CINDEXTEST_PRINTINGPOLICY_POLISHFORDECLARATION",
         CXPrintingPolicy_PolishForDeclaration},
        {"CINDEXTEST_PRINTINGPOLICY_HALF", CXPrintingPolicy_Half},
        {"CINDEXTEST_PRINTINGPOLICY_MSWCHAR", CXPrintingPolicy_MSWChar},
        {"CINDEXTEST_PRINTINGPOLICY_INCLUDENEWLINES",
         CXPrintingPolicy_IncludeNewlines},
        {"CINDEXTEST_PRINTINGPOLICY_MSVCFORMATTING",
         CXPrintingPolicy_MSVCFormatting},
        {"CINDEXTEST_PRINTINGPOLICY_CONSTANTSASWRITTEN",
         CXPrintingPolicy_ConstantsAsWritten},
        {"CINDEXTEST_PRINTINGPOLICY_SUPPRESSIMPLICITBASE",
         CXPrintingPolicy_SuppressImplicitBase},
        {"CINDEXTEST_PRINTINGPOLICY_FULLYQUALIFIEDNAME",
         CXPrintingPolicy_FullyQualifiedName},
    };

    unsigned i;
    for (i = 0; i < sizeof(mappings) / sizeof(struct Mapping); i++) {
        char* value = getenv(mappings[i].name);
        if (value) {
            clang_PrintingPolicy_setProperty(Policy, mappings[i].property,
                (unsigned)strtoul(value, 0L, 10));
        }
    }
}

/** Returns 0 in case of success, non-zero in case of a failure. */
static int checkForErrors(CXTranslationUnit TU);

static void describeLibclangFailure(enum CXErrorCode Err) {
    switch (Err) {
    case CXError_Success:
        fprintf(stderr, "Success\n");
        return;

    case CXError_Failure:
        fprintf(stderr, "Failure (no details available)\n");
        return;

    case CXError_Crashed:
        fprintf(stderr, "Failure: libclang crashed\n");
        return;

    case CXError_InvalidArguments:
        fprintf(stderr, "Failure: invalid arguments passed to a libclang routine\n");
        return;

    case CXError_ASTReadError:
        fprintf(stderr, "Failure: AST deserialization error occurred\n");
        return;
    }
}

static void PrintExtent(FILE* out, unsigned begin_line, unsigned begin_column,
    unsigned end_line, unsigned end_column) {
    fprintf(out, "[%d:%d - %d:%d]", begin_line, begin_column,
        end_line, end_column);
}

static unsigned CreateTranslationUnit(CXIndex Idx, const char* file,
    CXTranslationUnit* TU) {
    enum CXErrorCode Err = clang_createTranslationUnit2(Idx, file, TU);
    if (Err != CXError_Success) {
        fprintf(stderr, "Unable to load translation unit from '%s'!\n", file);
        describeLibclangFailure(Err);
        *TU = 0;
        return 0;
    }
    return 1;
}

void free_remapped_files(struct CXUnsavedFile* unsaved_files,
    int num_unsaved_files) {
    int i;
    for (i = 0; i != num_unsaved_files; ++i) {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
        free((char*)unsaved_files[i].Filename);
        free((char*)unsaved_files[i].Contents);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    }
    free(unsaved_files);
}

static int parse_remapped_files_with_opt(const char* opt_name,
    int argc, const char** argv,
    int start_arg,
    struct CXUnsavedFile** unsaved_files,
    int* num_unsaved_files) {
    int i;
    int arg;
    auto prefix_len = strlen(opt_name);
    int arg_indices[20];
    *unsaved_files = 0;
    *num_unsaved_files = 0;

    /* Count the number of remapped files. */
    for (arg = start_arg; arg < argc; ++arg) {
        if (strncmp(argv[arg], opt_name, prefix_len))
            continue;

        assert(*num_unsaved_files < (int)(sizeof(arg_indices) / sizeof(int)));
        arg_indices[*num_unsaved_files] = arg;
        ++*num_unsaved_files;
    }

    if (*num_unsaved_files == 0)
        return 0;

    *unsaved_files
        = (struct CXUnsavedFile*)malloc(sizeof(struct CXUnsavedFile) *
            *num_unsaved_files);
    assert(*unsaved_files);
    for (i = 0; i != *num_unsaved_files; ++i) {
        struct CXUnsavedFile* unsaved = *unsaved_files + i;
        const char* arg_string = argv[arg_indices[i]] + prefix_len;
        ptrdiff_t filename_len;
        char* filename;
        char* contents;
        FILE* to_file;
        const char* sep = strchr(arg_string, ',');
        if (!sep) {
            fprintf(stderr,
                "error: %sfrom:to argument is missing comma\n", opt_name);
            free_remapped_files(*unsaved_files, i);
            *unsaved_files = 0;
            *num_unsaved_files = 0;
            return -1;
        }

        /* Open the file that we're remapping to. */
        to_file = fopen(sep + 1, "rb");
        if (!to_file) {
            fprintf(stderr, "error: cannot open file %s that we are remapping to\n",
                sep + 1);
            free_remapped_files(*unsaved_files, i);
            *unsaved_files = 0;
            *num_unsaved_files = 0;
            return -1;
        }

        /* Determine the length of the file we're remapping to. */
        fseek(to_file, 0, SEEK_END);
        unsaved->Length = ftell(to_file);
        fseek(to_file, 0, SEEK_SET);

        /* Read the contents of the file we're remapping to. */
        contents = (char*)malloc(unsaved->Length + 1);
        assert(contents);
        if (fread(contents, 1, unsaved->Length, to_file) != unsaved->Length) {
            fprintf(stderr, "error: unexpected %s reading 'to' file %s\n",
                (feof(to_file) ? "EOF" : "error"), sep + 1);
            fclose(to_file);
            free_remapped_files(*unsaved_files, i);
            free(contents);
            *unsaved_files = 0;
            *num_unsaved_files = 0;
            return -1;
        }
        contents[unsaved->Length] = 0;
        unsaved->Contents = contents;

        /* Close the file. */
        fclose(to_file);

        /* Copy the file name that we're remapping from. */
        filename_len = sep - arg_string;
        filename = (char*)malloc(filename_len + 1);
        assert(filename);
        memcpy(filename, arg_string, filename_len);
        filename[filename_len] = 0;
        unsaved->Filename = filename;
    }

    return 0;
}

static int parse_remapped_files(int argc, const char** argv, int start_arg,
    struct CXUnsavedFile** unsaved_files,
    int* num_unsaved_files) {
    return parse_remapped_files_with_opt("-remap-file=", argc, argv, start_arg,
        unsaved_files, num_unsaved_files);
}

static int parse_remapped_files_with_try(int try_idx,
    int argc, const char** argv,
    int start_arg,
    struct CXUnsavedFile** unsaved_files,
    int* num_unsaved_files) {
    struct CXUnsavedFile* unsaved_files_no_try_idx;
    int num_unsaved_files_no_try_idx;
    struct CXUnsavedFile* unsaved_files_try_idx;
    int num_unsaved_files_try_idx;
    int ret;
    char opt_name[32];

    ret = parse_remapped_files(argc, argv, start_arg,
        &unsaved_files_no_try_idx, &num_unsaved_files_no_try_idx);
    if (ret)
        return ret;

    sprintf(opt_name, "-remap-file-%d=", try_idx);
    ret = parse_remapped_files_with_opt(opt_name, argc, argv, start_arg,
        &unsaved_files_try_idx, &num_unsaved_files_try_idx);
    if (ret)
        return ret;

    if (num_unsaved_files_no_try_idx == 0) {
        *unsaved_files = unsaved_files_try_idx;
        *num_unsaved_files = num_unsaved_files_try_idx;
        return 0;
    }
    if (num_unsaved_files_try_idx == 0) {
        *unsaved_files = unsaved_files_no_try_idx;
        *num_unsaved_files = num_unsaved_files_no_try_idx;
        return 0;
    }

    *num_unsaved_files = num_unsaved_files_no_try_idx + num_unsaved_files_try_idx;
    *unsaved_files
        = (struct CXUnsavedFile*)realloc(unsaved_files_no_try_idx,
            sizeof(struct CXUnsavedFile) *
            *num_unsaved_files);
    assert(*unsaved_files);
    memcpy(*unsaved_files + num_unsaved_files_no_try_idx,
        unsaved_files_try_idx, sizeof(struct CXUnsavedFile) *
        num_unsaved_files_try_idx);
    free(unsaved_files_try_idx);
    return 0;
}

static const char* parse_comments_schema(int argc, const char** argv) {
    const char* CommentsSchemaArg = "-comments-xml-schema=";
    const char* CommentSchemaFile = NULL;

    if (argc == 0)
        return CommentSchemaFile;

    if (!strncmp(argv[0], CommentsSchemaArg, strlen(CommentsSchemaArg)))
        CommentSchemaFile = argv[0] + strlen(CommentsSchemaArg);

    return CommentSchemaFile;
}

/******************************************************************************/
/* Pretty-printing.                                                           */
/******************************************************************************/

static const char* FileCheckPrefix = "CHECK";

static void PrintCString(const char* CStr) {
    if (CStr != NULL && CStr[0] != '\0') {
        for (; *CStr; ++CStr) {
            const char C = *CStr;
            switch (C) {
            case '\n': printf("\\n"); break;
            case '\r': printf("\\r"); break;
            case '\t': printf("\\t"); break;
            case '\v': printf("\\v"); break;
            case '\f': printf("\\f"); break;
            default:   putchar(C);    break;
            }
        }
    }
}

static void PrintCStringWithPrefix(const char* Prefix, const char* CStr) {
    printf(" %s=[", Prefix);
    PrintCString(CStr);
    printf("]");
}

static void PrintCXStringAndDispose(CXString Str) {
    PrintCString(clang_getCString(Str));
    clang_disposeString(Str);
}

static void PrintCXStringWithPrefix(const char* Prefix, CXString Str) {
    PrintCStringWithPrefix(Prefix, clang_getCString(Str));
}

static void PrintCXStringWithPrefixAndDispose(const char* Prefix,
    CXString Str) {
    PrintCStringWithPrefix(Prefix, clang_getCString(Str));
    clang_disposeString(Str);
}

static void PrintRange(CXSourceRange R, const char* str) {
    CXFile begin_file, end_file;
    unsigned begin_line, begin_column, end_line, end_column;

    clang_getSpellingLocation(clang_getRangeStart(R),
        &begin_file, &begin_line, &begin_column, 0);
    clang_getSpellingLocation(clang_getRangeEnd(R),
        &end_file, &end_line, &end_column, 0);
    if (!begin_file || !end_file)
        return;

    if (str)
        printf(" %s=", str);
    PrintExtent(stdout, begin_line, begin_column, end_line, end_column);
}

static enum DisplayType {
    DisplayType_Spelling,
    DisplayType_DisplayName,
    DisplayType_Pretty
} wanted_display_type = DisplayType_Spelling;

static void printVersion(const char* Prefix, CXVersion Version) {
    if (Version.Major < 0)
        return;
    printf("%s%d", Prefix, Version.Major);

    if (Version.Minor < 0)
        return;
    printf(".%d", Version.Minor);

    if (Version.Subminor < 0)
        return;
    printf(".%d", Version.Subminor);
}

struct CommentASTDumpingContext {
    int IndentLevel;
};

static void DumpCXCommentInternal(struct CommentASTDumpingContext* Ctx,
    CXComment Comment) {
    unsigned i;
    unsigned e;
    enum CXCommentKind Kind = clang_Comment_getKind(Comment);

    Ctx->IndentLevel++;
    for (i = 0, e = Ctx->IndentLevel; i != e; ++i)
        printf("  ");

    printf("(");
    switch (Kind) {
    case CXComment_Null:
        printf("CXComment_Null");
        break;
    case CXComment_Text:
        printf("CXComment_Text");
        PrintCXStringWithPrefixAndDispose("Text",
            clang_TextComment_getText(Comment));
        if (clang_Comment_isWhitespace(Comment))
            printf(" IsWhitespace");
        if (clang_InlineContentComment_hasTrailingNewline(Comment))
            printf(" HasTrailingNewline");
        break;
    case CXComment_InlineCommand:
        printf("CXComment_InlineCommand");
        PrintCXStringWithPrefixAndDispose(
            "CommandName",
            clang_InlineCommandComment_getCommandName(Comment));
        switch (clang_InlineCommandComment_getRenderKind(Comment)) {
        case CXCommentInlineCommandRenderKind_Normal:
            printf(" RenderNormal");
            break;
        case CXCommentInlineCommandRenderKind_Bold:
            printf(" RenderBold");
            break;
        case CXCommentInlineCommandRenderKind_Monospaced:
            printf(" RenderMonospaced");
            break;
        case CXCommentInlineCommandRenderKind_Emphasized:
            printf(" RenderEmphasized");
            break;
        case CXCommentInlineCommandRenderKind_Anchor:
            printf(" RenderAnchor");
            break;
        }
        for (i = 0, e = clang_InlineCommandComment_getNumArgs(Comment);
            i != e; ++i) {
            printf(" Arg[%u]=", i);
            PrintCXStringAndDispose(
                clang_InlineCommandComment_getArgText(Comment, i));
        }
        if (clang_InlineContentComment_hasTrailingNewline(Comment))
            printf(" HasTrailingNewline");
        break;
    case CXComment_HTMLStartTag: {
        unsigned NumAttrs;
        printf("CXComment_HTMLStartTag");
        PrintCXStringWithPrefixAndDispose(
            "Name",
            clang_HTMLTagComment_getTagName(Comment));
        NumAttrs = clang_HTMLStartTag_getNumAttrs(Comment);
        if (NumAttrs != 0) {
            printf(" Attrs:");
            for (i = 0; i != NumAttrs; ++i) {
                printf(" ");
                PrintCXStringAndDispose(clang_HTMLStartTag_getAttrName(Comment, i));
                printf("=");
                PrintCXStringAndDispose(clang_HTMLStartTag_getAttrValue(Comment, i));
            }
        }
        if (clang_HTMLStartTagComment_isSelfClosing(Comment))
            printf(" SelfClosing");
        if (clang_InlineContentComment_hasTrailingNewline(Comment))
            printf(" HasTrailingNewline");
        break;
    }
    case CXComment_HTMLEndTag:
        printf("CXComment_HTMLEndTag");
        PrintCXStringWithPrefixAndDispose(
            "Name",
            clang_HTMLTagComment_getTagName(Comment));
        if (clang_InlineContentComment_hasTrailingNewline(Comment))
            printf(" HasTrailingNewline");
        break;
    case CXComment_Paragraph:
        printf("CXComment_Paragraph");
        if (clang_Comment_isWhitespace(Comment))
            printf(" IsWhitespace");
        break;
    case CXComment_BlockCommand:
        printf("CXComment_BlockCommand");
        PrintCXStringWithPrefixAndDispose(
            "CommandName",
            clang_BlockCommandComment_getCommandName(Comment));
        for (i = 0, e = clang_BlockCommandComment_getNumArgs(Comment);
            i != e; ++i) {
            printf(" Arg[%u]=", i);
            PrintCXStringAndDispose(
                clang_BlockCommandComment_getArgText(Comment, i));
        }
        break;
    case CXComment_ParamCommand:
        printf("CXComment_ParamCommand");
        switch (clang_ParamCommandComment_getDirection(Comment)) {
        case CXCommentParamPassDirection_In:
            printf(" in");
            break;
        case CXCommentParamPassDirection_Out:
            printf(" out");
            break;
        case CXCommentParamPassDirection_InOut:
            printf(" in,out");
            break;
        }
        if (clang_ParamCommandComment_isDirectionExplicit(Comment))
            printf(" explicitly");
        else
            printf(" implicitly");
        PrintCXStringWithPrefixAndDispose(
            "ParamName",
            clang_ParamCommandComment_getParamName(Comment));
        if (clang_ParamCommandComment_isParamIndexValid(Comment))
            printf(" ParamIndex=%u", clang_ParamCommandComment_getParamIndex(Comment));
        else
            printf(" ParamIndex=Invalid");
        break;
    case CXComment_TParamCommand:
        printf("CXComment_TParamCommand");
        PrintCXStringWithPrefixAndDispose(
            "ParamName",
            clang_TParamCommandComment_getParamName(Comment));
        if (clang_TParamCommandComment_isParamPositionValid(Comment)) {
            printf(" ParamPosition={");
            for (i = 0, e = clang_TParamCommandComment_getDepth(Comment);
                i != e; ++i) {
                printf("%u", clang_TParamCommandComment_getIndex(Comment, i));
                if (i != e - 1)
                    printf(", ");
            }
            printf("}");
        }
        else
            printf(" ParamPosition=Invalid");
        break;
    case CXComment_VerbatimBlockCommand:
        printf("CXComment_VerbatimBlockCommand");
        PrintCXStringWithPrefixAndDispose(
            "CommandName",
            clang_BlockCommandComment_getCommandName(Comment));
        break;
    case CXComment_VerbatimBlockLine:
        printf("CXComment_VerbatimBlockLine");
        PrintCXStringWithPrefixAndDispose(
            "Text",
            clang_VerbatimBlockLineComment_getText(Comment));
        break;
    case CXComment_VerbatimLine:
        printf("CXComment_VerbatimLine");
        PrintCXStringWithPrefixAndDispose(
            "Text",
            clang_VerbatimLineComment_getText(Comment));
        break;
    case CXComment_FullComment:
        printf("CXComment_FullComment");
        break;
    }
    if (Kind != CXComment_Null) {
        const unsigned NumChildren = clang_Comment_getNumChildren(Comment);
        unsigned i;
        for (i = 0; i != NumChildren; ++i) {
            printf("\n// %s: ", FileCheckPrefix);
            DumpCXCommentInternal(Ctx, clang_Comment_getChild(Comment, i));
        }
    }
    printf(")");
    Ctx->IndentLevel--;
}

static void DumpCXComment(CXComment Comment) {
    struct CommentASTDumpingContext Ctx;
    Ctx.IndentLevel = 1;
    printf("\n// %s:  CommentAST=[\n// %s:", FileCheckPrefix, FileCheckPrefix);
    DumpCXCommentInternal(&Ctx, Comment);
    printf("]");
}

static void ValidateCommentXML(const char* Str, const char* CommentSchemaFile) {
#ifdef CLANG_HAVE_LIBXML
    xmlRelaxNGParserCtxtPtr RNGParser;
    xmlRelaxNGPtr Schema;
    xmlDocPtr Doc;
    xmlRelaxNGValidCtxtPtr ValidationCtxt;
    int status;

    if (!CommentSchemaFile)
        return;

    RNGParser = xmlRelaxNGNewParserCtxt(CommentSchemaFile);
    if (!RNGParser) {
        printf(" libXMLError");
        return;
    }
    Schema = xmlRelaxNGParse(RNGParser);

    Doc = xmlParseDoc((const xmlChar*)Str);

    if (!Doc) {
        xmlErrorPtr Error = xmlGetLastError();
        printf(" CommentXMLInvalid [not well-formed XML: %s]", Error->message);
        return;
    }

    ValidationCtxt = xmlRelaxNGNewValidCtxt(Schema);
    status = xmlRelaxNGValidateDoc(ValidationCtxt, Doc);
    if (!status)
        printf(" CommentXMLValid");
    else if (status > 0) {
        xmlErrorPtr Error = xmlGetLastError();
        printf(" CommentXMLInvalid [not valid XML: %s]", Error->message);
    }
    else
        printf(" libXMLError");

    xmlRelaxNGFreeValidCtxt(ValidationCtxt);
    xmlFreeDoc(Doc);
    xmlRelaxNGFree(Schema);
    xmlRelaxNGFreeParserCtxt(RNGParser);
#endif
}

static void PrintCursorComments(CXCursor Cursor,
    const char* CommentSchemaFile) {
        {
            CXString RawComment;
            const char* RawCommentCString;
            CXString BriefComment;
            const char* BriefCommentCString;

            RawComment = clang_Cursor_getRawCommentText(Cursor);
            RawCommentCString = clang_getCString(RawComment);
            if (RawCommentCString != NULL && RawCommentCString[0] != '\0') {
                PrintCStringWithPrefix("RawComment", RawCommentCString);
                PrintRange(clang_Cursor_getCommentRange(Cursor), "RawCommentRange");

                BriefComment = clang_Cursor_getBriefCommentText(Cursor);
                BriefCommentCString = clang_getCString(BriefComment);
                if (BriefCommentCString != NULL && BriefCommentCString[0] != '\0')
                    PrintCStringWithPrefix("BriefComment", BriefCommentCString);
                clang_disposeString(BriefComment);
            }
            clang_disposeString(RawComment);
        }

        {
            CXComment Comment = clang_Cursor_getParsedComment(Cursor);
            if (clang_Comment_getKind(Comment) != CXComment_Null) {
                PrintCXStringWithPrefixAndDispose("FullCommentAsHTML",
                    clang_FullComment_getAsHTML(Comment));
                {
                    CXString XML;
                    XML = clang_FullComment_getAsXML(Comment);
                    PrintCXStringWithPrefix("FullCommentAsXML", XML);
                    ValidateCommentXML(clang_getCString(XML), CommentSchemaFile);
                    clang_disposeString(XML);
                }

                DumpCXComment(Comment);
            }
        }
}

typedef struct {
    unsigned line;
    unsigned col;
} LineCol;

static int lineCol_cmp(const void* p1, const void* p2) {
    const LineCol* lhs = static_cast<const LineCol*>(p1);
    const LineCol* rhs = static_cast<const LineCol*>(p2);
    if (lhs->line != rhs->line)
        return (int)lhs->line - (int)rhs->line;
    return (int)lhs->col - (int)rhs->col;
}

static CXString CursorToText(CXCursor Cursor) {
    CXString text;
    switch (wanted_display_type) {
    case DisplayType_Spelling:
        return clang_getCursorSpelling(Cursor);
    case DisplayType_DisplayName:
        return clang_getCursorDisplayName(Cursor);
    case DisplayType_Pretty: {
        CXPrintingPolicy Policy = clang_getCursorPrintingPolicy(Cursor);
        ModifyPrintingPolicyAccordingToEnv(Policy);
        text = clang_getCursorPrettyPrinted(Cursor, Policy);
        clang_PrintingPolicy_dispose(Policy);
        return text;
    }
    }
    assert(0 && "unknown display type"); /* no llvm_unreachable in C. */
    /* Set to NULL to prevent uninitialized variable warnings. */
    text.data = NULL;
    text.private_flags = 0;
    return text;
}

static void PrintCursor(CXCursor Cursor, const char* CommentSchemaFile) {
    CXTranslationUnit TU = clang_Cursor_getTranslationUnit(Cursor);
    if (clang_isInvalid(Cursor.kind)) {
        CXString ks = clang_getCursorKindSpelling(Cursor.kind);
        printf("Invalid Cursor => %s", clang_getCString(ks));
        clang_disposeString(ks);
    }
    else {
        CXString string, ks;
        CXCursor Referenced;
        unsigned line, column;
        CXCursor SpecializationOf;
        CXCursor* overridden;
        unsigned num_overridden;
        unsigned RefNameRangeNr;
        CXSourceRange CursorExtent;
        CXSourceRange RefNameRange;
        int AlwaysUnavailable;
        int AlwaysDeprecated;
        CXString UnavailableMessage;
        CXString DeprecatedMessage;
        CXPlatformAvailability PlatformAvailability[2];
        int NumPlatformAvailability;
        int I;

        ks = clang_getCursorKindSpelling(Cursor.kind);
        string = CursorToText(Cursor);
        printf("%s=%s", clang_getCString(ks),
            clang_getCString(string));
        clang_disposeString(ks);
        clang_disposeString(string);

        Referenced = clang_getCursorReferenced(Cursor);
        if (!clang_equalCursors(Referenced, clang_getNullCursor())) {
            if (clang_getCursorKind(Referenced) == CXCursor_OverloadedDeclRef) {
                unsigned I, N = clang_getNumOverloadedDecls(Referenced);
                printf("[");
                for (I = 0; I != N; ++I) {
                    CXCursor Ovl = clang_getOverloadedDecl(Referenced, I);
                    CXSourceLocation Loc;
                    if (I)
                        printf(", ");

                    Loc = clang_getCursorLocation(Ovl);
                    clang_getSpellingLocation(Loc, 0, &line, &column, 0);
                    printf("%d:%d", line, column);
                }
                printf("]");
            }
            else {
                CXSourceLocation Loc = clang_getCursorLocation(Referenced);
                clang_getSpellingLocation(Loc, 0, &line, &column, 0);
                printf(":%d:%d", line, column);
            }

            if (clang_getCursorKind(Referenced) == CXCursor_TypedefDecl) {
                CXType T = clang_getCursorType(Referenced);
                if (clang_Type_isTransparentTagTypedef(T)) {
                    CXType Underlying = clang_getTypedefDeclUnderlyingType(Referenced);
                    CXString S = clang_getTypeSpelling(Underlying);
                    printf(" (Transparent: %s)", clang_getCString(S));
                    clang_disposeString(S);
                }
            }
        }

        if (clang_isCursorDefinition(Cursor))
            printf(" (Definition)");

        switch (clang_getCursorAvailability(Cursor)) {
        case CXAvailability_Available:
            break;

        case CXAvailability_Deprecated:
            printf(" (deprecated)");
            break;

        case CXAvailability_NotAvailable:
            printf(" (unavailable)");
            break;

        case CXAvailability_NotAccessible:
            printf(" (inaccessible)");
            break;
        }

        NumPlatformAvailability
            = clang_getCursorPlatformAvailability(Cursor,
                &AlwaysDeprecated,
                &DeprecatedMessage,
                &AlwaysUnavailable,
                &UnavailableMessage,
                PlatformAvailability, 2);
        if (AlwaysUnavailable) {
            printf("  (always unavailable: \"%s\")",
                clang_getCString(UnavailableMessage));
        }
        else if (AlwaysDeprecated) {
            printf("  (always deprecated: \"%s\")",
                clang_getCString(DeprecatedMessage));
        }
        else {
            for (I = 0; I != NumPlatformAvailability; ++I) {
                if (I >= 2)
                    break;

                printf("  (%s", clang_getCString(PlatformAvailability[I].Platform));
                if (PlatformAvailability[I].Unavailable)
                    printf(", unavailable");
                else {
                    printVersion(", introduced=", PlatformAvailability[I].Introduced);
                    printVersion(", deprecated=", PlatformAvailability[I].Deprecated);
                    printVersion(", obsoleted=", PlatformAvailability[I].Obsoleted);
                }
                if (clang_getCString(PlatformAvailability[I].Message)[0])
                    printf(", message=\"%s\"",
                        clang_getCString(PlatformAvailability[I].Message));
                printf(")");
            }
        }
        for (I = 0; I != NumPlatformAvailability; ++I) {
            if (I >= 2)
                break;
            clang_disposeCXPlatformAvailability(PlatformAvailability + I);
        }

        clang_disposeString(DeprecatedMessage);
        clang_disposeString(UnavailableMessage);

        if (clang_CXXConstructor_isDefaultConstructor(Cursor))
            printf(" (default constructor)");

        if (clang_CXXConstructor_isMoveConstructor(Cursor))
            printf(" (move constructor)");
        if (clang_CXXConstructor_isCopyConstructor(Cursor))
            printf(" (copy constructor)");
        if (clang_CXXConstructor_isConvertingConstructor(Cursor))
            printf(" (converting constructor)");
        if (clang_CXXField_isMutable(Cursor))
            printf(" (mutable)");
        if (clang_CXXMethod_isDefaulted(Cursor))
            printf(" (defaulted)");
        if (clang_CXXMethod_isDeleted(Cursor))
            printf(" (deleted)");
        if (clang_CXXMethod_isStatic(Cursor))
            printf(" (static)");
        if (clang_CXXMethod_isVirtual(Cursor))
            printf(" (virtual)");
        if (clang_CXXMethod_isConst(Cursor))
            printf(" (const)");
        if (clang_CXXMethod_isPureVirtual(Cursor))
            printf(" (pure)");
        if (clang_CXXMethod_isCopyAssignmentOperator(Cursor))
            printf(" (copy-assignment operator)");
        if (clang_CXXMethod_isMoveAssignmentOperator(Cursor))
            printf(" (move-assignment operator)");
        if (clang_CXXMethod_isExplicit(Cursor))
            printf(" (explicit)");
        if (clang_CXXRecord_isAbstract(Cursor))
            printf(" (abstract)");
        if (clang_EnumDecl_isScoped(Cursor))
            printf(" (scoped)");
        if (clang_Cursor_isVariadic(Cursor))
            printf(" (variadic)");
        if (clang_Cursor_isObjCOptional(Cursor))
            printf(" (@optional)");
        if (clang_isInvalidDeclaration(Cursor))
            printf(" (invalid)");

        switch (clang_getCursorExceptionSpecificationType(Cursor))
        {
        case CXCursor_ExceptionSpecificationKind_None:
            break;

        case CXCursor_ExceptionSpecificationKind_DynamicNone:
            printf(" (noexcept dynamic none)");
            break;

        case CXCursor_ExceptionSpecificationKind_Dynamic:
            printf(" (noexcept dynamic)");
            break;

        case CXCursor_ExceptionSpecificationKind_MSAny:
            printf(" (noexcept dynamic any)");
            break;

        case CXCursor_ExceptionSpecificationKind_BasicNoexcept:
            printf(" (noexcept)");
            break;

        case CXCursor_ExceptionSpecificationKind_ComputedNoexcept:
            printf(" (computed-noexcept)");
            break;

        case CXCursor_ExceptionSpecificationKind_Unevaluated:
        case CXCursor_ExceptionSpecificationKind_Uninstantiated:
        case CXCursor_ExceptionSpecificationKind_Unparsed:
            break;
        }

        {
            CXString language;
            CXString definedIn;
            unsigned generated;
            if (clang_Cursor_isExternalSymbol(Cursor, &language, &definedIn,
                &generated)) {
                printf(" (external lang: %s, defined: %s, gen: %d)",
                    clang_getCString(language), clang_getCString(definedIn), generated);
                clang_disposeString(language);
                clang_disposeString(definedIn);
            }
        }

        if (Cursor.kind == CXCursor_IBOutletCollectionAttr) {
            CXType T =
                clang_getCanonicalType(clang_getIBOutletCollectionType(Cursor));
            CXString S = clang_getTypeKindSpelling(T.kind);
            printf(" [IBOutletCollection=%s]", clang_getCString(S));
            clang_disposeString(S);
        }

        if (Cursor.kind == CXCursor_CXXBaseSpecifier) {
            enum CX_CXXAccessSpecifier access = clang_getCXXAccessSpecifier(Cursor);
            unsigned isVirtual = clang_isVirtualBase(Cursor);
            const char* accessStr = 0;

            switch (access) {
            case CX_CXXInvalidAccessSpecifier:
                accessStr = "invalid"; break;
            case CX_CXXPublic:
                accessStr = "public"; break;
            case CX_CXXProtected:
                accessStr = "protected"; break;
            case CX_CXXPrivate:
                accessStr = "private"; break;
            }

            printf(" [access=%s isVirtual=%s]", accessStr,
                isVirtual ? "true" : "false");
        }

        SpecializationOf = clang_getSpecializedCursorTemplate(Cursor);
        if (!clang_equalCursors(SpecializationOf, clang_getNullCursor())) {
            CXSourceLocation Loc = clang_getCursorLocation(SpecializationOf);
            CXString Name = clang_getCursorSpelling(SpecializationOf);
            clang_getSpellingLocation(Loc, 0, &line, &column, 0);
            printf(" [Specialization of %s:%d:%d]",
                clang_getCString(Name), line, column);
            clang_disposeString(Name);

            if (Cursor.kind == CXCursor_FunctionDecl
                || Cursor.kind == CXCursor_StructDecl
                || Cursor.kind == CXCursor_ClassDecl
                || Cursor.kind == CXCursor_ClassTemplatePartialSpecialization) {
                /* Collect the template parameter kinds from the base template. */
                int NumTemplateArgs = clang_Cursor_getNumTemplateArguments(Cursor);
                int I;
                if (NumTemplateArgs < 0) {
                    printf(" [no template arg info]");
                }
                for (I = 0; I < NumTemplateArgs; I++) {
                    enum CXTemplateArgumentKind TAK =
                        clang_Cursor_getTemplateArgumentKind(Cursor, I);
                    switch (TAK) {
                    case CXTemplateArgumentKind_Type:
                    {
                        CXType T = clang_Cursor_getTemplateArgumentType(Cursor, I);
                        CXString S = clang_getTypeSpelling(T);
                        printf(" [Template arg %d: kind: %d, type: %s]",
                            I, TAK, clang_getCString(S));
                        clang_disposeString(S);
                    }
                    break;
                    case CXTemplateArgumentKind_Integral:
                        printf(" [Template arg %d: kind: %d, intval: %lld]",
                            I, TAK, clang_Cursor_getTemplateArgumentValue(Cursor, I));
                        break;
                    default:
                        printf(" [Template arg %d: kind: %d]\n", I, TAK);
                    }
                }
            }
        }

        clang_getOverriddenCursors(Cursor, &overridden, &num_overridden);
        if (num_overridden) {
            unsigned I;
            LineCol lineCols[50];
            assert(num_overridden <= 50);
            printf(" [Overrides ");
            for (I = 0; I != num_overridden; ++I) {
                CXSourceLocation Loc = clang_getCursorLocation(overridden[I]);
                clang_getSpellingLocation(Loc, 0, &line, &column, 0);
                lineCols[I].line = line;
                lineCols[I].col = column;
            }
            /* Make the order of the override list deterministic. */
            qsort(lineCols, num_overridden, sizeof(LineCol), lineCol_cmp);
            for (I = 0; I != num_overridden; ++I) {
                if (I)
                    printf(", ");
                printf("@%d:%d", lineCols[I].line, lineCols[I].col);
            }
            printf("]");
            clang_disposeOverriddenCursors(overridden);
        }

        if (Cursor.kind == CXCursor_InclusionDirective) {
            CXFile File = clang_getIncludedFile(Cursor);
            CXString Included = clang_getFileName(File);
            const char* IncludedString = clang_getCString(Included);
            printf(" (%s)", IncludedString ? IncludedString : "(null)");
            clang_disposeString(Included);

            if (clang_isFileMultipleIncludeGuarded(TU, File))
                printf("  [multi-include guarded]");
        }

        CursorExtent = clang_getCursorExtent(Cursor);
        RefNameRange = clang_getCursorReferenceNameRange(Cursor,
            CXNameRange_WantQualifier
            | CXNameRange_WantSinglePiece
            | CXNameRange_WantTemplateArgs,
            0);
        if (!clang_equalRanges(CursorExtent, RefNameRange))
            PrintRange(RefNameRange, "SingleRefName");

        for (RefNameRangeNr = 0; 1; RefNameRangeNr++) {
            RefNameRange = clang_getCursorReferenceNameRange(Cursor,
                CXNameRange_WantQualifier
                | CXNameRange_WantTemplateArgs,
                RefNameRangeNr);
            if (clang_equalRanges(clang_getNullRange(), RefNameRange))
                break;
            if (!clang_equalRanges(CursorExtent, RefNameRange))
                PrintRange(RefNameRange, "RefName");
        }

        PrintCursorComments(Cursor, CommentSchemaFile);

        {
            unsigned PropAttrs = clang_Cursor_getObjCPropertyAttributes(Cursor, 0);
            if (PropAttrs != CXObjCPropertyAttr_noattr) {
                printf(" [");
#define PRINT_PROP_ATTR(A) \
          if (PropAttrs & CXObjCPropertyAttr_##A) printf(#A ",")
                PRINT_PROP_ATTR(readonly);
                PRINT_PROP_ATTR(getter);
                PRINT_PROP_ATTR(assign);
                PRINT_PROP_ATTR(readwrite);
                PRINT_PROP_ATTR(retain);
                PRINT_PROP_ATTR(copy);
                PRINT_PROP_ATTR(nonatomic);
                PRINT_PROP_ATTR(setter);
                PRINT_PROP_ATTR(atomic);
                PRINT_PROP_ATTR(weak);
                PRINT_PROP_ATTR(strong);
                PRINT_PROP_ATTR(unsafe_unretained);
                PRINT_PROP_ATTR(class);
                printf("]");
            }
        }

        if (Cursor.kind == CXCursor_ObjCPropertyDecl) {
            CXString Name = clang_Cursor_getObjCPropertyGetterName(Cursor);
            CXString Spelling = clang_getCursorSpelling(Cursor);
            const char* CName = clang_getCString(Name);
            const char* CSpelling = clang_getCString(Spelling);
            if (CName && strcmp(CName, CSpelling)) {
                printf(" (getter=%s)", CName);
            }
            clang_disposeString(Spelling);
            clang_disposeString(Name);
        }

        if (Cursor.kind == CXCursor_ObjCPropertyDecl) {
            CXString Name = clang_Cursor_getObjCPropertySetterName(Cursor);
            CXString Spelling = clang_getCursorSpelling(Cursor);
            const char* CName = clang_getCString(Name);
            const char* CSpelling = clang_getCString(Spelling);
            char* DefaultSetter = static_cast<char*>(malloc(strlen(CSpelling) + 5));
            sprintf(DefaultSetter, "set%s:", CSpelling);
            DefaultSetter[3] &= ~(1 << 5); /* Make uppercase */
            if (CName && strcmp(CName, DefaultSetter)) {
                printf(" (setter=%s)", CName);
            }
            free(DefaultSetter);
            clang_disposeString(Spelling);
            clang_disposeString(Name);
        }

        {
            unsigned QT = clang_Cursor_getObjCDeclQualifiers(Cursor);
            if (QT != CXObjCDeclQualifier_None) {
                printf(" [");
#define PRINT_OBJC_QUAL(A) \
          if (QT & CXObjCDeclQualifier_##A) printf(#A ",")
                PRINT_OBJC_QUAL(In);
                PRINT_OBJC_QUAL(Inout);
                PRINT_OBJC_QUAL(Out);
                PRINT_OBJC_QUAL(Bycopy);
                PRINT_OBJC_QUAL(Byref);
                PRINT_OBJC_QUAL(Oneway);
                printf("]");
            }
        }
    }
}

static const char* GetCursorSource(CXCursor Cursor) {
    CXSourceLocation Loc = clang_getCursorLocation(Cursor);
    CXString source;
    CXFile file;
    clang_getExpansionLocation(Loc, &file, 0, 0, 0);
    source = clang_getFileName(file);
    if (!clang_getCString(source)) {
        clang_disposeString(source);
        return "<invalid loc>";
    }
    else {
        const char* b = basename(clang_getCString(source));
        clang_disposeString(source);
        return b;
    }
}

static CXString createCXString(const char* CS) {
    CXString Str;
    Str.data = (const void*)CS;
    Str.private_flags = 0;
    return Str;
}

/******************************************************************************/
/* Callbacks.                                                                 */
/******************************************************************************/

typedef void (*PostVisitTU)(CXTranslationUnit);

void PrintDiagnostic(CXDiagnostic Diagnostic) {
    FILE* out = stderr;
    CXFile file;
    CXString Msg;
    unsigned display_opts = CXDiagnostic_DisplaySourceLocation
        | CXDiagnostic_DisplayColumn | CXDiagnostic_DisplaySourceRanges
        | CXDiagnostic_DisplayOption;
    unsigned i, num_fixits;

    if (clang_getDiagnosticSeverity(Diagnostic) == CXDiagnostic_Ignored)
        return;

    Msg = clang_formatDiagnostic(Diagnostic, display_opts);
    fprintf(stderr, "%s\n", clang_getCString(Msg));
    clang_disposeString(Msg);

    clang_getSpellingLocation(clang_getDiagnosticLocation(Diagnostic),
        &file, 0, 0, 0);
    if (!file)
        return;

    num_fixits = clang_getDiagnosticNumFixIts(Diagnostic);
    fprintf(stderr, "Number FIX-ITs = %d\n", num_fixits);
    for (i = 0; i != num_fixits; ++i) {
        CXSourceRange range;
        CXString insertion_text = clang_getDiagnosticFixIt(Diagnostic, i, &range);
        CXSourceLocation start = clang_getRangeStart(range);
        CXSourceLocation end = clang_getRangeEnd(range);
        unsigned start_line, start_column, end_line, end_column;
        CXFile start_file, end_file;
        clang_getSpellingLocation(start, &start_file, &start_line,
            &start_column, 0);
        clang_getSpellingLocation(end, &end_file, &end_line, &end_column, 0);
        if (clang_equalLocations(start, end)) {
            /* Insertion. */
            if (start_file == file)
                fprintf(out, "FIX-IT: Insert \"%s\" at %d:%d\n",
                    clang_getCString(insertion_text), start_line, start_column);
        }
        else if (strcmp(clang_getCString(insertion_text), "") == 0) {
            /* Removal. */
            if (start_file == file && end_file == file) {
                fprintf(out, "FIX-IT: Remove ");
                PrintExtent(out, start_line, start_column, end_line, end_column);
                fprintf(out, "\n");
            }
        }
        else {
            /* Replacement. */
            if (start_file == end_file) {
                fprintf(out, "FIX-IT: Replace ");
                PrintExtent(out, start_line, start_column, end_line, end_column);
                fprintf(out, " with \"%s\"\n", clang_getCString(insertion_text));
            }
        }
        clang_disposeString(insertion_text);
    }
}

void PrintDiagnosticSet(CXDiagnosticSet Set) {
    int i = 0, n = clang_getNumDiagnosticsInSet(Set);
    for (; i != n; ++i) {
        CXDiagnostic Diag = clang_getDiagnosticInSet(Set, i);
        CXDiagnosticSet ChildDiags = clang_getChildDiagnostics(Diag);
        PrintDiagnostic(Diag);
        if (ChildDiags)
            PrintDiagnosticSet(ChildDiags);
    }
}

void PrintDiagnostics(CXTranslationUnit TU) {
    CXDiagnosticSet TUSet = clang_getDiagnosticSetFromTU(TU);
    PrintDiagnosticSet(TUSet);
    clang_disposeDiagnosticSet(TUSet);
}

void PrintMemoryUsage(CXTranslationUnit TU) {
    unsigned long total = 0;
    unsigned i = 0;
    CXTUResourceUsage usage = clang_getCXTUResourceUsage(TU);
    fprintf(stderr, "Memory usage:\n");
    for (i = 0; i != usage.numEntries; ++i) {
        const char* name = clang_getTUResourceUsageName(usage.entries[i].kind);
        unsigned long amount = usage.entries[i].amount;
        total += amount;
        fprintf(stderr, "  %s : %ld bytes (%f MBytes)\n", name, amount,
            ((double)amount) / (1024 * 1024));
    }
    fprintf(stderr, "  TOTAL = %ld bytes (%f MBytes)\n", total,
        ((double)total) / (1024 * 1024));
    clang_disposeCXTUResourceUsage(usage);
}

/******************************************************************************/
/* Logic for testing traversal.                                               */
/******************************************************************************/

static void PrintCursorExtent(CXCursor C) {
    CXSourceRange extent = clang_getCursorExtent(C);
    PrintRange(extent, "Extent");
}

/* Data used by the visitors. */
typedef struct {
    CXTranslationUnit TU;
    enum CXCursorKind* Filter;
    const char* CommentSchemaFile;
} VisitorData;


enum CXChildVisitResult FilteredPrintingVisitor(CXCursor Cursor,
    CXCursor Parent,
    CXClientData ClientData) {
    VisitorData* Data = (VisitorData*)ClientData;
    if (!Data->Filter || (Cursor.kind == *(enum CXCursorKind*)Data->Filter)) {
        CXSourceLocation Loc = clang_getCursorLocation(Cursor);
        unsigned line, column;
        clang_getSpellingLocation(Loc, 0, &line, &column, 0);
        printf("// %s: %s:%d:%d: ", FileCheckPrefix,
            GetCursorSource(Cursor), line, column);
        PrintCursor(Cursor, Data->CommentSchemaFile);
        PrintCursorExtent(Cursor);
        if (clang_isDeclaration(Cursor.kind)) {
            enum CX_CXXAccessSpecifier access = clang_getCXXAccessSpecifier(Cursor);
            const char* accessStr = 0;

            switch (access) {
            case CX_CXXInvalidAccessSpecifier: break;
            case CX_CXXPublic:
                accessStr = "public"; break;
            case CX_CXXProtected:
                accessStr = "protected"; break;
            case CX_CXXPrivate:
                accessStr = "private"; break;
            }

            if (accessStr)
                printf(" [access=%s]", accessStr);
        }
        printf("\n");
        return CXChildVisit_Recurse;
    }

    return CXChildVisit_Continue;
}

static enum CXChildVisitResult FunctionScanVisitor(CXCursor Cursor,
    CXCursor Parent,
    CXClientData ClientData) {
    const char* startBuf, * endBuf;
    unsigned startLine, startColumn, endLine, endColumn, curLine, curColumn;
    CXCursor Ref;
    VisitorData* Data = (VisitorData*)ClientData;

    if (Cursor.kind != CXCursor_FunctionDecl ||
        !clang_isCursorDefinition(Cursor))
        return CXChildVisit_Continue;

    clang_getDefinitionSpellingAndExtent(Cursor, &startBuf, &endBuf,
        &startLine, &startColumn,
        &endLine, &endColumn);
    /* Probe the entire body, looking for both decls and refs. */
    curLine = startLine;
    curColumn = startColumn;

    while (startBuf < endBuf) {
        CXSourceLocation Loc;
        CXFile file;
        CXString source;

        if (*startBuf == '\n') {
            startBuf++;
            curLine++;
            curColumn = 1;
        }
        else if (*startBuf != '\t')
            curColumn++;

        Loc = clang_getCursorLocation(Cursor);
        clang_getSpellingLocation(Loc, &file, 0, 0, 0);

        source = clang_getFileName(file);
        if (clang_getCString(source)) {
            CXSourceLocation RefLoc
                = clang_getLocation(Data->TU, file, curLine, curColumn);
            Ref = clang_getCursor(Data->TU, RefLoc);
            if (Ref.kind == CXCursor_NoDeclFound) {
                /* Nothing found here; that's fine. */
            }
            else if (Ref.kind != CXCursor_FunctionDecl) {
                printf("// %s: %s:%d:%d: ", FileCheckPrefix, GetCursorSource(Ref),
                    curLine, curColumn);
                PrintCursor(Ref, Data->CommentSchemaFile);
                printf("\n");
            }
        }
        clang_disposeString(source);
        startBuf++;
    }

    return CXChildVisit_Continue;
}

/******************************************************************************/
/* USR testing.                                                               */
/******************************************************************************/

enum CXChildVisitResult USRVisitor(CXCursor C, CXCursor parent,
    CXClientData ClientData) {
    VisitorData* Data = (VisitorData*)ClientData;
    if (!Data->Filter || (C.kind == *(enum CXCursorKind*)Data->Filter)) {
        CXString USR = clang_getCursorUSR(C);
        const char* cstr = clang_getCString(USR);
        if (!cstr || cstr[0] == '\0') {
            clang_disposeString(USR);
            return CXChildVisit_Recurse;
        }
        printf("// %s: %s %s", FileCheckPrefix, GetCursorSource(C), cstr);

        PrintCursorExtent(C);
        printf("\n");
        clang_disposeString(USR);

        return CXChildVisit_Recurse;
    }

    return CXChildVisit_Continue;
}

/******************************************************************************/
/* Inclusion stack testing.                                                   */
/******************************************************************************/

void InclusionVisitor(CXFile includedFile, CXSourceLocation* includeStack,
    unsigned includeStackLen, CXClientData data) {

    unsigned i;
    CXString fname;

    fname = clang_getFileName(includedFile);
    printf("file: %s\nincluded by:\n", clang_getCString(fname));
    clang_disposeString(fname);

    for (i = 0; i < includeStackLen; ++i) {
        CXFile includingFile;
        unsigned line, column;
        clang_getSpellingLocation(includeStack[i], &includingFile, &line,
            &column, 0);
        fname = clang_getFileName(includingFile);
        printf("  %s:%d:%d\n", clang_getCString(fname), line, column);
        clang_disposeString(fname);
    }
    printf("\n");
}

void PrintInclusionStack(CXTranslationUnit TU) {
    clang_getInclusions(TU, InclusionVisitor, NULL);
}

/******************************************************************************/
/* Linkage testing.                                                           */
/******************************************************************************/

static enum CXChildVisitResult PrintLinkage(CXCursor cursor, CXCursor p,
    CXClientData d) {
    const char* linkage = 0;

    if (clang_isInvalid(clang_getCursorKind(cursor)))
        return CXChildVisit_Recurse;

    switch (clang_getCursorLinkage(cursor)) {
    case CXLinkage_Invalid: break;
    case CXLinkage_NoLinkage: linkage = "NoLinkage"; break;
    case CXLinkage_Internal: linkage = "Internal"; break;
    case CXLinkage_UniqueExternal: linkage = "UniqueExternal"; break;
    case CXLinkage_External: linkage = "External"; break;
    }

    if (linkage) {
        PrintCursor(cursor, NULL);
        printf("linkage=%s\n", linkage);
    }

    return CXChildVisit_Recurse;
}

/******************************************************************************/
/* Visibility testing.                                                        */
/******************************************************************************/

static enum CXChildVisitResult PrintVisibility(CXCursor cursor, CXCursor p,
    CXClientData d) {
    const char* visibility = 0;

    if (clang_isInvalid(clang_getCursorKind(cursor)))
        return CXChildVisit_Recurse;

    switch (clang_getCursorVisibility(cursor)) {
    case CXVisibility_Invalid: break;
    case CXVisibility_Hidden: visibility = "Hidden"; break;
    case CXVisibility_Protected: visibility = "Protected"; break;
    case CXVisibility_Default: visibility = "Default"; break;
    }

    if (visibility) {
        PrintCursor(cursor, NULL);
        printf("visibility=%s\n", visibility);
    }

    return CXChildVisit_Recurse;
}

/******************************************************************************/
/* Typekind testing.                                                          */
/******************************************************************************/

static void PrintTypeAndTypeKind(CXType T, const char* Format) {
    CXString TypeSpelling, TypeKindSpelling;

    TypeSpelling = clang_getTypeSpelling(T);
    TypeKindSpelling = clang_getTypeKindSpelling(T.kind);
    printf(Format,
        clang_getCString(TypeSpelling),
        clang_getCString(TypeKindSpelling));
    clang_disposeString(TypeSpelling);
    clang_disposeString(TypeKindSpelling);
}

static enum CXVisitorResult FieldVisitor(CXCursor C,
    CXClientData client_data) {
    (*(int*)client_data) += 1;
    return CXVisit_Continue;
}

static void PrintTypeTemplateArgs(CXType T, const char* Format) {
    int NumTArgs = clang_Type_getNumTemplateArguments(T);
    if (NumTArgs != -1 && NumTArgs != 0) {
        int i;
        CXType TArg;
        printf(Format, NumTArgs);
        for (i = 0; i < NumTArgs; ++i) {
            TArg = clang_Type_getTemplateArgumentAsType(T, i);
            if (TArg.kind != CXType_Invalid) {
                PrintTypeAndTypeKind(TArg, " [type=%s] [typekind=%s]");
            }
        }
        /* Ensure that the returned type is invalid when indexing off-by-one. */
        TArg = clang_Type_getTemplateArgumentAsType(T, i);
        assert(TArg.kind == CXType_Invalid);
        printf("]");
    }
}

static void PrintNullabilityKind(CXType T, const char* Format) {
    enum CXTypeNullabilityKind N = clang_Type_getNullability(T);

    const char* nullability = 0;
    switch (N) {
    case CXTypeNullability_NonNull:
        nullability = "nonnull";
        break;
    case CXTypeNullability_Nullable:
        nullability = "nullable";
        break;
    case CXTypeNullability_NullableResult:
        nullability = "nullable_result";
        break;
    case CXTypeNullability_Unspecified:
        nullability = "unspecified";
        break;
    case CXTypeNullability_Invalid:
        break;
    }

    if (nullability) {
        printf(Format, nullability);
    }
}

static enum CXChildVisitResult PrintType(CXCursor cursor, CXCursor p,
    CXClientData d) {
    if (!clang_isInvalid(clang_getCursorKind(cursor))) {
        CXType T = clang_getCursorType(cursor);
        CXType PT = clang_getPointeeType(T);
        enum CXRefQualifierKind RQ = clang_Type_getCXXRefQualifier(T);
        PrintCursor(cursor, NULL);
        PrintTypeAndTypeKind(T, " [type=%s] [typekind=%s]");
        PrintNullabilityKind(T, " [nullability=%s]");
        if (clang_isConstQualifiedType(T))
            printf(" const");
        if (clang_isVolatileQualifiedType(T))
            printf(" volatile");
        if (clang_isRestrictQualifiedType(T))
            printf(" restrict");
        if (RQ == CXRefQualifier_LValue)
            printf(" lvalue-ref-qualifier");
        if (RQ == CXRefQualifier_RValue)
            printf(" rvalue-ref-qualifier");
        /* Print the template argument types if they exist. */
        PrintTypeTemplateArgs(T, " [templateargs/%d=");
        /* Print the canonical type if it is different. */
        {
            CXType CT = clang_getCanonicalType(T);
            if (!clang_equalTypes(T, CT)) {
                PrintTypeAndTypeKind(CT, " [canonicaltype=%s] [canonicaltypekind=%s]");
                PrintTypeTemplateArgs(CT, " [canonicaltemplateargs/%d=");
            }
        }
        /* Print the value type if it exists. */
        {
            CXType VT = clang_Type_getValueType(T);
            if (VT.kind != CXType_Invalid)
                PrintTypeAndTypeKind(VT, " [valuetype=%s] [valuetypekind=%s]");
        }
        /* Print the modified type if it exists. */
        {
            CXType MT = clang_Type_getModifiedType(T);
            if (MT.kind != CXType_Invalid) {
                PrintTypeAndTypeKind(MT, " [modifiedtype=%s] [modifiedtypekind=%s]");
            }
        }
        /* Print the return type if it exists. */
        {
            CXType RT = clang_getCursorResultType(cursor);
            if (RT.kind != CXType_Invalid) {
                PrintTypeAndTypeKind(RT, " [resulttype=%s] [resulttypekind=%s]");
            }
            PrintNullabilityKind(RT, " [resultnullability=%s]");
        }
        /* Print the argument types if they exist. */
        {
            int NumArgs = clang_Cursor_getNumArguments(cursor);
            if (NumArgs != -1 && NumArgs != 0) {
                int i;
                printf(" [args=");
                for (i = 0; i < NumArgs; ++i) {
                    CXType T = clang_getCursorType(clang_Cursor_getArgument(cursor, i));
                    if (T.kind != CXType_Invalid) {
                        PrintTypeAndTypeKind(T, " [%s] [%s]");
                        PrintNullabilityKind(T, " [%s]");
                    }
                }
                printf("]");
            }
        }
        /* Print ObjC base types, type arguments, and protocol list if available. */
        {
            CXType BT = clang_Type_getObjCObjectBaseType(PT);
            if (BT.kind != CXType_Invalid) {
                PrintTypeAndTypeKind(BT, " [basetype=%s] [basekind=%s]");
            }
        }
        {
            unsigned NumTypeArgs = clang_Type_getNumObjCTypeArgs(PT);
            if (NumTypeArgs > 0) {
                unsigned i;
                printf(" [typeargs=");
                for (i = 0; i < NumTypeArgs; ++i) {
                    CXType TA = clang_Type_getObjCTypeArg(PT, i);
                    if (TA.kind != CXType_Invalid) {
                        PrintTypeAndTypeKind(TA, " [%s] [%s]");
                    }
                }
                printf("]");
            }
        }
        {
            unsigned NumProtocols = clang_Type_getNumObjCProtocolRefs(PT);
            if (NumProtocols > 0) {
                unsigned i;
                printf(" [protocols=");
                for (i = 0; i < NumProtocols; ++i) {
                    CXCursor P = clang_Type_getObjCProtocolDecl(PT, i);
                    if (!clang_isInvalid(clang_getCursorKind(P))) {
                        PrintCursor(P, NULL);
                    }
                }
                printf("]");
            }
        }
        /* Print if this is a non-POD type. */
        printf(" [isPOD=%d]", clang_isPODType(T));
        /* Print the pointee type. */
        {
            if (PT.kind != CXType_Invalid) {
                PrintTypeAndTypeKind(PT, " [pointeetype=%s] [pointeekind=%s]");
            }
        }
        /* Print the number of fields if they exist. */
        {
            int numFields = 0;
            if (clang_Type_visitFields(T, FieldVisitor, &numFields)) {
                if (numFields != 0) {
                    printf(" [nbFields=%d]", numFields);
                }
            }
        }

        /* Print if it is an anonymous record or namespace. */
        {
            unsigned isAnon = clang_Cursor_isAnonymous(cursor);
            if (isAnon != 0) {
                printf(" [isAnon=%d]", isAnon);
            }
        }

        /* Print if it is an anonymous record decl */
        {
            unsigned isAnonRecDecl = clang_Cursor_isAnonymousRecordDecl(cursor);
            printf(" [isAnonRecDecl=%d]", isAnonRecDecl);
        }

        /* Print if it is an inline namespace decl */
        {
            unsigned isInlineNamespace = clang_Cursor_isInlineNamespace(cursor);
            if (isInlineNamespace != 0)
                printf(" [isInlineNamespace=%d]", isInlineNamespace);
        }

        printf("\n");
    }
    return CXChildVisit_Recurse;
}

static void PrintSingleTypeSize(CXType T, const char* TypeKindFormat,
    const char* SizeFormat,
    const char* AlignFormat) {
    PrintTypeAndTypeKind(T, TypeKindFormat);
    /* Print the type sizeof if applicable. */
    {
        long long Size = clang_Type_getSizeOf(T);
        if (Size >= 0 || Size < -1) {
            printf(SizeFormat, Size);
        }
    }
    /* Print the type alignof if applicable. */
    {
        long long Align = clang_Type_getAlignOf(T);
        if (Align >= 0 || Align < -1) {
            printf(AlignFormat, Align);
        }
    }

    /* Print the return type if it exists. */
    {
        CXType RT = clang_getResultType(T);
        if (RT.kind != CXType_Invalid)
            PrintSingleTypeSize(RT, " [resulttype=%s] [resulttypekind=%s]",
                " [resultsizeof=%lld]", " [resultalignof=%lld]");
    }
}

static enum CXChildVisitResult PrintTypeSize(CXCursor cursor, CXCursor p,
    CXClientData d) {
    CXType T;
    enum CXCursorKind K = clang_getCursorKind(cursor);
    if (clang_isInvalid(K))
        return CXChildVisit_Recurse;
    T = clang_getCursorType(cursor);
    PrintCursor(cursor, NULL);
    PrintSingleTypeSize(T, " [type=%s] [typekind=%s]", " [sizeof=%lld]",
        " [alignof=%lld]");
    /* Print the record field offset if applicable. */
    {
        CXString FieldSpelling = clang_getCursorSpelling(cursor);
        const char* FieldName = clang_getCString(FieldSpelling);
        /* recurse to get the first parent record that is not anonymous. */
        unsigned RecordIsAnonymous = 0;
        if (clang_getCursorKind(cursor) == CXCursor_FieldDecl) {
            CXCursor Record;
            CXCursor Parent = p;
            do {
                Record = Parent;
                Parent = clang_getCursorSemanticParent(Record);
                RecordIsAnonymous = clang_Cursor_isAnonymous(Record);
                /* Recurse as long as the parent is a CXType_Record and the Record
                   is anonymous */
            } while (clang_getCursorType(Parent).kind == CXType_Record &&
                RecordIsAnonymous > 0);
            {
                long long Offset = clang_Type_getOffsetOf(clang_getCursorType(Record),
                    FieldName);
                long long Offset2 = clang_Cursor_getOffsetOfField(cursor);
                if (Offset == Offset2) {
                    printf(" [offsetof=%lld]", Offset);
                }
                else {
                    /* Offsets will be different in anonymous records. */
                    printf(" [offsetof=%lld/%lld]", Offset, Offset2);
                }
            }
        }
        clang_disposeString(FieldSpelling);
    }
    /* Print if its a bitfield */
    {
        int IsBitfield = clang_Cursor_isBitField(cursor);
        if (IsBitfield)
            printf(" [BitFieldSize=%d]", clang_getFieldDeclBitWidth(cursor));
    }

    printf("\n");

    return CXChildVisit_Recurse;
}

/******************************************************************************/
/* Mangling testing.                                                          */
/******************************************************************************/

static enum CXChildVisitResult PrintMangledName(CXCursor cursor, CXCursor p,
    CXClientData d) {
    CXString MangledName;
    if (clang_isUnexposed(clang_getCursorKind(cursor)))
        return CXChildVisit_Recurse;
    PrintCursor(cursor, NULL);
    MangledName = clang_Cursor_getMangling(cursor);
    printf(" [mangled=%s]\n", clang_getCString(MangledName));
    clang_disposeString(MangledName);
    return CXChildVisit_Continue;
}

static int perform_test_load(CXIndex Idx, CXTranslationUnit TU,
    const char* filter, const char* prefix,
    CXCursorVisitor Visitor,
    PostVisitTU PV,
    const char* CommentSchemaFile) {

    if (prefix)
        FileCheckPrefix = prefix;

    if (Visitor) {
        enum CXCursorKind K = CXCursor_NotImplemented;
        enum CXCursorKind* ck = &K;
        VisitorData Data;

        /* Perform some simple filtering. */
        if (!strcmp(filter, "all") || !strcmp(filter, "local")) ck = NULL;
        else if (!strcmp(filter, "all-display") ||
            !strcmp(filter, "local-display")) {
            ck = NULL;
            wanted_display_type = DisplayType_DisplayName;
        }
        else if (!strcmp(filter, "all-pretty") ||
            !strcmp(filter, "local-pretty")) {
            ck = NULL;
            wanted_display_type = DisplayType_Pretty;
        }
        else if (!strcmp(filter, "none")) K = (enum CXCursorKind)~0;
        else if (!strcmp(filter, "category")) K = CXCursor_ObjCCategoryDecl;
        else if (!strcmp(filter, "interface")) K = CXCursor_ObjCInterfaceDecl;
        else if (!strcmp(filter, "protocol")) K = CXCursor_ObjCProtocolDecl;
        else if (!strcmp(filter, "function")) K = CXCursor_FunctionDecl;
        else if (!strcmp(filter, "typedef")) K = CXCursor_TypedefDecl;
        else if (!strcmp(filter, "scan-function")) Visitor = FunctionScanVisitor;
        else {
            fprintf(stderr, "Unknown filter for -test-load-tu: %s\n", filter);
            return 1;
        }

        Data.TU = TU;
        Data.Filter = ck;
        Data.CommentSchemaFile = CommentSchemaFile;
        clang_visitChildren(clang_getTranslationUnitCursor(TU), Visitor, &Data);
    }

    if (PV)
        PV(TU);

    PrintDiagnostics(TU);
    if (checkForErrors(TU) != 0) {
        clang_disposeTranslationUnit(TU);
        return -1;
    }

    clang_disposeTranslationUnit(TU);
    return 0;
}

int perform_test_load_tu(const char* file, const char* filter,
    const char* prefix, CXCursorVisitor Visitor,
    PostVisitTU PV) {
    CXIndex Idx;
    CXTranslationUnit TU;
    int result;
    Idx = clang_createIndex(/* excludeDeclsFromPCH */
        !strcmp(filter, "local") ? 1 : 0,
        /* displayDiagnostics=*/1);

    if (!CreateTranslationUnit(Idx, file, &TU)) {
        clang_disposeIndex(Idx);
        return 1;
    }

    result = perform_test_load(Idx, TU, filter, prefix, Visitor, PV, NULL);
    clang_disposeIndex(Idx);
    return result;
}

int perform_test_load_source(int argc, const char** argv,
    const char* filter, CXCursorVisitor Visitor,
    PostVisitTU PV) {
    CXIndex Idx;
    CXTranslationUnit TU;
    const char* CommentSchemaFile;
    struct CXUnsavedFile* unsaved_files = 0;
    int num_unsaved_files = 0;
    enum CXErrorCode Err;
    int result;
    unsigned Repeats = 0;
    unsigned I;

    Idx =
        createIndexWithInvocationEmissionPath(/* excludeDeclsFromPCH */
            (!strcmp(filter, "local") ||
                !strcmp(filter, "local-display") ||
                !strcmp(filter, "local-pretty"))
            ? 1
            : 0,
            /* displayDiagnostics=*/1);
    if (!Idx)
        return -1;

    if ((CommentSchemaFile = parse_comments_schema(argc, argv))) {
        argc--;
        argv++;
    }

    if (parse_remapped_files(argc, argv, 0, &unsaved_files, &num_unsaved_files)) {
        clang_disposeIndex(Idx);
        return -1;
    }

    if (getenv("CINDEXTEST_EDITING"))
        Repeats = 5;

    //options |= CXTranslationUnit_IncludeAttributedTypes;
    //if (getenv("CINDEXTEST_VISIT_IMPLICIT_ATTRIBUTES"))
    //    options |= CXTranslationUnit_VisitImplicitAttributes;
    //if (getenv("CINDEXTEST_IGNORE_NONERRORS_FROM_INCLUDED_FILES"))
    //    options |= CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles;
    Err = clang_parseTranslationUnit2(Idx, 0,
        argv + num_unsaved_files,
        argc - num_unsaved_files,
        unsaved_files, num_unsaved_files,
        //getDefaultParsingOptions(),
        CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_Incomplete,
        &TU);
    if (Err != CXError_Success) {
        fprintf(stderr, "Unable to load translation unit!\n");
        describeLibclangFailure(Err);
        free_remapped_files(unsaved_files, num_unsaved_files);
        clang_disposeIndex(Idx);
        return 1;
    }

    for (I = 0; I != Repeats; ++I) {
        if (checkForErrors(TU) != 0)
            return -1;

        if (Repeats > 1) {
            clang_suspendTranslationUnit(TU);

            Err = static_cast<CXErrorCode>(clang_reparseTranslationUnit(TU, num_unsaved_files, unsaved_files,
                clang_defaultReparseOptions(TU)));
            if (Err != CXError_Success) {
                describeLibclangFailure(Err);
                free_remapped_files(unsaved_files, num_unsaved_files);
                clang_disposeIndex(Idx);
                return 1;
            }
        }
    }

    result = perform_test_load(Idx, TU, filter, NULL, Visitor, PV,
        CommentSchemaFile);
    free_remapped_files(unsaved_files, num_unsaved_files);
    clang_disposeIndex(Idx);
    return result;
}

static int checkForErrors(CXTranslationUnit TU) {
    unsigned Num, i;
    CXDiagnostic Diag;
    CXString DiagStr;

    if (!getenv("CINDEXTEST_FAILONERROR"))
        return 0;

    Num = clang_getNumDiagnostics(TU);
    for (i = 0; i != Num; ++i) {
        Diag = clang_getDiagnostic(TU, i);
        if (clang_getDiagnosticSeverity(Diag) >= CXDiagnostic_Error) {
            DiagStr = clang_formatDiagnostic(Diag,
                clang_defaultDiagnosticDisplayOptions());
            fprintf(stderr, "%s\n", clang_getCString(DiagStr));
            clang_disposeString(DiagStr);
            clang_disposeDiagnostic(Diag);
            return -1;
        }
        clang_disposeDiagnostic(Diag);
    }

    return 0;
}
#pragma warning( default : 4996 )

void TestParseCpp(const std::vector<std::string>& vecIncludeBasePath, const char* pszCppFilePath)
{
    std::vector<std::string> vecArg;
    vecArg.push_back(pszCppFilePath);
    vecArg.push_back("-x");
    vecArg.push_back("c++");
    vecArg.push_back("-std=c++14");
    vecArg.push_back("-w");
    vecArg.push_back("-MG");
    vecArg.push_back("-M");
    vecArg.push_back("-ferror-limit=0");
    vecArg.push_back("-o clangLog.txt");
    for (auto& it : vecIncludeBasePath)
        vecArg.push_back("-I" + it);
    //_putenv("VCToolsInstallDir=ItsNotAVCToolsInstallDir");//屏蔽stl搜索路径
    std::vector<const char*> arguments;
    for (auto& it : vecArg)
        arguments.push_back(&it[0]);

    perform_test_load_source(static_cast<int>(arguments.size()), arguments.data(), "all", PrintType, 0);
}

void Test0()
{
    //begin, PicoloParser用的参数
    //-x
    //c++
    //-std=c++11
    //-D__REFLECTION_PARSER__
    //-DNDEBUG
    //-D__clang__
    //-w
    //-MG
    //-M
    //-ferror-limit=0
    //-o clangLog.txt
    //-IF:/Fts/Proj/Test/Piccolo/engine/source
    //end


 //   std::vector<std::string> vecArg;
 //   vecArg.push_back("-x");
 //   vecArg.push_back("c++");
 //   vecArg.push_back("-std=c++14");
 //   vecArg.push_back("-w");
 //   vecArg.push_back("-MG");
 //   vecArg.push_back("-M");
 //   vecArg.push_back("-ferror-limit=0");
 //   vecArg.push_back("-o clangLog.txt");
 //   vecArg.push_back("-I../../CLangExp");
 //   std::vector<const char*> arguments;
 //   for (auto& it : vecArg)
 //       arguments.push_back(&it[0]);
 //   
    //auto m_index = clang_createIndex(true, true);
 //   auto m_translation_unit = clang_createTranslationUnitFromSourceFile(
 //       m_index,
 //       "../../CLangExp/Exp/Exp.cpp",
 //       static_cast<int>(arguments.size()),
 //       arguments.data(),
 //       0,
 //       nullptr
 //   );

 //   CXCursorVisitor Visitor = PrintType;
 //   clang_visitChildren(clang_getTranslationUnitCursor(m_translation_unit), Visitor, 0);
 //   clang_disposeTranslationUnit(m_translation_unit);

    //clang_disposeIndex(m_index);

    std::vector<std::string> vecInclude;
#define DEVELOPMENT_ROOT_SOURCE_PATH "SSSSSSSSSSSSSSSSSSS"
    vecInclude.push_back(CONCAT_HARDCODED_STRINGS_2(DEVELOPMENT_ROOT_SOURCE_PATH, "/Tools/Niflect"));
    TestParseCpp(vecInclude, CONCAT_HARDCODED_STRINGS_2(DEVELOPMENT_ROOT_SOURCE_PATH, "/Tools/Niflect/NiflectGen/Test/Exp/Exp.cpp"));
    printf("");
}