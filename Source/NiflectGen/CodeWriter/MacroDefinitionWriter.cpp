#include "NiflectGen/CodeWriter/MacroDefinitionWriter.h"
#include "Niflect/Util/StringUtil.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/HardCoded/MacroDefinitionTemplate.h"

namespace NiflectGen
{
    Niflect::CString GenerateLineNumberMacroName(const Niflect::CString& fileId, uint32 lineNumber, const Niflect::CString& namePostfix)
    {
        return NiflectUtil::FormatString("%s_%u_%s", fileId.c_str(), lineNumber, namePostfix.c_str());
    }
    void WriteLineNumberMacroDefinition(const Niflect::CString& macroName, const CCodeLines& linesBody, CCodeLines& linesDefinition)
    {
        CCodeTemplate tpl1;
        ReadTemplateFromRawData(tpl1, HardCodedTemplate::LineNumberMacroDefinition);
        CLabelToCodeMapping map;
        MapLabelToText(map, LABEL_7, macroName);
        MapLabelToLines(map, LABEL_8, linesBody);
        Niflect::TSet<Niflect::CString> setReplacedLabel;
        tpl1.ReplaceLabels(map, linesDefinition, &setReplacedLabel);
    }
}