#pragma once
#include "NiflectGen/TaggedType/TaggedInheritableType.h"
#include "NiflectGen/TaggedType/TaggedClass.h"

namespace NiflectGen
{
	class CGlobalsCollection
	{
	public:
		CGlobalsCollection();
		CSharedTaggedNode AddVariableChild() const;
		CSharedTaggedNode AddFunctionChild() const;
		//const Niflect::TSharedPtr<CTaggedClass>& GetGlobalsTypeNode() const { return m_globalsTypeNode; }
		void AddRootChildIndex(uint32 idx) { m_vecIdx.push_back(idx); }
		bool IsValid() const;

	public:
		Niflect::TArray<uint32> m_vecIdx;

	private:
		//Niflect::TSharedPtr<CTaggedClass> m_globalsTypeNode;
	};
}