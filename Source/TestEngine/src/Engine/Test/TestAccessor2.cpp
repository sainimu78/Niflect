#include "Engine/Test/TestAccessor2.h"
#include <stdio.h>
#include "Niflect/Default/CompoundAccessor.h"
#include "Niflect/Default/ValueTypeAccessor.h"
#include "Niflect/Default/ContainerTypeAccessor.h"
#include "Niflect/Default/STLContainerTypeAccessor.h"
#include "Niflect/NiflectTable.h"
#include "Niflect/Util/StringUtil.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "Engine/Test/TestMyTransform.h"
#include "Niflect/Default/EnumAccessor.h"

namespace TestAccessor2
{
	enum class EMyOption : uint8
	{
		None,
		Default,
		Mode0,
		Mode1,
	};
}

namespace Engine
{
	using namespace Niflect;

	template <typename TAccessor, typename TType>
	static CSharedAccessor CreateFieldLayoutForfFunctionPointerrrrrrrr()
	{
		auto accessor = Niflect::MakeShared<TAccessor>();
		accessor->InitType(StaticGetTypeOld<TType>());
		return accessor;
	}

	template <typename T>
	CSharedAccessor CreateSSSSSSSSSSSS();

	template <>
	CSharedAccessor CreateSSSSSSSSSSSS<Niflect::TArrayNif<float> >()
	{
		auto accessor0 = Niflect::MakeShared<TArrayAccessor<Niflect::TArrayNif<float> > >();
		accessor0->InitType(StaticGetTypeOld<Niflect::TArrayNif<float> >());
		{
			auto type = StaticGetTypeOld<float>();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
			accessor0->InitElementAccessor(accessor1);
		}
		return accessor0;
	}
	template <>
	CSharedAccessor CreateSSSSSSSSSSSS<Niflect::TArrayNif<Niflect::CString> >()
	{
		auto accessor0 = Niflect::MakeShared<TArrayAccessor<Niflect::TArrayNif<Niflect::CString> > >();
		accessor0->InitType(StaticGetTypeOld<Niflect::TArrayNif<Niflect::CString> >());
		{
			auto type = StaticGetTypeOld<Niflect::CString>();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
			accessor0->InitElementAccessor(accessor1);
		}
		return accessor0;
	}
	template <>
	CSharedAccessor CreateSSSSSSSSSSSS<Niflect::TArrayNif<TestAccessor2::TMyTransform<float> > >()
	{
		auto accessor0 = Niflect::MakeShared<TArrayAccessor<Niflect::TArrayNif<TestAccessor2::TMyTransform<float> > > >();
		accessor0->InitType(StaticGetTypeOld<Niflect::TArrayNif<TestAccessor2::TMyTransform<float> > >());
		{
			auto type = StaticGetTypeOld<TestAccessor2::TMyTransform<float> >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
			accessor0->InitElementAccessor(accessor1);
		}
		return accessor0;
	}
	template <>
	CSharedAccessor CreateSSSSSSSSSSSS<Niflect::TArrayNif<bool> >()
	{
		auto accessor0 = Niflect::MakeShared<TBitsArrayAccessor<Niflect::TArrayNif<bool> > >();
		accessor0->InitType(StaticGetTypeOld<Niflect::TArrayNif<bool> >());
		return accessor0;
	}
	template <>
	CSharedAccessor CreateSSSSSSSSSSSS<std::vector<bool> >()
	{
		auto accessor0 = Niflect::MakeShared<TSTLBitsArrayAccessor<std::vector<bool> > >();
		accessor0->InitType(StaticGetTypeOld<std::vector<bool> >());
		return accessor0;
	}
	template <>
	CSharedAccessor CreateSSSSSSSSSSSS<Niflect::TArrayNif<Niflect::TArrayNif<float> > >()
	{
		auto accessor0 = Niflect::MakeShared<TArrayAccessor<Niflect::TArrayNif<Niflect::TArrayNif<float> > > >();
		accessor0->InitType(StaticGetTypeOld<Niflect::TArrayNif<Niflect::TArrayNif<float> > >());
		{
			auto type = StaticGetTypeOld<Niflect::TArrayNif<float> >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
			accessor0->InitElementAccessor(accessor1);
		}
		return accessor0;
	}
	template <>
	CSharedAccessor CreateSSSSSSSSSSSS<std::pair<Niflect::CString, float> >()
	{
		auto accessor0 = Niflect::MakeShared<CCompoundAccessor>();
		accessor0->InitType(StaticGetTypeOld<std::pair<Niflect::CString, float> >());
		{
			auto type = StaticGetTypeOld<Niflect::CString>();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("first", GetMemberVariableOffset(&std::pair<Niflect::CString, float>::first));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<float>();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("second", GetMemberVariableOffset(&std::pair<Niflect::CString, float>::second));
			accessor0->AddChild(accessor1);
		}
		return accessor0;
	}
	template <>
	CSharedAccessor CreateSSSSSSSSSSSS<Niflect::TMap<Niflect::CString, float> >()
	{
		auto accessor0 = Niflect::MakeShared<TMapAccessor<Niflect::TMap<Niflect::CString, float> > >();
		accessor0->InitType(StaticGetTypeOld<Niflect::TMap<Niflect::CString, float> >());
		{
			auto type = StaticGetTypeOld<std::pair<Niflect::CString, float> >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
			accessor0->InitElementAccessor(accessor1);
		}
		return accessor0;
	}
	template <>
	CSharedAccessor CreateSSSSSSSSSSSS<float>()
	{
		auto accessor0 = Niflect::MakeShared<CFloatAccessor>();
		accessor0->InitType(StaticGetTypeOld<float>());
		return accessor0;
	}
	template <>
	CSharedAccessor CreateSSSSSSSSSSSS<bool>()
	{
		auto accessor0 = Niflect::MakeShared<CBoolAccessor>();
		accessor0->InitType(StaticGetTypeOld<bool>());
		return accessor0;
	}
	template <>
	CSharedAccessor CreateSSSSSSSSSSSS<Niflect::CString>()
	{
		auto accessor0 = Niflect::MakeShared<CStringAccessor>();
		accessor0->InitType(StaticGetTypeOld<Niflect::CString>());
		return accessor0;
	}
	template <>
	CSharedAccessor CreateSSSSSSSSSSSS<TestAccessor2::EMyOption>()
	{
		auto accessor0 = Niflect::MakeShared<Engine::CEnumClassAccessor>();
		accessor0->InitType(StaticGetTypeOld<TestAccessor2::EMyOption>());
		return accessor0;
	}
}

