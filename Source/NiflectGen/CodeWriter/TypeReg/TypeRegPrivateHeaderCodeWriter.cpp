#include "NiflectGen/CodeWriter/TypeReg/TypeRegPrivateHeaderCodeWriter.h"
#include "NiflectGen/CodeWriter/HardCoded/TypeRegPrivateHeaderTemplate.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/CppWriter.h"

namespace NiflectGen
{
	void WriteTypeRegsPrivateHeader(const CWritingContext& context, const STypeRegPrivateHeaderWritingContext& genCtx, STypeRegPrivateHeaderWritingData& data)
	{
		//todo: 收集同namespace, 同global的class定义

		CNoDupPathCollector includesNoDup;
		CNoDupPathCollector namespacesNoDup;
		CCodeLines linesRegClasses;
		for (auto& it0 : genCtx.m_vecWritingData)
		{
			for (auto& it1 : it0->m_linesRegClass)
				linesRegClasses.push_back(it1);
			for (auto& it1 : it0->m_privateHeaderincludePath.m_vecPath)
				includesNoDup.Add(it1);
			for (auto& it1 : it0->m_vecNamespaceRequirement)
				namespacesNoDup.Add(it1);
		}
		CCodeLines linesIncludes;
		ASSERT(false);
		//CIncludesHelper::Write(includesNoDup, linesIncludes);
		CCodeLines linesNamespaces;
		CIncludesHelper::WriteUsingNamespaces(namespacesNoDup, linesNamespaces);

		{
			CCodeTemplate tpl;
			ReadTemplateFromRawData(tpl, HardCodedTemplate::TypeRegPrivateH);
			CLabelToCodeMapping map;
			MapLabelToLines(map, LABEL_0, linesIncludes);
			MapLabelToLines(map, LABEL_1, linesRegClasses);
			MapLabelToLines(map, LABEL_2, linesNamespaces);
			tpl.ReplaceLabels(map, data.m_privateH);
		}
	}
}