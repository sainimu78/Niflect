#include "Engine/Test/TestAccessor.h"
#include "Niflect/Default/CompoundAccessor.h"
#include "Niflect/Default/ValueTypeAccessor.h"
#include "Niflect/Default/ContainerTypeAccessor.h"

namespace TestAccessor
{
	template <typename T>
	static Niflect::CString MyVectorToString(const TMyVector<T>& v)
	{
		std::basic_stringstream<Niflect::CString::value_type, Niflect::CString::traits_type, Niflect::CString::allocator_type> ss;
		ss << v.m_x << ',' << v.m_y << ',' << v.m_z;
		return ss.str();
	}
	template <typename T>
	static TMyVector<T> StringToMyVector(const Niflect::CString& str)
	{
		std::basic_stringstream<Niflect::CString::value_type, Niflect::CString::traits_type, Niflect::CString::allocator_type> ss(str);
		TMyVector<T> v;
		char d;
		ss >> v.m_x >> d >> v.m_y >> d >> v.m_z;
		return v;
	}
	template <typename T>
	static Niflect::CString MyQuaternionToString(const TMyQuaternion<T>& v)
	{
		std::basic_stringstream<Niflect::CString::value_type, Niflect::CString::traits_type, Niflect::CString::allocator_type> ss;
		ss << v.m_x << ',' << v.m_y << ',' << v.m_z << ',' << v.m_w;
		return ss.str();
	}
	template <typename T>
	static TMyQuaternion<T> StringToMyQuaternion(const Niflect::CString& str)
	{
		std::basic_stringstream<Niflect::CString::value_type, Niflect::CString::traits_type, Niflect::CString::allocator_type> ss(str);
		TMyQuaternion<T> v;
		char d;
		ss >> v.m_x >> d >> v.m_y >> d >> v.m_z >> d >> v.m_w;
		return v;
	}

}

namespace Engine
{
	using namespace TestAccessor;
	using namespace RwTree;

	template <typename T>
	class TMyTransformAccessor : public Niflect::CAccessor
	{
	public:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
		{
			auto& instance = *static_cast<const TMyTransform<T>*>(base);
			//序列化方式是任意的, 因此可认为支持自定义编码
			AddRwString(rw, "m_translation", MyVectorToString<T>(instance.m_translation));
			AddRwString(rw, "m_rotation", MyQuaternionToString<T>(instance.m_rotation));
			AddRwString(rw, "m_scale", MyVectorToString<T>(instance.m_scale));
			return true;
		}
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
		{
			auto& instance = *static_cast<TMyTransform<T>*>(base);
			//序列化方式是任意的, 因此可认为支持自定义编码
			instance.m_translation = StringToMyVector<T>(FindRwString(rw, "m_translation"));
			instance.m_rotation = StringToMyQuaternion<T>(FindRwString(rw, "m_rotation"));
			instance.m_scale = StringToMyVector<T>(FindRwString(rw, "m_scale"));
			return true;
		}
	};
}

namespace TestAccessor
{
#ifdef TEMP_DISABLED_FOR_REFACTORING_0
	void TestFieldLayout()
	{
	}
#else
	using namespace Engine;
	using namespace Niflect;