namespace TestAccessor2
{
	using namespace Engine;

	class CTestClassMy
	{
	public:
		CTestClassMy()
			: m_float_0(0.0f)
			, m_bool_1(false)
			, m_tm_8{}
		{
			printf("CTestClassMy, constructed\n");
		}
		~CTestClassMy()
		{
			printf("CTestClassMy, destructed\n");
		}

		void TestInit()
		{
			m_float_0 = 1.0f;
			m_bool_1 = true;
			m_array_2.resize(3);
			m_array_2[0] = 2.1f;
			m_array_2[1] = 2.2f;
			m_array_2[2] = 2.3f;
			m_array_array_3.resize(2);
			m_array_array_3[0].resize(3);
			m_array_array_3[0][0] = 3.1f;
			m_array_array_3[0][1] = 3.2f;
			m_array_array_3[0][2] = 3.3f;
			m_array_array_3[1].resize(2);
			m_array_array_3[1][0] = 4.1f;
			m_array_array_3[1][1] = 4.2f;
			m_array_4.resize(3);
			m_array_4[0] = true;
			m_array_4[1] = false;
			m_array_4[2] = true;
			m_array_5.resize(5);
			m_array_5[0] = true;
			m_array_5[1] = false;
			m_array_5[2] = true;
			m_array_5[3] = false;
			m_array_5[4] = true;
			m_map_6["nihao"] = 5.1f;
			m_map_6["bucuo"] = 5.2f;
			m_map_6["shima"] = 5.3f;
			m_array_7.push_back("zhende");
			m_array_7.push_back("woxin");
			m_array_7.push_back("jiushi");
			TestInit_TMyTransform(m_tm_8);
		}

		bool operator==(const CTestClassMy& rhs) const
		{
			//if (inherited::operator==(rhs))
			{
				return
					(m_float_0 == rhs.m_float_0)
					&& (m_bool_1 == rhs.m_bool_1)
					&& (m_array_2 == rhs.m_array_2)
					&& (m_array_array_3 == rhs.m_array_array_3)
					&& (m_array_4 == rhs.m_array_4)
					&& (m_array_5 == rhs.m_array_5)
					&& (m_map_6 == rhs.m_map_6)
					&& (m_array_7 == rhs.m_array_7)
					&& (m_tm_8 == rhs.m_tm_8)
					;
			}
			//return false;
		}

	public:
		float m_float_0;
		bool m_bool_1;
		Niflect::TArrayNif<float> m_array_2;
		Niflect::TArrayNif<Niflect::TArrayNif<float> > m_array_array_3;
		Niflect::TArrayNif<bool> m_array_4;
		std::vector<bool> m_array_5;
		Niflect::TMap<Niflect::CString, float> m_map_6;
		Niflect::TArrayNif<Niflect::CString> m_array_7;
		TMyTransform<float> m_tm_8;
	};

	class CTestClassMy2 : public CTestClassMy
	{
		typedef CTestClassMy inherited;
	public:
		CTestClassMy2()
			: m_derived_bool_0(false)
			, m_derived_float_2(0.0f)
			, m_derived_bool_4(false)
		{

		}

