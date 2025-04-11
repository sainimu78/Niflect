#include "NiflectGen/Collector/GlobalsCollection.h"
#include "NiflectGen/Util/CursorUtil.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/Log/Log.h"

namespace NiflectGen
{
	CGlobalsCollection::CGlobalsCollection()
		//: m_globalsTypeNode(Niflect::MakeShared<CTaggedClass>())
	{
	}
	CSharedTaggedNode CGlobalsCollection::AddVariableChild() const
	{
		return Niflect::MakeShared<CTaggedInheritableTypeField>();
	}
	CSharedTaggedNode CGlobalsCollection::AddFunctionChild() const
	{
		return Niflect::MakeShared<CTaggedInheritableTypeMethod>();
	}
	bool CGlobalsCollection::IsValid() const
	{
		//return m_globalsTypeNode->GetChildrenCount() > 0;
		return m_vecIdx.size() > 0;
	}
}