#pragma once
#include "NiflectGen/CodeWriter/TypeReg/TypeRegCodeWriter.h"
#include "NiflectGen/CodeWriter/TypeReg/TypeRegCode.h"

namespace NiflectGen
{
	class CMiscTypeRegCodeWriter : public CTypeRegCodeWriter2
	{
		typedef CTypeRegCodeWriter2 inherited;

	protected:
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		virtual void WriteResocursorElementsBodyCode(const SResocursorNodeBodyCodeWritingContext& context, SGetterSetterWritingData& data) const override;
		virtual void WriteResocursorChildrenBodyCode(const SResocursorNodeBodyCodeWritingContext& context, SGetterSetterWritingData& data) const override;
#else
		virtual void WriteResocursorNodeBodyCode(const SResocursorNodeBodyCodeWritingContext& context, SGetterSetterWritingData& data) const override;
#endif
		virtual void CollectDependencyHeaderFilePathAddrs(CDependencyHeaderFilePathAddrs& dependencyHeaderFilePathAddrs) const override;
	};
}