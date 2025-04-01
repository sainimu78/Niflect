#pragma once
#include "TestModule1/TestClass1_gen.h"

namespace TestModule1
{
	NIF_T()
	struct SMyRecord
	{
		NIF_F()
		float m_float_0;
		NIF_F()
		bool m_bool_1;
		NIF_F()
		int32 m_int_2;
	};
	static bool operator==(const SMyRecord& a, const SMyRecord& b)
	{
		return a.m_float_0 == b.m_float_0
			&& a.m_bool_1 == b.m_bool_1
			&& a.m_int_2 == b.m_int_2
			;
	}
	static void InitForTest_SMyRecord(SMyRecord& rec)
	{
		rec.m_float_0 = 123.0f;
		rec.m_bool_1 = false;
		rec.m_int_2 = 456;
	}

	class CTestBase1;
	using CSharedTestBase1 = Niflect::TSharedPtr<CTestBase1>;

#define TESTBASE1_SELF_INSTANCE_IDENTIFIER "self string 0"

	NIF_T()
	class CTestBase1
	{
		GENERATED_BODY()
	public:
		CTestBase1()
			: m_base_bool_1(false)
		{

		}
		~CTestBase1()
		{

		}
		void InitForTest()
		{
			m_base_string_0 = "base string 0";
			m_base_bool_1 = true;
			m_base_std_string_2 = "base std string 2";
			m_array_map_string_float_3.resize(3);
			m_array_map_string_float_3[0].insert({ "base nihao", 1.0f });
			m_array_map_string_float_3[0].insert({ "base bucuo", 1.1f });
			m_array_map_string_float_3[1].insert({ "base shima", 1.2f });
			m_array_map_string_float_3[1].insert({ "base zhende", 1.3f });
			m_array_map_string_float_3[2].insert({ "base buxin", 1.4f });
			m_array_map_string_float_3[2].insert({ "base ladao", 1.5f });
			m_shared_self_instance_4 = Niflect::MakeShared<CTestBase1>();
			m_shared_self_instance_4->m_base_string_0 = TESTBASE1_SELF_INSTANCE_IDENTIFIER;
			m_shared_self_instance_4->m_base_bool_1 = true;
			m_shared_self_instance_4->m_base_std_string_2 = "self std string 2";
			m_shared_self_instance_4->m_array_map_string_float_3.resize(2);
			m_shared_self_instance_4->m_array_map_string_float_3[0].insert({ "self nihao", 2.0f });
			m_shared_self_instance_4->m_array_map_string_float_3[0].insert({ "self bucuo", 2.1f });
			m_shared_self_instance_4->m_array_map_string_float_3[1].insert({ "self shima", 2.2f });
			m_shared_self_instance_4->m_array_map_string_float_3[1].insert({ "self zhende", 2.3f });
		}
		bool operator==(const CTestBase1& rhs) const
		{
			return m_base_string_0 == rhs.m_base_string_0
				&& m_base_bool_1 == rhs.m_base_bool_1
				&& m_base_std_string_2 == rhs.m_base_std_string_2
				&& m_array_map_string_float_3 == rhs.m_array_map_string_float_3
				&& m_shared_self_instance_4->m_base_string_0 == rhs.m_shared_self_instance_4->m_base_string_0
				&& m_shared_self_instance_4->m_base_bool_1 == rhs.m_shared_self_instance_4->m_base_bool_1
				&& m_shared_self_instance_4->m_base_std_string_2 == rhs.m_shared_self_instance_4->m_base_std_string_2
				&& m_shared_self_instance_4->m_array_map_string_float_3 == rhs.m_shared_self_instance_4->m_array_map_string_float_3
				;
		}

	public:
		NIF_F()
		Niflect::CString m_base_string_0;

	private:
		NIF_F()
		bool m_base_bool_1;
		NIF_F()
		std::string m_base_std_string_2;
		NIF_F()
		std::vector<std::map<std::string, float> > m_array_map_string_float_3;

	public:
		NIF_F()
		TestModule1::CSharedTestBase1 m_shared_self_instance_4;
	};

