#include "NiflectGen/CodeWriter/TypeReg/TypeRegGenHeaderCodeWriter.h"
#include "NiflectGen/CodeWriter/HardCoded/TypeRegGenHeaderTemplate.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"

namespace NiflectGen
{
	void WriteTypeRegsGenHeader(const CWritingContext& context, const STypeRegGenHeaderWritingContext& genCtx, STypeRegGenHeaderWritingData& data)
	{
		//todo: 收集同namespace, 同global的定义

		CCodeLines linesTypeDeclarations;
		CCodeLines linesStaticGetTypeSpecializations;
		CCodeLines linesExposedPrivateMacros;
		for (auto& it0 : genCtx.m_vecWritingData)
		{
			for (auto& it1 : it0->m_genHeaderDeclData.m_linesTypeDeclaration)
				linesTypeDeclarations.push_back(it1);

			for (auto& it1 : it0->m_genHeaderDeclData.m_linesStaticGetTypeSpecialization)
				linesStaticGetTypeSpecializations.push_back(it1);

			for (auto& it1 : it0->m_genHeaderDeclData.m_linesExposedPrivateMacro)
				linesExposedPrivateMacros.push_back(it1);
		}

		{
			CCodeTemplate tpl;
			ReadTemplateFromRawData(tpl, HardCodedTemplate::GenHeader);
			CLabelToCodeMapping map;
			MapLabelToLines(map, LABEL_0, linesTypeDeclarations);
			MapLabelToLines(map, LABEL_1, linesStaticGetTypeSpecializations);
			MapLabelToLines(map, LABEL_2, linesExposedPrivateMacros);
			tpl.ReplaceLabels(map, data.m_genH);
		}
	}
}