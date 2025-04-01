#include "Engine/Test/TestRegistration.h"
#include "Niflect/NiflectRegistrationOld.h"
#include "Niflect/Default/CompoundAccessor.h"
#include "Niflect/Default/ValueTypeAccessor.h"
#include "Niflect/Default/ContainerTypeAccessor.h"
#include "Engine/Engine.h"
#include "Niflect/Memory/Default/DefaultMemory.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "Engine/Test/TestStaticGetType.h"

namespace Niflect
{
	template <typename TType, typename TAccessor, typename TInfo>
	static TInfo* GetOrRegisterType(CNiflectTable* table, const Niflect::CString& typeName)
	{
		if (!TRegisteredType<TType>::IsValid())
		{
			ASSERT(!typeName.empty());
			table->RegisterType<TInfo, TType>(typeName, &__InternalCreateFieldLayoutForFunctionPointer<TAccessor>);
		}
		return Niflect::StaticGetTypeOld<TType>();
	}
}

namespace Niflect
{
	//template <typename T>
	//CNiflectType* StaticGetType()
	//{
	//	return TInternalRegisteredType<T>::s_type;
	//}

	//template <>
	//NIFLECTTYPEREG_API CNiflectType* StaticGetTypeOld<float>()
	//{
	//	return TInternalRegisteredType<float>::s_type;
	//}
	//template <>
	//NIFLECTTYPEREG_API CNiflectType* StaticGetTypeOld<bool>()
	//{
	//	return TInternalRegisteredType<bool>::s_type;
	//}
	//template <>
	//NIFLECTTYPEREG_API CNiflectType* StaticGetTypeOld<Niflect::TArrayNif<float> >()
	//{
	//	return TInternalRegisteredType<Niflect::TArrayNif<float> >::s_type;
	//}
	//template <>
	//NIFLECTTYPEREG_API CNiflectType* StaticGetTypeOld<TestRegistration::CMyRegClass>()
	//{
	//	return TInternalRegisteredType<TestRegistration::CMyRegClass>::s_type;
	//}

	template <>
	CNiflectType* StaticGetTypeOld<TestRegistration::CMyRegClass>()
	{
		return TRegisteredType<TestRegistration::CMyRegClass>::s_type;
	}
}

namespace TestRegistration
{
	static Niflect::CNiflectTable* GetSSSSSS()
	{
		static Niflect::CNiflectTable* s_table = NULL;
		if (s_table == NULL)
		{
			auto reg = Niflect::CNiflectRegistration::GetMutable();
			s_table = reg->AddNewTable();
		}
		return s_table;
	}

	//NIFLECT_REGISTER(Niflect::CClass, CMyRegClass, GetSSSSSS(), Niflect::CTypeCustomData())

	static Niflect::CSharedAccessor SSSSCreateFieldLayout()
	{
		auto node0 = Niflect::MakeShared<Niflect::CCompoundAccessor>();
		node0->InitType(Niflect::StaticGetTypeOld<CMyRegClass>());
		{
			auto type = Niflect::StaticGetTypeOld<float>();
			auto node1 = type->CreateFieldLayout();
			node1->InitMemberMeta("m_float_0", Niflect::GetMemberVariableOffset(&CMyRegClass::m_float_0));
			node0->AddChild(node1);
		}
		{
			auto type = Niflect::StaticGetTypeOld<bool>();
			auto node1 = type->CreateFieldLayout();
			node1->InitMemberMeta("m_bool_1", Niflect::GetMemberVariableOffset(&CMyRegClass::m_bool_1));
			node0->AddChild(node1);
		}
		return node0;
	}

	class CMyRegClassNata : public Niflect::CNata
	{
		typedef CMyRegClassNata CThis;
	public:
		CMyRegClassNata()
			: m_exampleValue(0.0f)
		{

		}
		CThis& SetExampleOption(float val)
		{
			m_exampleValue = val;
			return *this;
		}

	public:
		static CMyRegClassNata* Cast(Niflect::CNata* base)
		{
			ASSERT(dynamic_cast<CMyRegClassNata*>(base) != NULL);
			return static_cast<CMyRegClassNata*>(base);
		}

	public:
		float m_exampleValue;
	};

//#define ENABLED_TEST_STATIC_REG

#ifdef ENABLED_TEST_STATIC_REG
	Niflect::TStaticTableTypeReg<Niflect::CClass, CMyRegClass> ss(GetSSSSSS(), "CMyRegClass", &SSSSCreateFieldLayout, CMyRegClassNatimeta().SetExampleOption(1.23f));
#endif