	class CMyEnumConstNata : public Niflect::CNata
	{
		typedef CMyEnumConstNata CThis;
	public:
		CThis& SetOptionName(const Niflect::CString& name)
		{
			m_friendlyName = name;
			return *this;
		}

	public:
		static CMyEnumConstNata* Cast(Niflect::CNata* base)
		{
			ASSERT(dynamic_cast<CMyEnumConstNata*>(base) != NULL);
			return static_cast<CMyEnumConstNata*>(base);
		}

		Niflect::CString m_friendlyName;
	};

	NIF_T()
	enum class ETestEnum0
	{
		MyDefault NIF_C(),
		Option0 NIF_C(CMyEnumConstNata().SetOptionName("Nihao opt 0")),
		Option1,
		Option2 NIF_C(),
	};

	NIF_T()
	class CTestClass1 : public CTestBase1
	{
		GENERATED_BODY()
		typedef CTestBase1 inherited;
	public:
		CTestClass1()
			: m_bool_1(false)
			, m_bool_2(false)
			, m_bool_4(false)
			, m_bool_6(false)
			, m_float_7(0.0f)
		{

		}
		~CTestClass1()//不用virtual, 用于验证是否正确使用本类型的析构
		{

		}
		void InitForTest()
		{
			inherited::InitForTest();
			m_std_string_0 = "std string 0";
			m_bool_1 = true;
			m_bool_2 = false;
			m_std_array_string_3.resize(3);
			m_std_array_string_3[0] = "std array string[0] nihao";
			m_std_array_string_3[1] = "std array string[1] nihao + bucuo";
			m_std_array_string_3[2] = "std array string[2] nihao + bucuo + shima";
			m_bool_4 = true;
			auto ret = m_std_map_string_array_float_5.insert({ "std nihao", std::vector<float>() });
			ret.first->second.push_back(1.0f);
			ret.first->second.push_back(1.1f);
			ret.first->second.push_back(1.2f);
			ret = m_std_map_string_array_float_5.insert({ "std bucuo", std::vector<float>() });
			ret.first->second.push_back(2.0f);
			ret.first->second.push_back(2.1f);
			ret.first->second.push_back(2.2f);
			ret.first->second.push_back(2.3f);
			ret.first->second.push_back(2.4f);
			m_bool_6 = false;
			m_float_7 = 1.0f;
			m_shared_base1_instance_8 = Niflect::MakeShared<CTestBase1>();
			m_shared_base1_instance_8->InitForTest();
			m_my_class_in_ph_9.InitForTest();
		}
		bool operator==(const CTestClass1& rhs) const
		{
			return inherited::operator==(rhs)
				&& m_std_string_0 == rhs.m_std_string_0
				&& m_bool_1 == rhs.m_bool_1
				&& m_bool_2 == rhs.m_bool_2
				&& m_std_array_string_3 == rhs.m_std_array_string_3
				&& m_bool_4 == rhs.m_bool_4
				&& m_std_map_string_array_float_5 == rhs.m_std_map_string_array_float_5
				&& m_bool_6 == rhs.m_bool_6
				&& m_float_7 == rhs.m_float_7
				&& (*m_shared_base1_instance_8) == (*rhs.m_shared_base1_instance_8)
				&& m_my_class_in_ph_9 == rhs.m_my_class_in_ph_9
				;
		}

	private:
		NIF_F()
		std::string m_std_string_0;
		NIF_F()
		bool m_bool_1;
		NIF_F()
		bool m_bool_2;
		NIF_F()
		std::vector<std::string> m_std_array_string_3;
		NIF_F()
		bool m_bool_4;
		NIF_F()
		std::map<std::string, std::vector<float> > m_std_map_string_array_float_5;
		NIF_F()
		bool m_bool_6;
		NIF_F()
		float m_float_7;
		NIF_F()
		TestModule1::CSharedTestBase1 m_shared_base1_instance_8;
		NIF_F()
		TestModule1::CMyClassInPrecompileHeader m_my_class_in_ph_9;
	};
}