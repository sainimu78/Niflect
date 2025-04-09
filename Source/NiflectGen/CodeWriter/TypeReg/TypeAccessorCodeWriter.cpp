#include "NiflectGen/CodeWriter/TypeReg/TypeAccessorCodeWriter.h"
#include "NiflectGen/CodeWriter/CppTemplate.h"
#include "NiflectGen/CodeWriter/HardCoded/TypeRegInitialRegTemplate.h"
#include "NiflectGen/Collector/TaggedNode.h"//WriteNataArgNullOrVar
#include "Niflect/NiflectField.h"
#include "Niflect/NiflectMethod.h"
#include "NiflectGen/Util/WriterUtil.h"

namespace NiflectGen
{
	static void WriteNextInitNextAccessor2(const Niflect::CString& funcName, CCodeLines& linesCode)
	{
		CCodeTemplate tpl0;
		ReadTemplateFromRawData(tpl0, HardCodedTemplate::CreateAndInitNextsAccessor);
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_2, funcName);
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl0.ReplaceLabels(map, linesCode, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
	}
	void WriteNextInitChildAccessor2(const Niflect::CString& fieldOwnerResocursorName, const Niflect::CString& funcName, const Niflect::CString& fieldName, const CCodeLines& linesNata, CCodeLines& linesResoBodyCode)
	{
		CCodeLines linesCode;
		WriteNextInitNextAccessor2(funcName, linesCode);
		Niflect::CString nataNullOrVar;
		WriteNataArgNullOrVar(linesNata, linesCode, nataNullOrVar);
		{
			CCodeTemplate tpl0;
			ReadTemplateFromRawData(tpl0, HardCodedTemplate::InitChildAccessor);
			CLabelToCodeMapping map;
			MapLabelToText(map, LABEL_0, fieldName);
			auto invokeGetFieldOffset = ReplaceLabelToText2(HardCodedTemplate::GetFieldOffset, LABEL_0, LABEL_8, fieldOwnerResocursorName, fieldName);
			MapLabelToText(map, LABEL_7, invokeGetFieldOffset);
			MapLabelToText(map, LABEL_14, nataNullOrVar);
			MapLabelToText(map, LABEL_26, ConvertHashToString(Niflect::ComputeFieldHash(fieldName)));
			Niflect::TSet<Niflect::CString> setReplacedLabel;
			tpl0.ReplaceLabels(map, linesCode, &setReplacedLabel);
			ASSERT(setReplacedLabel.size() == map.size());
		}
		ReplaceLabelToImplScopeLines(linesCode, linesResoBodyCode);
	}
	void WriteNextInitElementAccessor2(const Niflect::CString& funcName, CCodeLines& linesResoBodyCode)
	{
		CCodeLines linesCode;
		WriteNextInitNextAccessor2(funcName, linesCode);
		{
			CCodeTemplate tpl0;
			ReadTemplateFromRawData(tpl0, HardCodedTemplate::InitElementAccessor);
			CLabelToCodeMapping map;
			Niflect::TSet<Niflect::CString> setReplacedLabel;
			tpl0.ReplaceLabels(map, linesCode, &setReplacedLabel);
			ASSERT(setReplacedLabel.size() == map.size());
		}
		ReplaceLabelToImplScopeLines(linesCode, linesResoBodyCode);
	}
	static void GenerateTypeBodyInvokeMethodFuncNameInternal(const char* pszTemplate, const Niflect::CString& typeName, uint32 methodIdx, Niflect::CString& typeBodyFuncName)
	{
		typeBodyFuncName = ReplaceLabelToText2(pszTemplate, LABEL_0, LABEL_16, typeName, NiflectUtil::FormatString("%u", methodIdx));
	}
	static Niflect::CString ConvertArgDummyToSpecific(const Niflect::CString& varName, const Niflect::CString& typeName, uint32 dummyIdx)
	{
		return NiflectUtil::FormatString("auto& %s = *static_cast<%s*>(args[%u]);", varName.c_str(), typeName.c_str(), dummyIdx);
	}
#define CONVERTED_RESULT_VAR_NAME "ret0"
	static void GenerateDummyArgsConversionCode(const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, CCodeLines& linesConversion, Niflect::CString& linePassing)
	{
		for (uint32 idx0 = 0; idx0 < vecArgument.size(); ++idx0)
		{
			auto& it0 = vecArgument[idx0];
			auto varName = NiflectUtil::FormatString("arg%u", idx0);
			auto conv = ConvertArgDummyToSpecific(varName, it0.m_resocursorName, idx0);
			linesConversion.push_back(conv);
			linePassing += varName;
			if (idx0 != vecArgument.size() - 1)
				linePassing += ", ";
		}
		if (resultType.IsValid())
		{
			Niflect::CString varName = CONVERTED_RESULT_VAR_NAME;
			auto conv = ConvertArgDummyToSpecific(varName, resultType.m_resocursorName, static_cast<uint32>(vecArgument.size()));
			linesConversion.push_back(conv);
		}
	}
	static void WriteTypeBodyInvokeConstructor(const Niflect::CString& typeNameForLastTemplateArg, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const Niflect::CString& typeBodyFuncName, CCodeLines& linesCode)
	{
		CCodeLines linesConversion;
		Niflect::CString linePassing;
		CResolvedCursorNode placeholder;
		GenerateDummyArgsConversionCode(vecArgument, placeholder, linesConversion, linePassing);

		CCodeTemplate tpl0;
		ReadTemplateFromRawData(tpl0, HardCodedTemplate::TypeBodyInvokeConstructorImpl);
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_9, typeNameForLastTemplateArg);
		MapLabelToText(map, LABEL_17, typeBodyFuncName);
		MapLabelToLines(map, LABEL_18, linesConversion);
		ASSERT(!linePassing.empty());
		MapLabelToText(map, LABEL_19, linePassing);
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl0.ReplaceLabels(map, linesCode, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
	}
	static void InternalWriteTypeBodyInvokeMethod(const char* pszTemplate, const Niflect::CString& typeName, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, const Niflect::CString& typeBodyFuncName, const Niflect::CString& methodName, CCodeLines& linesCode)
	{
		CCodeLines linesConversion;
		Niflect::CString linePassing;
		GenerateDummyArgsConversionCode(vecArgument, resultType, linesConversion, linePassing);

		CCodeTemplate tpl0;
		ReadTemplateFromRawData(tpl0, pszTemplate);
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_0, typeName);
		MapLabelToText(map, LABEL_17, typeBodyFuncName);
		if (linesConversion.size() > 0)
			MapLabelToLines(map, LABEL_18, linesConversion);
		if (!linePassing.empty())
			MapLabelToText(map, LABEL_19, linePassing);
		MapLabelToText(map, LABEL_21, methodName);
		if (resultType.IsValid())
			MapLabelToText(map, LABEL_22, NiflectUtil::FormatString("%s = ", CONVERTED_RESULT_VAR_NAME));
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl0.ReplaceLabels(map, linesCode, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
	}
	void WriteInvokeConstructorBody(const Niflect::CString& typeName, const Niflect::CString& typeNameForLastTemplateArg, uint32 methodIdx, const Niflect::TArray<CResolvedCursorNode>& vecArgument, Niflect::CString& typeBodyFuncName, CCodeLines& linesImpl)
	{
		GenerateTypeBodyInvokeMethodFuncNameInternal(HardCodedTemplate::TypeBodyInvokeConstructorFuncName, typeName, methodIdx, typeBodyFuncName);
		WriteTypeBodyInvokeConstructor(typeNameForLastTemplateArg, vecArgument, typeBodyFuncName, linesImpl);
	}
	void WriteInvokeMethodBody(const Niflect::CString& typeName, uint32 methodIdx, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, const Niflect::CString& methodName, Niflect::CString& typeBodyFuncName, CCodeLines& linesImpl)
	{
		GenerateTypeBodyInvokeMethodFuncNameInternal(HardCodedTemplate::TypeBodyInvokeMethodFuncName, typeName, methodIdx, typeBodyFuncName);
		InternalWriteTypeBodyInvokeMethod(HardCodedTemplate::TypeBodyInvokeMethodImpl, typeName, vecArgument, resultType, typeBodyFuncName, methodName, linesImpl);
	}
	void WriteInvokeStaticMemberFunctionBody(const Niflect::CString& typeName, uint32 methodIdx, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, const Niflect::CString& methodName, Niflect::CString& typeBodyFuncName, CCodeLines& linesImpl)
	{
		GenerateTypeBodyInvokeMethodFuncNameInternal(HardCodedTemplate::TypeBodyInvokeStaticMemberFunctionFuncName, typeName, methodIdx, typeBodyFuncName);
		InternalWriteTypeBodyInvokeMethod(HardCodedTemplate::TypeBodyInvokeFunctionImpl, typeName, vecArgument, resultType, typeBodyFuncName, methodName, linesImpl);
	}
	static Niflect::CString GenerateParameterInfoConstructionCode(const Niflect::CString& staticGetTypeFuncName)
	{
		return NiflectUtil::FormatString("CParameterInfo(%s(), false, false)", staticGetTypeFuncName.c_str());
	}
	static Niflect::CString GenerateInputParameterInfoConstructionCode(const Niflect::CString& staticGetTypeFuncName)
	{
		return NiflectUtil::FormatString("info.m_vecInput.push_back(%s);", GenerateParameterInfoConstructionCode(staticGetTypeFuncName).c_str());
	}
	static Niflect::CString GenerateOutputParameterInfoConstructionCode(const Niflect::CString& staticGetTypeFuncName)
	{
		return NiflectUtil::FormatString("info.m_vecOutput.push_back(%s);", GenerateParameterInfoConstructionCode(staticGetTypeFuncName).c_str());
	}
	void WriteMethodRegConstructorInfo(const Niflect::CString& typeBodyFuncName, const CCodeLines& linesNata, const Niflect::CString& moduleScopeSymbolPrefix, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, CCodeLines& linesReg)
	{
		CCodeLines linesArgReg;
		Niflect::TArray<Niflect::CString> vecArgResocursorName;
		if (vecArgument.size() > 0)
		{
			for (uint32 idx0 = 0; idx0 < vecArgument.size(); ++idx0)
			{
				auto& it0 = vecArgument[idx0];
				auto argStaticGetTypeFuncName = it0.GetStaticGetTypeFuncName(moduleScopeSymbolPrefix);
				linesArgReg.push_back(GenerateInputParameterInfoConstructionCode(argStaticGetTypeFuncName));
				vecArgResocursorName.push_back(it0.m_resocursorName);
			}
		}

		CCodeLines linesCode;
		Niflect::CString nataNullOrVar;
		WriteNataArgNullOrVar(linesNata, linesCode, nataNullOrVar);
		CCodeTemplate tpl0;
		ReadTemplateFromRawData(tpl0, HardCodedTemplate::MethodRegConstructorInfo);
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_14, nataNullOrVar);
		MapLabelToText(map, LABEL_17, typeBodyFuncName);
		if (linesArgReg.size() > 0)
			MapLabelToLines(map, LABEL_20, linesArgReg);
		auto argsSignature = NiflectUtil::CombineFromStrings(vecArgResocursorName, ',');
		MapLabelToText(map, LABEL_24, ConvertHashToString(Niflect::ComputeSignatureHash(argsSignature)));
		MapLabelToText(map, LABEL_25, "\"" + argsSignature + "\"");
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl0.ReplaceLabels(map, linesCode, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
		ReplaceLabelToImplScopeLines(linesCode, linesReg);
	}
	static void InternalWriteMethodRegMethodInfo(const char* pszTemplate, const Niflect::CString& typeBodyFuncName, const CCodeLines& linesNata, const Niflect::CString& moduleScopeSymbolPrefix, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, const Niflect::CString& methodName, CCodeLines& linesReg)
	{
		CCodeLines linesArgReg;
		Niflect::TArray<Niflect::CString> vecArgResocursorName;
		if (vecArgument.size() > 0)
		{
			for (uint32 idx0 = 0; idx0 < vecArgument.size(); ++idx0)
			{
				auto& it0 = vecArgument[idx0];
				auto argStaticGetTypeFuncName = it0.GetStaticGetTypeFuncName(moduleScopeSymbolPrefix);
				linesArgReg.push_back(GenerateInputParameterInfoConstructionCode(argStaticGetTypeFuncName));
				vecArgResocursorName.push_back(it0.m_resocursorName);
			}
			if (resultType.IsValid())
			{
				auto retStaticGetTypeFuncName = resultType.GetStaticGetTypeFuncName(moduleScopeSymbolPrefix);
				linesArgReg.push_back(GenerateOutputParameterInfoConstructionCode(retStaticGetTypeFuncName));
			}
		}

		CCodeLines linesCode;
		Niflect::CString nataNullOrVar;
		WriteNataArgNullOrVar(linesNata, linesCode, nataNullOrVar);
		CCodeTemplate tpl0;
		ReadTemplateFromRawData(tpl0, pszTemplate);
		CLabelToCodeMapping map;
		MapLabelToText(map, LABEL_14, nataNullOrVar);
		MapLabelToText(map, LABEL_17, typeBodyFuncName);
		if (linesArgReg.size() > 0)
			MapLabelToLines(map, LABEL_20, linesArgReg);
		MapLabelToText(map, LABEL_21, methodName);
		auto argsSignature = NiflectUtil::CombineFromStrings(vecArgResocursorName, ',');
		auto signature = methodName + ',' + argsSignature;
		MapLabelToText(map, LABEL_24, ConvertHashToString(Niflect::ComputeSignatureHash(signature)));
		MapLabelToText(map, LABEL_25, "\"" + argsSignature + "\"");
		Niflect::TSet<Niflect::CString> setReplacedLabel;
		tpl0.ReplaceLabels(map, linesCode, &setReplacedLabel);
		ASSERT(setReplacedLabel.size() == map.size());
		ReplaceLabelToImplScopeLines(linesCode, linesReg);
	}
	void WriteMethodRegMethodInfo(const Niflect::CString& typeBodyFuncName, const CCodeLines& linesNata, const Niflect::CString& moduleScopeSymbolPrefix, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, const Niflect::CString& methodName, CCodeLines& linesReg)
	{
		InternalWriteMethodRegMethodInfo(HardCodedTemplate::MethodRegMethodInfo, typeBodyFuncName, linesNata, moduleScopeSymbolPrefix, vecArgument, resultType, methodName, linesReg);
	}
	void WriteMethodRegStaticMemberFunctionInfo(const Niflect::CString& typeBodyFuncName, const CCodeLines& linesNata, const Niflect::CString& moduleScopeSymbolPrefix, const Niflect::TArray<CResolvedCursorNode>& vecArgument, const CResolvedCursorNode& resultType, const Niflect::CString& methodName, CCodeLines& linesReg)
	{
		InternalWriteMethodRegMethodInfo(HardCodedTemplate::MethodRegStaticMemberFunctionInfo, typeBodyFuncName, linesNata, moduleScopeSymbolPrefix, vecArgument, resultType, methodName, linesReg);
	}
}