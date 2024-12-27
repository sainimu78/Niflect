#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
	void ReadTemplateFromRawData(CCodeTemplate& tpl, const char* data)
	{
		tpl.InitLabelMarks(TEMPLATELABEL_L, TEMPLATELABEL_R);
		tpl.ReadFromRawData2(data);
	}
	void ReadTemplateFromInputStream(CCodeTemplate& tpl, std::istream& is)
	{
		tpl.InitLabelMarks(TEMPLATELABEL_L, TEMPLATELABEL_R);
		tpl.ReadFromInputStream2(is);
	}
	Niflect::CString ReplaceLabelToText1(const Niflect::CString& templateText, const Niflect::CString& label, const Niflect::CString& text)
	{
		CCodeTemplate tpl0;
		ReadTemplateFromRawData(tpl0, templateText.c_str());
		CLabelToCodeMapping map0;
		MapLabelToText(map0, label, text);
		CCodeLines lines0;
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl0.ReplaceLabels(map0, lines0, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map0.size());
		ASSERT(lines0.size() == 1);
		return lines0.back();
	}
	Niflect::CString ReplaceLabelToText2(const Niflect::CString& templateText, const Niflect::CString& label0, const Niflect::CString& label1, const Niflect::CString& text0, const Niflect::CString& text1)
	{
		CCodeTemplate tpl0;
		ReadTemplateFromRawData(tpl0, templateText.c_str());
		CLabelToCodeMapping map0;
		MapLabelToText(map0, label0, text0);
		MapLabelToText(map0, label1, text1);
		CCodeLines lines0;
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl0.ReplaceLabels(map0, lines0, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map0.size());
		ASSERT(lines0.size() == 1);
		return lines0.back();
	}
	void ReplaceLabelToLines1(const Niflect::CString& templateText, const Niflect::CString& label0, const Niflect::CString& text0, CCodeLines& linesResult)
	{
		CCodeTemplate tpl0;
		ReadTemplateFromRawData(tpl0, templateText.c_str());
		CLabelToCodeMapping map0;
		MapLabelToText(map0, label0, text0);
		CCodeLines lines0;
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl0.ReplaceLabels(map0, linesResult, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map0.size());
	}
	void ReplaceLabelToImplScopeLines(const CCodeLines& linesInScope, CCodeLines& vecLine)
	{
		const char* hct =
R"({
	)" MAKELABEL("L") R"(
})";
		CCodeTemplate tpl1;
		ReadTemplateFromRawData(tpl1, hct);
		CLabelToCodeMapping map;
		MapLabelToLines(map, "L", linesInScope);
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl1.ReplaceLabels(map, vecLine, &setReplacedLabel);
	}
	void WriteLinesIntoNamespaceScope(const Niflect::CString& namespaceName, const CCodeLines& linesInScope, CCodeLines& vecLine)
	{
		const char* hct =
"namespace " MAKELABEL("Name") R"(
{
	)" MAKELABEL("Lines") R"(
})";
		CCodeTemplate tpl1;
		ReadTemplateFromRawData(tpl1, hct);
		CLabelToCodeMapping map;
		MapLabelToText(map, "Name", namespaceName);
		MapLabelToLines(map, "Lines", linesInScope);
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl1.ReplaceLabels(map, vecLine, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
	}
}