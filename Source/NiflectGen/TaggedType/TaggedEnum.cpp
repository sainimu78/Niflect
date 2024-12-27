#include "NiflectGen/TaggedType/TaggedEnum.h"
#include "NiflectGen/Util/CursorUtil.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/CodeWriter/TypeReg/EnumRegCodeWriter.h"

namespace NiflectGen
{
	using namespace Niflect;

	class CTaggedEnumConstant : public CTaggedNode2
	{
		typedef CTaggedNode2 inherited;
	public:
		virtual bool CollectSibling(const CXCursor& cursor, const STaggedNodeCollectingContext& context) override
		{
			if (FindTagByKindAndDisplayName(cursor, CXCursor_AnnotateAttr, NiflectGenDefinition::CodeTag::EnumConstant))
			{
				//CString displayName;
				//CXStringToCString(clang_getCursorSpelling(m_enumCursor), displayName);
				//printf("Belongs to enum: %s\n", displayName.c_str());

				CXCursor macroCursor;
#ifdef SIMPLIFIED_MACRO_CURSOR_FINDING
				context.m_tagCollection.PopMacroExpansion(macroCursor);
#else
				auto tagLocation = clang_getCursorLocation(cursor);
				context.m_tagCollection.TakeByTagLocation(tagLocation, macroCursor);
#endif
				this->InitMacroExpansionCursor(macroCursor);
			}
			return false;
		}
	};

	bool CTaggedEnum::CollectSibling(const CXCursor& cursor, const STaggedNodeCollectingContext& context)
	{
		bool addedTaggedChild = false;
		auto kind = clang_getCursorKind(cursor);
		if (kind == CXCursor_EnumConstantDecl)
		{
			auto taggedChild = MakeShared<CTaggedEnumConstant>();
			this->AddChildAndInitDefault(taggedChild, cursor, g_invalidCursor);
			addedTaggedChild = true;
		}
		return addedTaggedChild;
	}
	CSharedTypeRegCodeWriter CTaggedEnum::CreateCodeWriter(const STypeRegClassWritingSetting& setting) const
	{
		return MakeShared<CEnumRegCodeWriter>(this->GetCursor(), setting);
	}
	CSharedTypeRegCodeWriter2 CTaggedEnum::CreateCodeWriter2() const
	{
		return Niflect::MakeShared<CEnumRegCodeWriter2>();
	}
}