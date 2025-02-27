#pragma once
#include "NiflectGen/CodeWriter/CodeWriter.h"
#include "Niflect/Util/FileStreamUtil.h"
#include <sstream>
#include <iostream>

namespace NiflectGen
{
	struct SCodeTemplateLabelCode
	{
		const Niflect::CString m_textCopy;
		const CCodeLines* m_pVecLine;
	};

	using CLabelToCodeMapping = Niflect::TMap<Niflect::CString, SCodeTemplateLabelCode>;

	class CCodeTemplate
	{
		struct SSection
		{
			Niflect::CString m_data;
			bool m_isLabel;
		};
		class CSectionLine
		{
		public:
			CSectionLine()
				: m_tabsCount(0)
			{

			}
			void AddLabelIndex(const Niflect::CString& data)
			{
				m_vecSection.push_back({ data, true });
			}
			void AddTextIndex(const Niflect::CString& data)
			{
				m_vecSection.push_back({ data, false });
			}
			uint32 m_tabsCount;
			Niflect::TArrayNif<SSection> m_vecSection;
		};

	public:
		CCodeTemplate()
			: m_labelPrefix("${")
			, m_labelPostfix("}")
			, m_tabChar('\t')
		{
		}
		void InitLabelMarks(const Niflect::CString& labelPrefix, const Niflect::CString& labelPostfix)
		{
			m_labelPrefix = labelPrefix;
			m_labelPostfix = labelPostfix;
		}
		void ReadFromFileStream2(const Niflect::CString& filePath)
		{
			std::ifstream ifstm;
			if (NiflectUtil::OpenFileStream(ifstm, filePath))
				this->ReadFromInputStream2(ifstm);
			else
				ASSERT(false);
		}
		void ReadFromRawData2(const char* psz)
		{
			std::stringstream ss1;
			ss1 << psz;
			this->ReadFromInputStream2(ss1);
		}
		void ReadFromInputStream2(std::istream& istm)
		{
			using namespace Niflect;
			CString line;
			uint32 lineNumber = 0;
			while (std::getline(istm, line))
				this->ParseLine(line, ++lineNumber);
		}
		void sssssssssssssssssssss(const SCodeTemplateLabelCode& code, const Niflect::CString& labelName, const CSectionLine& sectionLine, CCodeLines& vecLine, Niflect::TSet<Niflect::CString>* pSetReplacedLabel = NULL) const
		{
			if (code.m_pVecLine == NULL)
			{
				//ASSERT(!code.m_textCopy.empty());//有些标签值为空, 因此允许
				if (!code.m_textCopy.empty())
				{
					Niflect::CString line;
					for (uint32 idx1 = 0; idx1 < sectionLine.m_tabsCount; ++idx1)
						line += m_tabChar;
					line += code.m_textCopy;
					vecLine.emplace_back(line);

					if (pSetReplacedLabel != NULL)
						pSetReplacedLabel->insert(labelName);
				}
			}
			else
			{
				ASSERT(code.m_textCopy.empty());
				for (auto& it1 : *code.m_pVecLine)
				{
					Niflect::CString line;
					for (uint32 idx1 = 0; idx1 < sectionLine.m_tabsCount; ++idx1)
						line += m_tabChar;
					line += it1;
					vecLine.emplace_back(line);
				}

				if (pSetReplacedLabel != NULL)
					pSetReplacedLabel->insert(labelName);
			}
		}
		void ReplaceLabels(const CLabelToCodeMapping& mapLabelToCode, CCodeLines& vecLine, Niflect::TSet<Niflect::CString>* pSetReplacedLabel = NULL) const
		{
			using namespace Niflect;
			for (uint32 idx0 = 0; idx0 < m_vecSectionLine.size(); ++idx0)
			{
				auto& sectionLine = m_vecSectionLine[idx0];
				if (sectionLine.m_vecSection.size() == 1 && sectionLine.m_vecSection.back().m_isLabel)
				{
					const auto& labelName = sectionLine.m_vecSection.back().m_data;
					//auto itFound = mapLabelToCode.find(labelName);
					//if (itFound != mapLabelToCode.end())
					//{
					//	auto& code = itFound->second;
					//	if (code.m_pVecLine == NULL)
					//	{
					//		ASSERT(!code.m_textCopy.empty());
					//		CString line;
					//		for (uint32 idx1 = 0; idx1 < sectionLine.m_tabsCount; ++idx1)
					//			line += m_tabChar;
					//		line += code.m_textCopy;
					//		vecLine.emplace_back(line);
					//	}
					//	else
					//	{
					//		ASSERT(code.m_textCopy.empty());
					//		for (auto& it1 : *code.m_pVecLine)
					//		{
					//			CString line;
					//			for (uint32 idx1 = 0; idx1 < sectionLine.m_tabsCount; ++idx1)
					//				line += m_tabChar;
					//			line += it1;
					//			vecLine.emplace_back(line);
					//		}
					//	}

					//	if (pSetReplacedLabel != NULL)
					//		pSetReplacedLabel->insert(labelName);
					//}

					auto itFound = mapLabelToCode.find(labelName);
					if (itFound != mapLabelToCode.end())
					{
						auto& code = itFound->second;
						sssssssssssssssssssss(code, labelName, sectionLine, vecLine, pSetReplacedLabel);
					}
				}
				else
				{
					CString line;
					for (uint32 idx1 = 0; idx1 < sectionLine.m_tabsCount; ++idx1)
						line += m_tabChar;
					for (auto& it1 : sectionLine.m_vecSection)
					{
						if (it1.m_isLabel)
						{
							const auto& labelName = it1.m_data;
							//auto itFound = mapLabelToCode.find(labelName);
							//if (itFound != mapLabelToCode.end())
							//{
							//	auto& code = itFound->second;
							//	ASSERT(code.m_pVecLine == NULL);
							//	line += code.m_textCopy;

							//	if (pSetReplacedLabel != NULL)
							//		pSetReplacedLabel->insert(labelName);
							//}

							auto itFound = mapLabelToCode.find(labelName);
							if (itFound != mapLabelToCode.end())
							{
								auto& code = itFound->second;
								if (code.m_textCopy.empty())
								{
									sssssssssssssssssssss(code, labelName, sectionLine, vecLine, pSetReplacedLabel);
								}
								else
								{
									ASSERT(code.m_pVecLine == NULL);
									line += code.m_textCopy;

									if (pSetReplacedLabel != NULL)
										pSetReplacedLabel->insert(labelName);
								}
							}
						}
						else
						{
							line += it1.m_data;
						}
					}
					vecLine.emplace_back(line);
				}
			}
		}

