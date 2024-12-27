#include "NiflectGen/Collector/TaggedNode.h"
#include <cstring>
#include "Niflect/Util/StringUtil.h"
#include "Niflect/Util/DebugUtil.h"
#include "NiflectGen/Util/CursorUtil.h"
#include "NiflectGen/Log/Log.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"

namespace NiflectGen
{
	using namespace Niflect;

	bool CXSourceLocationComp::operator()(const CXSourceLocation& lhs, const CXSourceLocation& rhs) const
	{
		//// Use Clang API to compare source locations. Assume `isEqual` returns
		//// true if the two source locations are equal.
		//if (clang_equalLocations(lhs, rhs)) {
		//	return false; // Equal locations, so lhs is not less than rhs
		//}

		// Compare the source locations by their respective data.
		// clang_getExpansionLocation could be used to get file, line, column, and offset data.
		CXFile lhs_file, rhs_file;
		unsigned lhs_line, lhs_column, lhs_offset;
		unsigned rhs_line, rhs_column, rhs_offset;

		//��ֻ����macroExpansion��cursor����, ����ȷ���Ƿ��б�Ҫ��clang_getExpansionLocation, 
		//clang_getExpansionLocation(lhs, &lhs_file, &lhs_line, &lhs_column, &lhs_offset);
		//clang_getExpansionLocation(rhs, &rhs_file, &rhs_line, &rhs_column, &rhs_offset);
		clang_getSpellingLocation(lhs, &lhs_file, &lhs_line, &lhs_column, &lhs_offset);
		clang_getSpellingLocation(rhs, &rhs_file, &rhs_line, &rhs_column, &rhs_offset);

		// Prioritize by file, then line, then column, then offset
		if (lhs_file != rhs_file) {
			return lhs_file < rhs_file;
		}
		else if (lhs_line != rhs_line) {
			return lhs_line < rhs_line;
		}
		else if (lhs_column != rhs_column) {
			return lhs_column < rhs_column;
		}
		return lhs_offset < rhs_offset;
	}

	CMacroTagCollection2::CMacroTagCollection2()
	{

	}
	CMacroTagCollection2::~CMacroTagCollection2()
	{
	}
#ifdef SIMPLIFIED_MACRO_CURSOR_FINDING
	bool CMacroTagCollection2::PushMacroExpansion(CXCursorKind kind, const CXCursor& cursor)
	{
		if (kind == CXCursor_MacroExpansion)
		{
			m_stkMacroCursor.push_back(cursor);
			return true;
		}
		return false;
	}
	void CMacroTagCollection2::PopMacroExpansion(CXCursor& macroCursor)
	{
		auto it = m_stkMacroCursor.begin();
		it += m_stkMacroCursor.size() - 1;
		macroCursor = *it;
		m_stkMacroCursor.erase(it);
	}
#else
	bool CMacroTagCollection2::CollectMacroExpansion(const CXCursor& cursor)
	{
		auto kind = clang_getCursorKind(cursor);
		if (kind == CXCursor_MacroExpansion)
		{
			auto loc = clang_getCursorLocation(cursor);
			auto ret = m_mapLocationToMacroCursor.insert({ loc, cursor });
			if (ret.second)
			{
			}
			else
			{
				ASSERT(false);//��Ӧ�����ظ�loc
			}
			return true;
		}
		return false;
	}
	void CMacroTagCollection2::TakeByTagLocation(const CXSourceLocation& tagLocation, CXCursor& macroCursor)
	{
		auto itFound = m_mapLocationToMacroCursor.find(tagLocation);
		if (itFound != m_mapLocationToMacroCursor.end())
		{
			macroCursor = itFound->second;
			m_mapLocationToMacroCursor.erase(itFound);
		}
		else
		{
			ASSERT(false);
		}
	}
#endif

	CTaggedNode2::CTaggedNode2()
		: m_cursor(g_invalidCursor)
		, m_macroCursor(g_invalidCursor)
	{

	}
	void CTaggedNode2::Init(const CXCursor& cursor)
	{
		m_cursor = cursor;
	}
	void CTaggedNode2::InitMacroExpansionCursor(const CXCursor& cursor)
	{
		m_macroCursor = cursor;
	}

	class CMacroExpansionNataData
	{
	public:
		Niflect::CString m_nataCode;
	};