	void TestTypeNatimeta()
	{
		using namespace Niflect;

		if (false)//Initial Reg (模块初始化注册), 基本类型
		{
			CDefaultMemoryStatsScope scope;
			auto memTest = GetDefaultMemoryStats();

			CNiflectTable table;
			auto type = GetOrRegisterType<float, CFloatAccessor, CNiflectType>(&table, "float");
			auto layout = type->CreateFieldLayout();
			auto sharedSrcData = NiflectTypeMakeShared<float>(type);
			auto& srcData = *sharedSrcData;
			srcData = 1.23f;

			CRwNode rw;
			layout->SaveToRwNode(&srcData, &rw);
			CStringStream ss;
			CJsonFormat::Write(&rw, ss);

			auto sharedDstData = NiflectTypeMakeShared<void*>(type);
			auto& dstData = *reinterpret_cast<float*>(sharedDstData.Get());
			layout->LoadFromRwNode(&dstData, &rw);

			ASSERT(srcData == dstData);

			printf("%f\n", dstData);
			printf("");
		}
		if (false)//Initial Reg (模块初始化注册), 数组
		{
			CDefaultMemoryStatsScope scope;
			auto memTest = GetDefaultMemoryStats();

			CNiflectTable table;
			auto type = GetOrRegisterType<Niflect::TArrayNif<float>, TArrayAccessor<Niflect::TArrayNif<float> >, CNiflectType>(&table, "Niflect::TArrayNif<float>");
			auto layout = type->CreateFieldLayout();
			{
				auto type = GetOrRegisterType<float, CFloatAccessor, CNiflectType>(&table, "float");
				auto elemLayout = type->CreateFieldLayout();
				layout->InitElementAccessor(elemLayout);
			}
			auto sharedSrcData = NiflectTypeMakeShared<void*>(type);
			auto& srcData = *reinterpret_cast<Niflect::TArrayNif<float>*>(sharedSrcData.Get());
			for (uint32 idx = 0; idx < 123; ++idx)
				srcData.push_back(static_cast<float>(idx));

			CRwNode rw;
			layout->SaveToRwNode(&srcData, &rw);
			CStringStream ss;
			CJsonFormat::Write(&rw, ss);

			auto sharedDstData = NiflectTypeMakeShared<void*>(type);
			auto& dstData = *reinterpret_cast<Niflect::TArrayNif<float>*>(sharedDstData.Get());
			layout->LoadFromRwNode(&dstData, &rw);

			ASSERT(srcData == dstData);

			printf("cnt: %u\n", static_cast<uint32>(dstData.size()));
			for (auto& it : dstData)
				printf("%f\n", it);
			printf("");
		}
		if (false)//Static Reg (静态注册) 与 Initial Reg (模块初始化注册)
		{
#ifdef ENABLED_TEST_STATIC_REG
#else
			ASSERT(false);//应先启用宏 ENABLED_TEST_STATIC_REG
#endif

			//TestCreateModuleReg0();

			auto table = GetSSSSSS();
			//GetOrRegisterType<Niflect::TArrayNif<float>, TStlArrayAccessor<Niflect::TArrayNif<float> >, CNiflectType>(table, "Niflect::TArrayNif<float>");
			//GetOrRegisterType<Niflect::TArrayNif<float>, TStlArrayAccessor<Niflect::TArrayNif<float> >, CNiflectType>(table, "Niflect::TArrayNif<float>");
			
			{
				CDefaultMemoryStatsScope scope;
				auto memTest = GetDefaultMemoryStats();

				{
					CDefaultMemoryStatsScope::CDisabled globalScope(scope);//对于仅 Initial Reg 是不需要 Disabled 的, Static Reg 所在的 Scope 为 MakeSureInitMemoryStats 中的静态 StatsScope (即全局 StatsScope)
					auto memTest2 = GetDefaultMemoryStats();
					GetOrRegisterType<float, CFloatAccessor, CNiflectType>(table, "float");
					GetOrRegisterType<bool, CBoolAccessor, CNiflectType>(table, "bool");
				}

				auto type = Niflect::StaticGetTypeOld<CMyRegClass>();

				auto nata = CMyRegClassNata::Cast(type->GetNata());
				printf("Nata, exampleValue: %f\n", nata->m_exampleValue);

				auto layout = type->CreateFieldLayout();
				auto sharedSrcData = NiflectTypeMakeShared<void*>(type);
				auto& srcData = *reinterpret_cast<CMyRegClass*>(sharedSrcData.Get());
				srcData.TestInit();

				CRwNode rw;
				layout->SaveToRwNode(&srcData, &rw);
				CStringStream ss;
				CJsonFormat::Write(&rw, ss);

				auto sharedDstData = NiflectTypeMakeShared<void*>(type);
				auto& dstData = *reinterpret_cast<CMyRegClass*>(sharedDstData.Get());
				layout->LoadFromRwNode(&dstData, &rw);

				ASSERT(srcData == dstData);
				printf("%s\n", ss.str().c_str());

				SSSSSSSS();
			}
		}
		printf("");
	}
}