	private:
		void ParseLine(const Niflect::CString& line, uint32 lineNumber)
		{
			using namespace Niflect;

			CSectionLine sections;

			std::size_t posPrecedingTab = 0;
			const auto lineLength = line.length();
			while (posPrecedingTab < lineLength)
			{
				if (line[posPrecedingTab] == m_tabChar)
					sections.m_tabsCount++;
				else
					break;
				posPrecedingTab++;
			}

			auto posLabelFinding = posPrecedingTab;
			while (true)
			{
				bool foundLabel = false;
				auto posTextBegin = posLabelFinding;
				auto posTextEnd = lineLength;
				auto posPrefix = line.find(m_labelPrefix, posLabelFinding);
				auto posPostfix = posPrefix;
				if (posPrefix != std::string::npos)
				{
					posPostfix = line.find(m_labelPostfix, posPrefix);
					if (posPostfix != std::string::npos)
					{
						posLabelFinding = posPostfix + m_labelPostfix.length();
						posTextEnd = posPrefix;
						foundLabel = true;
					}
					//else
					//{
					//	this->AddError(NiflectUtil::FormatString("Mismatching of label postfix %s at line %u", m_labelPostfix, lineNumber));//需要传入行号与标签后缀字符串
					//	break;
					//}
				}
				if (posTextEnd != posTextBegin)
					sections.AddTextIndex(line.substr(posTextBegin, posTextEnd - posTextBegin));
				if (foundLabel)
					sections.AddLabelIndex(line.substr(posPrefix + m_labelPrefix.length(), posPostfix - posPrefix - m_labelPrefix.length()));
				if (posTextEnd == lineLength)
					break;
			}

			m_vecSectionLine.emplace_back(sections);
		}

	private:
		void AddError(const Niflect::CString& text)
		{
			ASSERT(false);
			m_vecError.push_back(text);
		}

	public:
		void TestReconstruct(CCodeLines& vecLine)
		{
			using namespace Niflect;
			for (uint32 idx0 = 0; idx0 < m_vecSectionLine.size(); ++idx0)
			{
				auto& sectionLine = m_vecSectionLine[idx0];
				CString line;
				for (uint32 idx1 = 0; idx1 < sectionLine.m_tabsCount; ++idx1)
					line += m_tabChar;
				for (auto& it1 : sectionLine.m_vecSection)
				{
					if (it1.m_isLabel)
						line += m_labelPrefix + it1.m_data + m_labelPostfix;
					else
						line += it1.m_data;
				}
				vecLine.emplace_back(line);
			}
		}