		void TestInit()
		{
			inherited::TestInit();

			m_derived_bool_0 = true;
			m_derived_array_1.resize(4);
			m_derived_array_1[0] = 20.1f;
			m_derived_array_1[1] = 20.2f;
			m_derived_array_1[2] = 20.3f;
			m_derived_array_1[3] = 20.4f;
			m_derived_float_2 = 10.1f;
			m_derived_array_3.resize(3);
			m_derived_array_3[0] = true;
			m_derived_array_3[1] = false;
			m_derived_array_3[2] = true;
			m_derived_bool_4 = false;
			m_derived_array_5.resize(3);
			for (auto idx = 0; idx < m_derived_array_5.size(); ++idx)
			{
				auto& it = m_derived_array_5[idx];
				TestInit_TMyTransform(it);
				it.m_scale.m_y += 1 + idx;
			}
		}

		bool operator==(const CTestClassMy2& rhs) const
		{
			if (inherited::operator==(rhs))
			{
				return
					(m_derived_bool_0 == rhs.m_derived_bool_0) &&
					(m_derived_array_1 == rhs.m_derived_array_1) &&
					(m_derived_float_2 == rhs.m_derived_float_2) &&
					(m_derived_array_3 == rhs.m_derived_array_3) &&
					(m_derived_bool_4 == rhs.m_derived_bool_4) &&
					(m_derived_array_5 == rhs.m_derived_array_5)
					;
			}
			return false;
		}

		bool m_derived_bool_0;
		Niflect::TArrayNif<float> m_derived_array_1;
		float m_derived_float_2;
		Niflect::TArrayNif<bool> m_derived_array_3;
		bool m_derived_bool_4;
		Niflect::TArrayNif<TMyTransform<float> > m_derived_array_5;
	};