	static Niflect::CSharedAccessor BuildAccessor_CTestClassMy()
	{
		using namespace Engine;

		auto accessor0 = Niflect::MakeShared<Niflect::CCompoundAccessor>();
		{
			auto accessor1 = Niflect::MakeShared<CFloatAccessor>();
			accessor1->InitMemberMeta("m_float_0", Niflect::GetMemberVariableOffset(&CTestClassMy::m_float_0));
			accessor0->AddChild(accessor1);
		}
		{
			auto accessor1 = Niflect::MakeShared<TArrayAccessor<Niflect::TArrayNif<float> > >();
			accessor1->InitMemberMeta("m_array_1", Niflect::GetMemberVariableOffset(&CTestClassMy::m_array_1));
			{
				auto accessor2 = Niflect::MakeShared<CFloatAccessor>();
				accessor2->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
				accessor1->InitElementAccessor(accessor2);
			}
			accessor0->AddChild(accessor1);
		}
		{
			auto accessor1 = Niflect::MakeShared<CFloatAccessor>();
			accessor1->InitMemberMeta("m_float_2", Niflect::GetMemberVariableOffset(&CTestClassMy::m_float_2));
			accessor0->AddChild(accessor1);
		}
		{
			auto accessor1 = Niflect::MakeShared<CBoolAccessor>();
			accessor1->InitMemberMeta("m_bool_3", Niflect::GetMemberVariableOffset(&CTestClassMy::m_bool_3));
			accessor0->AddChild(accessor1);
		}
		{
			auto accessor1 = Niflect::MakeShared<TArrayAccessor<Niflect::TArrayNif<Niflect::TArrayNif<float> > > >();
			accessor1->InitMemberMeta("m_array_array_4", Niflect::GetMemberVariableOffset(&CTestClassMy::m_array_array_4));
			{
				auto accessor2 = Niflect::MakeShared<TArrayAccessor<Niflect::TArrayNif<float> > >();
				accessor2->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
				{
					auto accessor3 = Niflect::MakeShared<CFloatAccessor>();
					accessor3->InitMemberMeta("reserved_dim1", Niflect::CAddrOffset::None);
					accessor2->InitElementAccessor(accessor3);
				}
				accessor1->InitElementAccessor(accessor2);
			}
			accessor0->AddChild(accessor1);
		}
		{
			auto accessor1 = Niflect::MakeShared<TMyTransformAccessor<float> >();
			accessor1->InitMemberMeta("m_tm_5", Niflect::GetMemberVariableOffset(&CTestClassMy::m_tm_5));
			accessor0->AddChild(accessor1);
		}
		return accessor0;
	}
	static Niflect::CSharedAccessor BuildAccessor_CTestClassMy2()
	{
		using namespace Engine;

		auto accessor0 = Niflect::MakeShared<Niflect::CCompoundAccessor>();
		{
			auto accessor1 = Niflect::MakeShared<CBoolAccessor>();
			accessor1->InitMemberMeta("m_derived_bool_0", Niflect::GetMemberVariableOffset(&CTestClassMy2::m_derived_bool_0));
			accessor0->AddChild(accessor1);
		}
		{
			auto accessor1 = Niflect::MakeShared<TArrayAccessor<Niflect::TArrayNif<float> > >();
			accessor1->InitMemberMeta("m_derived_array_1", Niflect::GetMemberVariableOffset(&CTestClassMy2::m_derived_array_1));
			{
				auto accessor2 = Niflect::MakeShared<CFloatAccessor>();
				accessor2->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
				accessor1->InitElementAccessor(accessor2);
			}
			accessor0->AddChild(accessor1);
		}
		{
			auto accessor1 = Niflect::MakeShared<CFloatAccessor>();
			accessor1->InitMemberMeta("m_derived_float_2", Niflect::GetMemberVariableOffset(&CTestClassMy2::m_derived_float_2));
			accessor0->AddChild(accessor1);
		}
		{
			auto accessor1 = Niflect::MakeShared<TBitsArrayAccessor<Niflect::TArrayNif<bool> > >();
			accessor1->InitMemberMeta("m_derived_array_3", Niflect::GetMemberVariableOffset(&CTestClassMy2::m_derived_array_3));
			{
				auto accessor2 = Niflect::MakeShared<CBoolAccessor>();
				accessor2->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
				accessor1->InitElementAccessor(accessor2);
			}
			accessor0->AddChild(accessor1);
		}
		{
			auto accessor1 = Niflect::MakeShared<CBoolAccessor>();
			accessor1->InitMemberMeta("m_derived_bool_4", Niflect::GetMemberVariableOffset(&CTestClassMy2::m_derived_bool_4));
			accessor0->AddChild(accessor1);
		}
		{
			auto accessor1 = Niflect::MakeShared<TArrayAccessor<Niflect::TArrayNif<TMyTransform<float> > > >();
			accessor1->InitMemberMeta("m_derived_array_5", Niflect::GetMemberVariableOffset(&CTestClassMy2::m_derived_array_5));
			{
				auto accessor2 = Niflect::MakeShared<TMyTransformAccessor<float> >();
				accessor2->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
				accessor1->InitElementAccessor(accessor2);
			}
			accessor0->AddChild(accessor1);
		}
		return accessor0;
	}

