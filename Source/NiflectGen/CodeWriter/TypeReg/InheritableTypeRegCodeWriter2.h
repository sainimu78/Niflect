#pragma once
#include "NiflectGen/CodeWriter/TypeReg/TypeRegCodeWriter.h"
#include "NiflectGen/Collector/CollectionData.h"
#include "NiflectGen/Resolver/ResolvedData.h"
#include "NiflectGen/TaggedType/TaggedInheritableType.h"

namespace NiflectGen
{
	class CInheritableTypeRegCodeWriter2 : public CTypeRegCodeWriter2
	{
		typedef CTypeRegCodeWriter2 inherited;
	public:
		CInheritableTypeRegCodeWriter2(const Niflect::TArrayNif<CResolvedCursorNode>& vecFieldResocursorNode, const CTaggedType* baseTaggedType, uint32 generatedBodyLineNumber
			, const Niflect::TArrayNif<CTaggedInheritableTypeField*>& vecField
#ifdef PORTING_ACCESS_METHOD
			, const Niflect::TArrayNif<CTaggedInheritableTypeAccessMethod*>& vecAccessMethod
#endif
			, const Niflect::TArray<CResolvedMethod>& vecResomethod
			, const Niflect::TArray<CTaggedInheritableTypeMethod*>& vecMethod
		);
//#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
//		virtual void WriteInvokeMethodFunc(const STypeReginvokeMethodFuncWritingInput& input, STypeReginvokeMethodFuncWritingOutput& output) const override;
//#endif

	protected:
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		virtual void WriteResocursorNodeBodyCode(const SResocursorNodeBodyCodeWritingContext& context, SGetterSetterWritingData& data) const override;
#else
		virtual void WriteResocursorNodeBodyCode(const SResocursorNodeBodyCodeWritingContext& context, SGetterSetterWritingData& data) const override;
#endif
		virtual void CollectDependencyHeaderFilePathAddrs(CDependencyHeaderFilePathAddrs& dependencyHeaderFilePathAddrs) const override;
		virtual void CollectDataForGenH(SCollectingGeneratedBodyWritingData& data) const override;

	public:
		virtual void WriteInvokeInitType(const STypeRegClassWritingContext& context, CTypeRegTaggedTypeInitWritingData2& data) const override;

	private:
		void WriteStaticInitType(const STypeRegClassWritingContext& context, CCodeLines& lines) const;

	private:
		const Niflect::TArrayNif<CResolvedCursorNode>& m_vecFieldResocursorNode;
		const Niflect::TArrayNif<CTaggedInheritableTypeField*>& m_vecField;
#ifdef PORTING_ACCESS_METHOD
		const Niflect::TArrayNif<CTaggedInheritableTypeAccessMethod*>& m_vecAccessMethod;
#endif
		const Niflect::TArray<CResolvedMethod>& m_vecResomethod;
		const Niflect::TArray<CTaggedInheritableTypeMethod*>& m_vecTaggedMethod;
		const CTaggedType* m_baseTaggedType;
		const uint32 m_generatedBodyLineNumber;
	};
}