	private:
		Niflect::CString m_labelPrefix;//如有需要改成可设置即可
		Niflect::CString m_labelPostfix;
		const char m_tabChar;
		Niflect::TArrayNif<CSectionLine> m_vecSectionLine;
		Niflect::TArrayNif<Niflect::CString> m_vecError;
	};

	static void MapLabelToText(CLabelToCodeMapping& map, const Niflect::CString& label, const Niflect::CString& text)
	{
		map.insert({ label, {text, NULL} });
	}
	static void MapLabelToLines(CLabelToCodeMapping& map, const Niflect::CString& label, const CCodeLines& vecLine)
	{
		using namespace Niflect;
		map.insert({ label, {CString(), &vecLine} });
	}
	static void MapAndReplaceLabelsToTexts1(CCodeTemplate& tpl1, CCodeLines& vecLine, const Niflect::CString& label0, const Niflect::CString& text0)
	{
		CLabelToCodeMapping map1;
		MapLabelToText(map1, label0, text0);
		tpl1.ReplaceLabels(map1, vecLine);
	}
	static void MapAndReplaceLabelsToTexts2(CCodeTemplate& tpl1, CCodeLines& vecLine, const Niflect::CString& label0, const Niflect::CString& label1, const Niflect::CString& text0, const Niflect::CString& text1)
	{
		CLabelToCodeMapping map1;
		MapLabelToText(map1, label0, text0);
		MapLabelToText(map1, label1, text1);
		tpl1.ReplaceLabels(map1, vecLine);
	}

	static void TestCodeTemplate0()
	{
		using namespace Niflect;
		CCodeTemplate tpl;
		//const char* pszFilePath = "F:/Fts/Proj/Test/Piccolo/engine/template/allSerializer.ipp.mustache";
		const char* pszFilePath = "E:/TestFunctionTemplate0.template";
		tpl.ReadFromFileStream2(pszFilePath);

		TArrayNif<CString> vecLine;
		tpl.TestReconstruct(vecLine);

		{
			const auto& vec0 = vecLine;

			std::ifstream ifstm;
			ifstm.open(pszFilePath, std::ios::in);
			CString line1;
			TArrayNif<CString> vec1;
			while (std::getline(ifstm, line1))
				vec1.push_back(line1);
			ASSERT(vec0 == vec1);
			printf("");
		}
	}
	static void TestCodeTemplate1()
	{
		using namespace Niflect;
		CCodeTemplate tpl0;
		const char* pszTemplateText0 =
			R"(void MyFunc0_${FuncName0}_${FuncName1}(float ${ParamName0})
{
	${DoSomething0}
	std::string str;
	str = "${Number0}";
	std::vector<float> ${ArrayVarName0};
	${ArrayVarName0}.push_back(${Number0});
	${ArrayOp0}
})";
		std::stringstream ss0;
		ss0 << pszTemplateText0;
		tpl0.ReadFromInputStream2(ss0);

		CLabelToCodeMapping map;
		MapLabelToText(map, "FuncName0", "MyFirstName");
		MapLabelToText(map, "FuncName1", "MySecondName");
		MapLabelToText(map, "ParamName0", "MyArg");
		CCodeLines vecLine3;
		vecLine3.push_back("//My comment");
		vecLine3.push_back("//in multiple lines");
		MapLabelToLines(map, "DoSomething0", vecLine3);
		MapLabelToText(map, "Number0", "123.4");
		const char* pszLabelArrayVar = "ArrayVarName0";
		const char* pszDataArrayVar = "vec";
		MapLabelToText(map, pszLabelArrayVar, pszDataArrayVar);
		const char* pszTemplateText1 = R"(${ArrayVarName0}.push_back(${Number});)";
		CCodeTemplate tpl1;
		tpl1.ReadFromRawData2(pszTemplateText1);
		CCodeLines vecLine6;
		const char* pszLabelNumber = "Number";
		MapAndReplaceLabelsToTexts2(tpl1, vecLine6, pszLabelArrayVar, pszLabelNumber, pszDataArrayVar, "1.1");
		MapAndReplaceLabelsToTexts2(tpl1, vecLine6, pszLabelArrayVar, pszLabelNumber, pszDataArrayVar, "1.2");
		MapLabelToLines(map, "ArrayOp0", vecLine6);

		CCodeLines vecFinal;
		tpl0.ReplaceLabels(map, vecFinal);
		for (auto& it : vecFinal)
			printf("%s\n", it.c_str());
		printf("");
	}
}