	void TestFieldLayout()
	{
		if (false)//BuiltIn Float
		{
			using namespace Engine;
			auto accessor0 = Niflect::MakeShared<CFloatAccessor>();
			float srcData = 1.23f;
			CRwNode root;
			accessor0->SaveToRwNode(&srcData, &root);
			float dstData = 0.0f;
			accessor0->LoadFromRwNode(&dstData, &root);
			ASSERT(srcData == dstData);
			printf("%f\n", dstData);
		}
		if (false)//数组
		{
			using namespace Engine;
			auto accessor0 = Niflect::MakeShared<TArrayAccessor<Niflect::TArrayNif<float> > >();
			{
				auto accessor1 = Niflect::MakeShared<CFloatAccessor>();
				accessor1->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
				accessor0->InitElementAccessor(accessor1);
			}
			Niflect::TArrayNif<float> srcData;
			srcData.resize(2);
			srcData[0] = 1.2f;
			srcData[1] = 2.3f;
			CRwNode root;
			accessor0->SaveToRwNode(&srcData, &root);
			Niflect::TArrayNif<float> dstData;
			accessor0->LoadFromRwNode(&dstData, &root);
			ASSERT(srcData == dstData);
			for (auto& it : dstData)
				printf("%f\n", it);
			printf("");
		}
		if (false)//Map
		{
			using namespace Engine;
			auto accessor0 = Niflect::MakeShared<TMapAccessor<Niflect::TMap<Niflect::CString, float> > >();
			{
				auto accessor1 = Niflect::MakeShared<Niflect::CCompoundAccessor>();
				accessor1->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
				{
					{
						auto accessor2 = Niflect::MakeShared<CStringAccessor>();
						accessor2->InitMemberMeta("first", Niflect::GetMemberVariableOffset(&std::pair<const Niflect::CString, float>::first));
						accessor1->AddChild(accessor2);
					}
					{
						auto accessor2 = Niflect::MakeShared<CFloatAccessor>();
						accessor2->InitMemberMeta("second", Niflect::GetMemberVariableOffset(&std::pair<const Niflect::CString, float>::second));
						accessor1->AddChild(accessor2);
					}
				}
				accessor0->InitElementAccessor(accessor1);
			}
			Niflect::TMap<Niflect::CString, float> srcData;
			srcData["Nihao"] = 1.2f;
			srcData["Bucuo"] = 3.4f;
			CRwNode root;
			accessor0->SaveToRwNode(&srcData, &root);
			Niflect::TMap<Niflect::CString, float> dstData;
			accessor0->LoadFromRwNode(&dstData, &root);
			ASSERT(srcData == dstData);
			for (auto& it : dstData)
				printf("%s, %f\n", it.first.c_str(), it.second);
			printf("");
		}
		if (false)//自定义序列化
		{
			using namespace Engine;
			auto accessor0 = Niflect::MakeShared<TMyTransformAccessor<float> >();
			TMyTransform<float> srcData;
			TestInit_TMyTransform(srcData);
			CRwNode root;
			accessor0->SaveToRwNode(&srcData, &root);
			TMyTransform<float> dstData;
			accessor0->LoadFromRwNode(&dstData, &root);
			ASSERT(srcData == dstData);
			printf("");
		}
		if (false)//STL中, 被特殊处理的 bool 数组
		{
			using namespace Engine;
			auto accessor0 = Niflect::MakeShared<TBitsArrayAccessor<Niflect::TArrayNif<bool> > >();
			{
				//todo: 但此测试未实现特殊处理, 和一般数组一样 InitElementAccessor 是错误的
				auto accessor1 = Niflect::MakeShared<CBoolAccessor>();
				accessor1->InitMemberMeta("reserved_dim0", Niflect::CAddrOffset::None);
				accessor0->InitElementAccessor(accessor1);
			}
			Niflect::TArrayNif<bool> srcData;
			srcData.resize(2);
			srcData[0] = false;
			srcData[1] = true;
			CRwNode root;
			accessor0->SaveToRwNode(&srcData, &root);
			Niflect::TArrayNif<bool> dstData;
			accessor0->LoadFromRwNode(&dstData, &root);
			ASSERT(srcData == dstData);
			for (const auto& it : dstData)
				printf("%s\n", it ? "true" : "false");
			printf("");
		}
		if (false)//简单类
		{
			using namespace Engine;
			auto accessor0 = BuildAccessor_CTestClassMy();
			CTestClassMy srcData;
			srcData.TestInit();
			CRwNode root;
			accessor0->SaveToRwNode(&srcData, &root);
			CTestClassMy dstData;
			accessor0->LoadFromRwNode(&dstData, &root);
			ASSERT(srcData == dstData);
			printf("");
		}
		if (false)//继承类
		{
			using namespace Engine;
			auto accessor0 = BuildAccessor_CTestClassMy();
			auto accessor1 = BuildAccessor_CTestClassMy2();
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
	}
#endif
}