	Niflect::CSharedAccessor SSSSSSSSSS_CTestClassMy()
	{
		using namespace Niflect;
		auto accessor0 = Niflect::MakeShared<CCompoundAccessor>();
		accessor0->InitType(StaticGetTypeOld<CTestClassMy>());
		{
			auto type = StaticGetTypeOld<float>();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_float_0", GetMemberVariableOffset(&CTestClassMy::m_float_0));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<bool>();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_bool_1", GetMemberVariableOffset(&CTestClassMy::m_bool_1));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<Niflect::TArrayNif<float> >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_array_2", GetMemberVariableOffset(&CTestClassMy::m_array_2));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<Niflect::TArrayNif<Niflect::TArrayNif<float> > >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_array_array_3", GetMemberVariableOffset(&CTestClassMy::m_array_array_3));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<Niflect::TArrayNif<bool> >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_array_4", GetMemberVariableOffset(&CTestClassMy::m_array_4));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<std::vector<bool> >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_array_5", GetMemberVariableOffset(&CTestClassMy::m_array_5));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<Niflect::TMap<Niflect::CString, float> >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_map_6", GetMemberVariableOffset(&CTestClassMy::m_map_6));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<Niflect::TArrayNif<Niflect::CString> >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_array_7", GetMemberVariableOffset(&CTestClassMy::m_array_7));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<TMyTransform<float> >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_tm_8", GetMemberVariableOffset(&CTestClassMy::m_tm_8));
			accessor0->AddChild(accessor1);
		}
		return accessor0;
	}
	Niflect::CSharedAccessor SSSSSSSSSS_CTestClassMy2()
	{
		using namespace Niflect;
		auto accessor0 = Niflect::MakeShared<CCompoundAccessor>();
		accessor0->InitType(StaticGetTypeOld<CTestClassMy2>());
		{
			auto type = StaticGetTypeOld<bool>();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_derived_bool_0", GetMemberVariableOffset(&CTestClassMy2::m_derived_bool_0));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<Niflect::TArrayNif<float> >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_derived_array_1", GetMemberVariableOffset(&CTestClassMy2::m_derived_array_1));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<float>();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_derived_float_2", GetMemberVariableOffset(&CTestClassMy2::m_derived_float_2));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<Niflect::TArrayNif<bool> >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_derived_array_3", GetMemberVariableOffset(&CTestClassMy2::m_derived_array_3));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<bool>();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_derived_bool_4", GetMemberVariableOffset(&CTestClassMy2::m_derived_bool_4));
			accessor0->AddChild(accessor1);
		}
		{
			auto type = StaticGetTypeOld<Niflect::TArrayNif<TMyTransform<float> > >();
			auto accessor1 = type->CreateFieldLayout();
			accessor1->InitMemberMeta("m_derived_array_5", GetMemberVariableOffset(&CTestClassMy2::m_derived_array_5));
			accessor0->AddChild(accessor1);
		}
		return accessor0;
	}

	class CPropertyNode;

	using CSharedPropertyNode = Niflect::TSharedPtr<CPropertyNode>;


	class CPropertyNodeFactory
	{
	public:
		void Register(Niflect::CNiflectType* accessorType, Niflect::CNiflectType* propType)
		{
			auto ret = m_mapAccessorToPropType.insert({ accessorType, propType });
			ASSERT(ret.second);
		}
		Niflect::CNiflectType* FindBinding(Niflect::CNiflectType* accessorType) const
		{
			auto itFound = m_mapAccessorToPropType.find(accessorType);
			if (itFound != m_mapAccessorToPropType.end())
				return itFound->second;
			return NULL;
		}

	private:
		Niflect::TMap<CNiflectType*, CNiflectType*> m_mapAccessorToPropType;
	};

	struct SSSSSSSSContext
	{
		const CPropertyNodeFactory& m_factory;
		const CRwNode* m_rw;
		const CAccessor* m_accessor;
	};

	class CPropertyNode
	{
	public:
		void Init(const Niflect::CString& name)
		{
			m_name = name;
		}

	public:
		virtual void BuildSelf(const SSSSSSSSContext& ctx) = 0;

	public:
		void AddNode(const CSharedPropertyNode& node)
		{
			m_vecNode.push_back(node);
		}

	public:
		Niflect::CString m_name;
		Niflect::TArrayNif<CSharedPropertyNode> m_vecNode;
	};

	static CSharedPropertyNode BuildPropertyTreeRecurs(const CPropertyNodeFactory& factory, const CRwNode* rwParent, const Niflect::CAccessor* accessorParent);

	class CPropertyItem : public CPropertyNode
	{
	public:
		virtual void BuildSelf(const SSSSSSSSContext& ctx) override
		{

		}
		static CPropertyItem* CastChecked(CPropertyNode* base)
		{
			return dynamic_cast<CPropertyItem*>(base);
		}
		static const CPropertyItem* CastChecked(const CPropertyNode* base)
		{
			return dynamic_cast<const CPropertyItem*>(base);
		}
		CSharedRwNode m_rwData;
	};
	using CSharedPropertyItem = Niflect::TSharedPtr<CPropertyItem>;

	class CPropertyGroup : public CPropertyNode
	{
		typedef CPropertyNode inherited;
	public:
		virtual void BuildSelf(const SSSSSSSSContext& ctx) override
		{
			auto cnt = ctx.m_accessor->GetChildrenCount();
			for (uint32 idx = 0; idx < cnt; ++idx)
			{
				auto accessorChild = ctx.m_accessor->GetChild(idx);
				auto rwChild = ctx.m_rw->GetNode(idx);
				if (auto propChild = BuildPropertyTreeRecurs(ctx.m_factory, rwChild, accessorChild))
				{
					propChild->Init(accessorChild->GetName());
					this->AddNode(propChild);
				}
			}
		}
	};

	class CPropertyArray : public CPropertyNode
	{
		typedef CPropertyNode inherited;
	public:
		virtual void BuildSelf(const SSSSSSSSContext& ctx) override
		{
			auto elemAccessor = ctx.m_accessor->GetElementAccessor();
			auto cnt = ctx.m_rw->GetNodesCount();
			for (uint32 idx = 0; idx < cnt; ++idx)
			{
				auto rwChild = ctx.m_rw->GetNode(idx);
				if (auto propChild = BuildPropertyTreeRecurs(ctx.m_factory, rwChild, elemAccessor))
				{
					propChild->Init(NiflectUtil::FormatString("%u (%s)", idx, elemAccessor->GetType()->GetTypeName().c_str()));
					this->AddNode(propChild);
				}
			}
		}
	};

	class CPropertyBitsArray : public CPropertyNode
	{
		typedef CPropertyNode inherited;
	public:
		virtual void BuildSelf(const SSSSSSSSContext& ctx) override
		{
			auto cnt = ctx.m_rw->GetNodesCount();
			for (uint32 idx = 0; idx < cnt; ++idx)
			{
				auto rwChild = ctx.m_rw->GetNode(idx);
				auto propElem = Niflect::MakeShared<CPropertyItem>();
				//打印时无值, 是因为 rwChild 未保存到 propElem 中, 现 propElem 仅作占位
				//须先实现对应 Accessor 的 Save/Load, 后再根据其数据读写方式改写 property, 可参考 TPropertyMyTransform
				//大致实现方法为只保存 CPropertyBitsArray 一级的 m_rwData, 从而可复用通用的 PropertyTree 序列化
				//计划实现的 PropertyTree 序列化, 只需要递归收集每个 PropertyNode 的 m_rwData, 现仅 PropertyItem 具有 m_rwData, 可考虑改为 PropertyNode 即具有 rwData, 废弃 PropertyItem
				propElem->Init(NiflectUtil::FormatString("%u (bit)", idx));
				this->AddNode(propElem);
			}
		}
	};

	class CPropertyMap : public CPropertyNode
	{
		typedef CPropertyNode inherited;
	public:
		virtual void BuildSelf(const SSSSSSSSContext& ctx) override
		{
			auto elemAccessor = ctx.m_accessor->GetElementAccessor();
			auto cnt = ctx.m_rw->GetNodesCount();
			for (uint32 idx = 0; idx < cnt; ++idx)
			{
				auto rwChild = ctx.m_rw->GetNode(idx);
				//希望将 first 与 second 另命名为 key 与 value, 可通过选项传入, 或另外定义 pair 的 property 与 accessor
				if (auto propChild = BuildPropertyTreeRecurs(ctx.m_factory, rwChild, elemAccessor))
				{
					//如需要, 可在此处修改 first 与 second 名称
					propChild->Init(NiflectUtil::FormatString("%u (%s)", idx, elemAccessor->GetType()->GetTypeName().c_str()));
					this->AddNode(propChild);
				}
			}
		}
	};

	CSharedPropertyNode BuildPropertyTreeRecurs(const CPropertyNodeFactory& factory, const CRwNode* rwParent, const Niflect::CAccessor* accessorParent)
	{
		auto accessorType = accessorParent->GetType();
		CSharedPropertyNode prop;
		if (auto propType = factory.FindBinding(accessorType))
			prop = NiflectTypeMakeShared<CPropertyNode>(propType);
		if (prop == NULL)
		{
			if (rwParent->IsValue())
			{
				ASSERT(false);//没注册type与Property的绑定? 见 factory.Register(StaticGetTypeOld<float>(), StaticGetTypeOld<CPropertyFloat>());
			}
			else if (rwParent->IsArray())
			{
				if (auto p = CArrayAccessor::CastChecked(accessorParent))
					prop = Niflect::MakeShared<CPropertyArray>();
				else if (auto p = CBitsArrayAccessor::CastChecked(accessorParent))
					prop = Niflect::MakeShared<CPropertyBitsArray>();
				else if (auto p = CMapAccessor::CastChecked(accessorParent))
					prop = Niflect::MakeShared<CPropertyMap>();
				else
					ASSERT(false);
			}
			else
			{
				prop = Niflect::MakeShared<CPropertyGroup>();
			}
		}

		if (prop != NULL)
		{
			SSSSSSSSContext ctxChild{ factory, rwParent, accessorParent };
			prop->BuildSelf(ctxChild);
		}
		return prop;
	}
	static void DebugPrintPropertyNodeRecurs(const CPropertyNode* propParent, uint32 level = 0)
	{
		auto strLevel = NiflectUtil::DebugIndentToString(level);
		printf("%s%s", strLevel.c_str(), propParent->m_name.c_str());
		if (auto propItem = CPropertyItem::CastChecked(propParent))
		{
			if (auto rw = propItem->m_rwData.Get())
			{
				if (rw->IsValue())
				{
					Niflect::CStringStream ss;
					CJsonFormat::Write(rw, ss);
					printf(", %s", ss.str().c_str());
				}
			}
		}
		printf("\n");
		level++;
		for (auto& it : propParent->m_vecNode)
			DebugPrintPropertyNodeRecurs(it.Get(), level);
	}

	class CPropertyBool : public CPropertyItem
	{
		typedef CPropertyItem inherited;
	public:
		virtual void BuildSelf(const SSSSSSSSContext& ctx) override
		{
			ASSERT(ctx.m_rw->IsValue());
			auto& value = ctx.m_rw->GetValue()->GetAs<bool>();
			m_rwData = CreateRwNode();
			m_rwData->ToValue()->SetAs<bool>(value);
		}
	};

	class CPropertyFloat : public CPropertyItem
	{
		typedef CPropertyItem inherited;
	public:
		virtual void BuildSelf(const SSSSSSSSContext& ctx) override
		{
			ASSERT(ctx.m_rw->IsValue());
			auto& value = ctx.m_rw->GetValue()->GetFloat();
			m_rwData = CreateRwNode();
			SetRwValueAs<float>(m_rwData->ToValue(), value);
		}
	};

	class CPropertyString : public CPropertyItem
	{
		typedef CPropertyItem inherited;
	public:
		virtual void BuildSelf(const SSSSSSSSContext& ctx) override
		{
			ASSERT(ctx.m_rw->IsValue());
			auto& value = ctx.m_rw->GetValue()->GetString();
			m_rwData = CreateRwNode();
			SetRwValueAs<Niflect::CString>(m_rwData->ToValue(), value);
		}
	};

	template <typename T2>
	static CSharedPropertyItem CreateVectorEntryProperty();

	template <>
	CSharedPropertyItem CreateVectorEntryProperty<float>()
	{
		return Niflect::MakeShared<CPropertyFloat>();
	}

	template <typename TPrecision>
	class TPropertyMyVectorN : public CPropertyItem
	{
		typedef CPropertyItem inherited;
	public:
		virtual void BuildSelf(const SSSSSSSSContext& ctx) override
		{
			printf("");
		}

	public:
		void AddEntry(const Niflect::CString& name, const TPrecision& val)
		{
			auto propX = CreateVectorEntryProperty<TPrecision>();
			propX->m_rwData = CreateRwNode();
			SetRwValueAs<TPrecision>(propX->m_rwData->ToValue(), val);
			propX->Init(name);
			this->AddNode(propX);
		}
	};

	template <typename TPrecision>
	class TPropertyMyTransform : public CPropertyItem
	{
		typedef CPropertyItem inherited;
	public:
		virtual void BuildSelf(const SSSSSSSSContext& ctx) override
		{
			m_rwData = CreateRwNode();
			TMyTransform<TPrecision> instance;
			auto accessorTransform = ctx.m_accessor->GetType()->CreateFieldLayout();
			accessorTransform->LoadFromRwNode(&instance, ctx.m_rw);
			accessorTransform->SaveToRwNode(&instance, m_rwData.Get());

			{
				auto propTranslation = Niflect::MakeShared<TPropertyMyVectorN<TPrecision> >();
				auto x = instance.m_translation.m_x;//可理解为某种解码
				propTranslation->AddEntry("Prop_X", x);
				auto y = instance.m_translation.m_y;
				propTranslation->AddEntry("Prop_Y", y);
				auto z = instance.m_translation.m_z;
				propTranslation->AddEntry("Prop_Z", z);
				propTranslation->Init("Prop_Translation");
				this->AddNode(propTranslation);
			}
			{
				auto propRotation = Niflect::MakeShared<TPropertyMyVectorN<TPrecision> >();
				auto x = instance.m_rotation.m_x;
				propRotation->AddEntry("Prop_X", x);
				auto y = instance.m_rotation.m_y;
				propRotation->AddEntry("Prop_Y", y);
				auto z = instance.m_rotation.m_z;
				propRotation->AddEntry("Prop_Z", z);
				auto w = instance.m_rotation.m_w;
				propRotation->AddEntry("Prop_W", w);
				propRotation->Init("Prop_Rotation");
				this->AddNode(propRotation);
			}
			{
				auto propScale = Niflect::MakeShared<TPropertyMyVectorN<TPrecision> >();
				auto x = instance.m_scale.m_x;//可理解为某种解码
				propScale->AddEntry("Prop_X", x);
				auto y = instance.m_scale.m_y;
				propScale->AddEntry("Prop_Y", y);
				auto z = instance.m_scale.m_z;
				propScale->AddEntry("Prop_Z", z);
				propScale->Init("Prop_Scale");
				this->AddNode(propScale);
			}
		}
	};

	static void TestAccessor0()
	{
		using namespace Engine;
		using namespace Niflect;

		//ArrayAccessor
		Niflect::CNiflectTable tableHolder;
		{
			auto table = &tableHolder;
			table->RegisterType2<Niflect::TArrayNif<float>, CClass>("Niflect::TArrayNif<float>", &CreateSSSSSSSSSSSS<Niflect::TArrayNif<float> >);
			table->RegisterType2<float, CClass>("float", &CreateSSSSSSSSSSSS<float>);
			table->InitTypesLayout();
		}

		auto type = StaticGetTypeOld<Niflect::TArrayNif<float> >();

		Niflect::TArrayNif<float> srcData;
		srcData.resize(2);
		srcData[0] = 1.2f;
		srcData[1] = 2.3f;
		CRwNode root;
		type->SaveInstanceToRwNode(&srcData, &root);
		Niflect::TArrayNif<float> dstData;
		type->LoadInstanceFromRwNode(&dstData, &root);
		ASSERT(srcData == dstData);
		for (auto& it : dstData)
			printf("%f\n", it);
		printf("");
	}
	static void TestAccessor1()
	{
		using namespace Engine;
		using namespace Niflect;

		Niflect::CNiflectTable tableHolder;
		{
			auto table = &tableHolder;
			table->RegisterType2<EMyOption, CEnum>("TestAccessor2::EMyOption", &CreateSSSSSSSSSSSS<EMyOption>);

			Niflect::CEnumMeta enumMeta;
			enumMeta.InitAddConst("None", NULL);
			enumMeta.InitAddConst("Default", NULL);
			enumMeta.InitAddConst("Mode 0", NULL);
			enumMeta.InitAddConst("Mode 1", NULL);
			auto et = Niflect::CEnum::Cast(StaticGetTypeOld<EMyOption>());
			et->InitEnumMeta(enumMeta);
		}

		auto type = Niflect::CEnum::Cast(StaticGetTypeOld<EMyOption>());
		auto accessor0 = type->CreateFieldLayout();

		EMyOption srcData = EMyOption::Mode1;
		CRwNode root;
		accessor0->SaveToRwNode(&srcData, &root);
		EMyOption dstData;
		accessor0->LoadFromRwNode(&dstData, &root);
		ASSERT(srcData == dstData);
		printf("%s\n", type->GetEnumConstName(dstData).c_str());
	}
	static void TestAccessor2()
	{
		using namespace Engine;
		using namespace Niflect;

		ASSERT(false);

		//static void InvokeMethod_MyFunc0(AddrType obj, AddrType * inputInstanceArray, AddrType * ouputInstanceArray, const TArrayNif<CNiflectType*>&vecInputType, const TArrayNif<CNiflectType*>&vecOutputType)
		//{
		//	auto& arg0 = vecInputType[0]->GetInstanceRef<float>(inputInstanceArray[0]);
		//	auto& arg1 = vecInputType[1]->GetInstanceRef<CMyClass0>(inputInstanceArray[1]);
		//	auto& arg2 = vecInputType[2]->GetInstanceRef<bool*>(inputInstanceArray[2]);
		//	auto& arg3 = vecInputType[3]->GetInstanceRef<CMyClass0*>(inputInstanceArray[3]);
		//	auto& arg4 = vecInputType[4]->GetInstanceRef<float**>(inputInstanceArray[4]);
		//	auto& ret0 = vecOutputType[0]->GetInstanceRef<bool>(ouputInstanceArray[0]);
		//	ret0 = static_cast<CMyClassBase0*>(obj)->MyFunc0(arg0, arg1, arg2, arg3, arg4);
		//}
		//static void StaticInitMethods()
		//{
		//	auto type = Cast<CClass>(StaticGetTypeOld<CMyClassBase0>());
		//	CNiflectMethod2 method;
		//	method.m_InvokeFunc = &InvokeMethod_MyFunc0;
		//	method.m_vecInputType.push_back(StaticGetTypeOld<CFloatField>());
		//	method.m_vecInputType.push_back(StaticGetTypeOld<CMyClass0>());
		//	method.m_vecInputType.push_back(StaticGetType_Misc<CPointerField, bool*>("bool*"));
		//	method.m_vecInputType.push_back(StaticGetType_Misc<CPointerField, CMyClass0*>("CMyClass0*"));
		//	method.m_vecInputType.push_back(StaticGetType_Misc<CPointerField, float**>("float**"));
		//	method.m_vecOutputType.push_back(StaticGetTypeOld<CBoolField>());
		//	type->m_vecMethod.push_back(method);
		//}

		//{
		//	using namespace MyTestClassScope;

		//	auto type = Cast<CClass>(StaticGetTypeOld<CMyClassBase0>());
		//	CMyClassBase0 obj;

		//	float a = 1.0f;
		//	CMyClass0 b;
		//	bool temp0 = true;
		//	bool* c = &temp0;
		//	CMyClass0 temp1;
		//	temp1.m_float_1 = 2.0f;
		//	CMyClass0* d = &temp1;
		//	float temp2_0[] = { 3.0f, 4.0f, 5.0f, 6.0f };
		//	float temp2_1[] = { 7.0f, 8.0f, 9.0f, 10.0f };
		//	float temp2_2[] = { 11.0f, 12.0f, 13.0f, 14.0f };
		//	float* temp2[3] = { temp2_0, temp2_1, temp2_2 };
		//	//实验时略有误解, 以下这种定义为连续内存, 访问是通过float*, 而不是float**
		//	//float temp2[3][4] = {{3.0f, 4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f, 10.0f}, {11.0f, 12.0f, 13.0f, 14.0f}};
		//	auto e = &temp2;
		//	void* inputInstanceArray[] = { &a, &b, &c, &d, &e };
		//	bool ret = false;
		//	void* outputInstanceArray[] = { &ret };
		//	type->m_vecMethod[0].Invoke(&obj, inputInstanceArray, outputInstanceArray);
		//	ASSERT(ret);
		//	printf("");
		//}

		Niflect::CNiflectTable tableHolder;
		{
			auto table = &tableHolder;
			table->RegisterType2<EMyOption, CEnum>("TestAccessor2::EMyOption", &CreateSSSSSSSSSSSS<EMyOption>);

			Niflect::CEnumMeta enumMeta;
			enumMeta.InitAddConst("None", NULL);
			enumMeta.InitAddConst("Default", NULL);
			enumMeta.InitAddConst("Mode 0", NULL);
			enumMeta.InitAddConst("Mode 1", NULL);
			auto et = Niflect::CEnum::Cast(StaticGetTypeOld<EMyOption>());
			et->InitEnumMeta(enumMeta);
		}

		auto type = Niflect::CEnum::Cast(StaticGetTypeOld<EMyOption>());
		auto accessor0 = type->CreateFieldLayout();

		EMyOption srcData = EMyOption::Mode1;
		CRwNode root;
		accessor0->SaveToRwNode(&srcData, &root);
		EMyOption dstData;
		accessor0->LoadFromRwNode(&dstData, &root);
		ASSERT(srcData == dstData);
		printf("%s\n", type->GetEnumConstName(dstData).c_str());
	}
	static void TestProperty0()
	{
		using namespace Engine;
		using namespace Niflect;

		Niflect::CNiflectTable tableHolder;
		{
			auto table = &tableHolder;
			table->RegisterType2<float, CNiflectType>("float", &CreateFieldLayoutForfFunctionPointerrrrrrrr<CFloatAccessor, float>);
			table->RegisterType2<bool, CNiflectType>("bool", &CreateFieldLayoutForfFunctionPointerrrrrrrr<CBoolAccessor, bool>);
			table->RegisterType2<Niflect::CString, CNiflectType>("Niflect::CString", &CreateFieldLayoutForfFunctionPointerrrrrrrr<CStringAccessor, Niflect::CString>);
			table->RegisterType2<TMyTransform<float>, CClass>("TMyTransform<float>", &CreateFieldLayoutForfFunctionPointerrrrrrrr<TMyTransformAccessor<float>, TMyTransform<float> >);
		}
		{
			auto table = &tableHolder;
			table->RegisterType2<Niflect::TArrayNif<float>, CClass>("Niflect::TArrayNif<float>", &CreateSSSSSSSSSSSS<Niflect::TArrayNif<float> >);
			table->RegisterType2<Niflect::TArrayNif<Niflect::TArrayNif<float> >, CClass>("Niflect::TArrayNif<Niflect::TArrayNif<float> >", &CreateSSSSSSSSSSSS<Niflect::TArrayNif<Niflect::TArrayNif<float> > >);
			table->RegisterType2<Niflect::TArrayNif<bool>, CClass>("Niflect::TArrayNif<bool>", &CreateSSSSSSSSSSSS<Niflect::TArrayNif<bool> >);
			table->RegisterType2<std::vector<bool>, CClass>("std::vector<bool>", &CreateSSSSSSSSSSSS<std::vector<bool> >);
			table->RegisterType2<std::pair<Niflect::CString, float>, CStruct>("std::pair<Niflect::CString, float>", &CreateSSSSSSSSSSSS<std::pair<Niflect::CString, float> >);
			table->RegisterType2<Niflect::TMap<Niflect::CString, float>, CClass>("Niflect::TMap<Niflect::CString, float>", &CreateSSSSSSSSSSSS<Niflect::TMap<Niflect::CString, float> >);
			table->RegisterType2<Niflect::TArrayNif<Niflect::CString>, CClass>("Niflect::TArrayNif<Niflect::CString>", &CreateSSSSSSSSSSSS<Niflect::TArrayNif<Niflect::CString> >);
			table->RegisterType2<Niflect::TArrayNif<TMyTransform<float> >, CClass>("Niflect::TArrayNif<TMyTransform<float> >", &CreateSSSSSSSSSSSS<Niflect::TArrayNif<TMyTransform<float> > >);
		}

		{
			auto table = &tableHolder;
			table->RegisterType2<CTestClassMy, CClass>("CTestClassMy", &SSSSSSSSSS_CTestClassMy);
			table->RegisterType2<CTestClassMy2, CClass>("CTestClassMy2", &SSSSSSSSSS_CTestClassMy2);
		}

		{
			auto type = StaticGetTypeOld<CTestClassMy>();
			auto accessor0 = type->CreateFieldLayout();

			CTestClassMy srcData;
			srcData.TestInit();
			CRwNode root;
			accessor0->SaveToRwNode(&srcData, &root);
			CTestClassMy dstData;
			accessor0->LoadFromRwNode(&dstData, &root);
			ASSERT(srcData == dstData);
			printf("");
		}
		{
			auto type0 = StaticGetTypeOld<CTestClassMy>();
			auto accessor0 = type0->CreateFieldLayout();
			auto type1 = StaticGetTypeOld<CTestClassMy2>();
			auto accessor1 = type1->CreateFieldLayout();

			CTestClassMy2 srcData;
			srcData.TestInit();
			CRwNode root;
			accessor0->SaveToRwNode(&srcData, &root);
			accessor1->SaveToRwNode(&srcData, &root);
			CTestClassMy2 dstData;
			accessor0->LoadFromRwNode(&dstData, &root);
			accessor1->LoadFromRwNode(&dstData, &root);
			ASSERT(srcData == dstData);
			printf("");
		}
		{
			{
				auto table = &tableHolder;
				table->RegisterType2<CPropertyBool, CClass>("CPropertyBool", NULL);
				table->RegisterType2<CPropertyFloat, CClass>("CPropertyFloat", NULL);
				table->RegisterType2<CPropertyString, CClass>("CPropertyString", NULL);
				table->RegisterType2<TPropertyMyTransform<float>, CClass>("TPropertyMyTransform<float>", NULL);
			}

			{
				CPropertyNodeFactory factory;
				factory.Register(StaticGetTypeOld<bool>(), StaticGetTypeOld<CPropertyBool>());
				factory.Register(StaticGetTypeOld<float>(), StaticGetTypeOld<CPropertyFloat>());
				factory.Register(StaticGetTypeOld<Niflect::CString>(), StaticGetTypeOld<CPropertyString>());
				factory.Register(StaticGetTypeOld<TMyTransform<float> >(), StaticGetTypeOld<TPropertyMyTransform<float> >());

				CSharedAccessor accessorSrc;
				CRwNode rwSrc;
				{
					auto type0 = StaticGetTypeOld<CTestClassMy>();
					accessorSrc = type0->CreateFieldLayout();
					auto instance = NiflectTypeMakeShared<void*>(type0);
					auto& srcData = *reinterpret_cast<CTestClassMy*>(instance.Get());
					srcData.TestInit();
					accessorSrc->SaveToRwNode(&srcData, &rwSrc);
				}

				auto propDst = BuildPropertyTreeRecurs(factory, &rwSrc, accessorSrc.Get());
				//factory.BuildPropertyTreeRecurs(&rwSrc, accessorSrc.Get(), &propDst);
				DebugPrintPropertyNodeRecurs(propDst.Get());
				printf("");
			}
		}
	}

	void TestFieldLayout()
	{
		if (true)//if (false)
			TestAccessor0();
		if (false)
			TestAccessor1();
		if (false)
			TestAccessor2();
		if (false)//if (true)
			TestProperty0();
	}
}