	bool ssssssssssss(const CXCursor& cursor, CMacroExpansionNataData& data)
	{
		CXSourceRange range = clang_getCursorExtent(cursor);
		CXTranslationUnit translationUnit = clang_Cursor_getTranslationUnit(cursor);

		bool containsIdentifier = true;
		{
			//��ȡ�޻��е������ַ�
			CXToken* tokens = nullptr;
			unsigned int numTokens = 0;
			clang_tokenize(translationUnit, range, &tokens, &numTokens);
			uint32 identifierKindsIdx = 0;
			for (unsigned idx = 0; idx < numTokens; ++idx)
			{
				auto& token = tokens[idx];
				auto kind = clang_getTokenKind(token);
				if (kind == CXToken_Identifier)
				{
					identifierKindsIdx++;
					if (identifierKindsIdx == NiflectGenDefinition::NiflectFramework::MacroNata::IdentifierTokenFindingEndCount)
					{
						//����¼���Ƶ�ԭ��: 
						//1. ͨ���ַ���ʽ���������ҳ� Cursor ��ζ����Ҫ��¼���� Cursor
						//2. ������ Nata ���ṩ��ʽ��Ϊ����������ʽ, ��������������ѡ����
						auto nataIdentifier = CXStringToCString(clang_getTokenSpelling(translationUnit, token));
						containsIdentifier = true;
						break;
					}
				}
			}
			clang_disposeTokens(translationUnit, tokens, numTokens);
		}

		{
			CXFile begin_file, end_file;
			unsigned begin_line, begin_column, end_line, end_column, begin_offset, end_offset;
			clang_getSpellingLocation(clang_getRangeStart(range),
				&begin_file, &begin_line, &begin_column, &begin_offset);
			clang_getSpellingLocation(clang_getRangeEnd(range),
				&end_file, &end_line, &end_column, &end_offset);
			if (begin_file && end_file)
			{
				ASSERT(begin_file == end_file);
				ASSERT(end_offset > begin_offset);

				size_t size = 0;
				auto contents = clang_getFileContents(translationUnit, begin_file, &size);

				{
					for (uint32 idx = begin_offset; idx < size; ++idx)
					{
						if (contents[idx] == NiflectGenDefinition::NiflectFramework::MacroNata::TokensBrackets[0])
						{
							begin_offset = idx + 1;
							break;
						}
					}
					ASSERT(end_offset > begin_offset);
					for (uint32 idx = end_offset; idx >= begin_offset; --idx)
					{
						if (contents[idx] == NiflectGenDefinition::NiflectFramework::MacroNata::TokensBrackets[1])
						{
							end_offset = idx;
							break;
						}
					}
				}

				data.m_nataCode.resize(end_offset - begin_offset);
				memcpy(&data.m_nataCode[0], contents + begin_offset, data.m_nataCode.size());
			}
		}

		return containsIdentifier;
	}
	void WriteNataArgNullOrVar(const CCodeLines& linesNata, CCodeLines& linesCopy, Niflect::CString& nataNullOrVar)
	{
		nataNullOrVar = "NULL";
		if (linesNata.size() > 0)
		{
			nataNullOrVar = "nata";
			for (auto& it : linesNata)
				linesCopy.push_back(it);
		}
	}
	void CTaggedNode2::ResolveMacroNata(const SResolvingMacroNataContext& context)
	{
		CMacroExpansionNataData data;
		if (ssssssssssss(m_macroCursor, data))
		{
			ASSERT(m_linesRawNata.size() == 0);
			CStringStream ss;
			ss << data.m_nataCode;
			CString line;
			while (std::getline(ss, line))
				m_linesRawNata.push_back(line);
		}
		else
		{
			GenLogError(context.m_log, NiflectUtil::FormatString("%s, is an unsupported Nata specification. Nata can only be specifed through an invocation within the Macro Tag, such as using NIF_F(CMyNata().SetOption0(true))", data.m_nataCode.c_str()));
		}
	}
	void CTaggedNode2::AddChildAndInitDefault(const CSharedTaggedNode& taggedNode, const CXCursor& cursor, const CXCursor& macroCursor)
	{
		taggedNode->Init(cursor);
		taggedNode->InitMacroExpansionCursor(macroCursor);
		m_vecChild.push_back(taggedNode);
	}
	bool ExtractBuiltinMetadata(const Niflect::CString& keyword, const Niflect::CString& line, Niflect::CString& metadata, uint32& movedPos)
	{
		auto pos0 = line.find(keyword, movedPos);
		if (pos0 != std::string::npos)
		{
			pos0 += keyword.length();
			auto pos1 = line.find("=", pos0);
			if (pos1 != std::string::npos)
			{
				pos1 += 1;
				auto pos2 = line.find(";", pos1);
				if (pos2 == std::string::npos)
					pos2 = line.length();
				metadata = line.substr(pos1, pos2 - pos1);
				metadata = NiflectUtil::Trim(metadata);

				if (pos2 < line.length() - 1)
				{
					pos2 += 1;
					while (true)
					{
						auto& ch = line[pos2];
						if (ch == ' ')
							pos2++;
						else
							break;
					}
				}
				movedPos = static_cast<uint32>(pos2);
				return true;
			}
		}
		return false;
	}
	struct SAsignmentExprParsingInfo
	{
		const char* m_keyword;
		Niflect::CString* m_value;
	};
	static void ParseAsignmentExpression(const Niflect::CString& line, const Niflect::TArrayNif<SAsignmentExprParsingInfo>& vecInfo, Niflect::TArrayNif<Niflect::CString>& vecExtractedKeyword, uint32& maxMovedPos)
	{
		maxMovedPos = 0;
		bool anyFailed = false;
		for (auto& it0 : vecInfo)
		{
			uint32 movedPos = 0;
			if (ExtractBuiltinMetadata(it0.m_keyword, line, *it0.m_value, movedPos))
			{
				maxMovedPos = std::max(maxMovedPos, movedPos);
			}
			else
			{
				anyFailed = true;
			}
		}
		if (anyFailed)
		{
			size_t startPos = 0;
			while (true)
			{
				auto pos0 = line.find('=', startPos);
				if (pos0 != std::string::npos)
				{
					auto extractedKeyword = line.substr(startPos, pos0 - 1 - startPos);
					extractedKeyword = NiflectUtil::Trim(extractedKeyword);
					bool found = false;
					for (auto& it1 : vecInfo)
					{
						if (extractedKeyword == it1.m_keyword)
						{
							found = true;
							break;
						}
					}
					if (!found)
						vecExtractedKeyword.push_back(extractedKeyword);
					startPos = line.find(';', pos0 + 1);
					if (startPos != std::string::npos)
						startPos++;
				}
				else
				{
					break;
				}
			}
		}
	}
	void CTaggedNode2::WriteCopyNataCode(CCodeLines& linesCopy) const
	{
		if (m_linesRawNata.size() > 0)
		{
			Niflect::CString firstLine = NiflectUtil::FormatString("auto nata = %s(", NiflectGenDefinition::NiflectFramework::FuncName::CopyDerivedMeta);
			uint32 idx = 0;
			linesCopy.push_back(firstLine + m_linesRawNata[idx++]);
			for (; idx < m_linesRawNata.size(); ++idx)
				linesCopy.push_back(m_linesRawNata[idx]);
			linesCopy.back() += ");";
		}
	}
#ifdef PORTING_GETTER_SETTER_DEFAULTVALUE
	void CTaggedNode2::WriteCopyNataCodeExtractBuiltinMetadata(CCodeLines& linesCopy, CBuiltinMetadata& builtinMetadata, CGenLog* log) const
	{
		if (m_linesRawNata.size() > 0)
		{
			Niflect::CString code = NiflectUtil::FormatString("auto nata = %s(", NiflectGenDefinition::NiflectFramework::FuncName::CopyDerivedMeta);
			uint32 idx = 0;
			auto firstLine = m_linesRawNata[idx++];
			Niflect::TArrayNif<SAsignmentExprParsingInfo> vecInfo;
			vecInfo.push_back({ NiflectGenDefinition::NiflectFramework::BuiltinMetadata::Getter, &builtinMetadata.m_getterName });
			vecInfo.push_back({ NiflectGenDefinition::NiflectFramework::BuiltinMetadata::Setter, &builtinMetadata.m_setterName });
			vecInfo.push_back({ NiflectGenDefinition::NiflectFramework::BuiltinMetadata::DefaultValue, &builtinMetadata.m_defaultValue });
			uint32 movedPos = 0;
			Niflect::TArrayNif<Niflect::CString> vecExtractedKeyword;
			ParseAsignmentExpression(firstLine, vecInfo, vecExtractedKeyword, movedPos);
			if (vecExtractedKeyword.size() > 0)
			{
				Niflect::CString strUndef;
				for (uint32 idx1 = 0; idx1 < vecExtractedKeyword.size(); ++idx1)
				{
					strUndef += vecExtractedKeyword[idx1];
					if (idx1 != vecExtractedKeyword.size() - 1)
						strUndef += ';';
				}
				Niflect::CString strDef;
				for (uint32 idx1 = 0; idx1 < vecInfo.size(); ++idx1)
				{
					strDef += vecInfo[idx1].m_keyword;
					if (idx1 != vecInfo.size() - 1)
						strDef += ';';
				}
				GenLogError(log, NiflectUtil::FormatString("Undefined builtin metadata keyword: %s (Supported: %s), at: %s", strUndef.c_str(), strDef.c_str(), GetCursorFormattedLocationInfo(this->GetCursor()).c_str()));
			}
			if (movedPos > 0)
				firstLine = firstLine.substr(movedPos, firstLine.length() - movedPos);
			if (!firstLine.empty())
			{
				linesCopy.push_back(code + firstLine);
				for (; idx < m_linesRawNata.size(); ++idx)
					linesCopy.push_back(m_linesRawNata[idx]);
				linesCopy.back() += ");";
			}
		}
	}
#endif
	void CTaggedNode2::DebugPrint(FILE* fp, uint32 level) const
	{
		auto strLevel = NiflectUtil::DebugIndentToString(level);

		//if (IsCursorNeededFindingTaggedType(clang_getCursorKind(it.m_typeCursor)))
		{
			auto typeName = CXStringToCString(clang_getCursorSpelling(m_cursor));
			auto macroName = CXStringToCString(clang_getCursorSpelling(m_macroCursor));
			auto macroArgs = GetMacroExpansionTokensInString(m_macroCursor);

			//if (!clang_Cursor_isNull(it.m_typeCursor))
			{
				auto strCursorKind = CXStringToCString(clang_getCursorKindSpelling(clang_getCursorKind(m_cursor)));
				printf("%s%s, %s, %s, %s", strLevel.c_str(), strCursorKind.c_str(), typeName.c_str(), macroName.c_str(), macroArgs.c_str());
				this->DebugDerivedPrint(fp);
				printf("\n");
			}
		}
	}
}