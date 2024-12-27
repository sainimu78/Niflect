#include "NiflectGen/TaggedType/TaggedClass.h"
#include "NiflectGen/Util/CursorUtil.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "NiflectGen/Log/Log.h"

namespace NiflectGen
{
	using namespace Niflect;

	CTaggedClass::CTaggedClass()
		: m_stage(EStage::None)
#ifdef SIMPLIFIED_MACRO_CURSOR_FINDING
#else
		, m_tagLocation{}
#endif
		, m_lastAccessSpecifier(CX_CXXPrivate)
	{
	}
	bool CTaggedClass::CollectSibling(const CXCursor& cursor, const STaggedNodeCollectingContext& context)
	{
		bool addedTaggedChild = false;
		if (m_stage == EStage::None)
		{
			auto kind = clang_getCursorKind(cursor);
			if (kind == CXCursor_TypedefDecl)
			{
				if (this->CollectGeneratedBodyTag(cursor, kind))
				{

				}
				else
				{
					auto found = FindTagByDisplayName(cursor, NiflectGenDefinition::CodeTag::Field);
					if (found)
						m_createdTaggedChild = Niflect::MakeShared<CTaggedInheritableTypeField>();
					if (!found)
					{
						found = FindTagByDisplayName(cursor, NiflectGenDefinition::CodeTag::Method);
						if (found)
							m_createdTaggedChild = Niflect::MakeShared<CTaggedInheritableTypeMethod>();
					}
#ifdef PORTING_ACCESS_METHOD
					if (!found)
					{
						found = FindTagByDisplayName(cursor, NiflectGenDefinition::CodeTag::AccessMethod);
						if (found)
							m_createdTaggedChild = Niflect::MakeShared<CTaggedInheritableTypeAccessMethod>();
					}
#endif
					if (found)
					{
#ifdef SIMPLIFIED_MACRO_CURSOR_FINDING
#else
						m_tagLocation = clang_getCursorLocation(cursor);
#endif
						m_stage = EStage::FoundMember;
					}
				}
			}
			else if (kind == CXCursor_CXXAccessSpecifier)
			{
				m_lastAccessSpecifier = clang_getCXXAccessSpecifier(cursor);//���ܲ���Ҫ����, ֱ��ͨ��cursor��ȡ����, ��Ϊ�����AccessorSpecifier��cursor, ����ͨ��clang_getCXXAccessSpecifier��ȡ��
			}
			else if (kind == CXCursor_CXXBaseSpecifier)
			{
				this->InitBaseTypeSpecifierCursor(cursor);
			}
		}
		else if (m_stage == EStage::FoundMember)
		{
			this->ErrorIfNoGeneratedBodyTag(cursor);

			bool ok = false;
			auto kind = clang_getCursorKind(cursor);
			if (kind == CXCursor_FieldDecl)
				ok = true;
			else if (kind == CXCursor_CXXMethod)
				ok = true;
			if (ok)
			{
				CXCursor macroCursor;
#ifdef SIMPLIFIED_MACRO_CURSOR_FINDING
				context.m_tagCollection.PopMacroExpansion(macroCursor);
#else
				context.m_tagCollection.TakeByTagLocation(m_tagLocation, macroCursor);
#endif
				m_createdTaggedChild->InitMember(m_lastAccessSpecifier);
				this->AddChildAndInitDefault(m_createdTaggedChild, cursor, macroCursor);
				m_stage = EStage::None;
				addedTaggedChild = true;
			}
			else
			{
				GenLogError(context.m_log, "The member must be either a field or a method");
			}
		}
		return addedTaggedChild;
	}
}