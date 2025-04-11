#include "TestApp.h"
#include "gtest/gtest.h"
#include "Niflect/Memory/Default/DefaultMemory.h"
#include "TestLib/TestLib.h"
#include "Test3_private.h"
#include <array>
#include "Niflect/Serialization/JsonFormat.h"

static void InitTestApp(Niflect::CNiflectTable& table)
{
    Niflect::GeneratedRegisterTypes(&table);
    Niflect::GeneratedInitTypes();
    table.InitTypesLayout();
}

template <typename TTestType>
static void TestSerializationOf()
{
    using namespace RwTree;
    CRwNode rw;
    TTestType src;
    src.InitForTest();
    auto type = Niflect::StaticGetType<TTestType>();
    type->SaveInstanceToRwNode(&src, &rw);
    TTestType dst;
    type->LoadInstanceFromRwNode(&dst, &rw);
    EXPECT_TRUE(src == dst);
}

template <typename ...TArgs>
inline static void DebugNiflectInvokeMethod(const Niflect::CNiflectType* type, Niflect::InstanceType* base, TArgs&& ...args)
{
    std::array<Niflect::InstanceType*, sizeof ...(TArgs)> argArray = { (&args)... };
    type->m_vecMethodInfo[1].m_Func(base, argArray.data());
}

TEST(TestAppAndLib, TestMain) {
    auto memTest = Niflect::GetDefaultMemoryStats();
    EXPECT_EQ(memTest->m_allocCount, 0);
    Niflect::CNiflectTable table0;
    InitTestLib(table0);
    Niflect::CNiflectTable table1;
    InitTestApp(table1);

    {
        auto type = GetTestLibGlobalsType();
        float arg0 = 321.0f;
        Niflect::InstanceType* args[] = { &arg0 };
        auto& vecField = type->GetFields();
        TestLibSetGlobalVar0(222.0f);
        auto& field = vecField[0];
        RwTree::CRwNode rw;
        field.LayoutSaveToRwNode(NULL, &rw);
        Niflect::CStringStream ss;
        RwTree::CJsonFormat::Write(&rw, ss);
        printf("Global var: %s, %s\n", field.GetName().c_str(), ss.str().c_str());
        type->m_vecStaticMemberFunctionInfo[0].m_Func(args);
        arg0 = 654.0f;
        type->m_vecStaticMemberFunctionInfo[1].m_Func(args);
        printf("");
    }

    using namespace TestLibScope;

    auto type = Niflect::StaticGetType<CTestLib>();
    for (uint32 idx = 0; idx < 1000; ++idx)
    {
        auto dummy = Niflect::NiflectTypeMakeShared<CTestBase>(type, 123.0f);
        //auto& instance = *reinterpret_cast<CTestLib*>(dummy.Get());
        auto& instance = *dummy.Cast<CTestLib>();
        instance.m_array_float_3.push_back(1);
        instance.m_array_float_3.push_back(2);
        instance.m_array_float_3.push_back(3);
        {
            type->m_vecMethodInfo[0].m_Func(&instance, NULL);
        }
        {
            DebugNiflectInvokeMethod(type, &instance, 456.0f);
        }
        {
            Niflect::CString arg0 = "bucuo";
            float ret0 = 0.0f;
            Niflect::InstanceType* args[] = { &arg0, &ret0 };
            type->m_vecMethodInfo[2].m_Func(&instance, args);
            printf("ret: %f\n", ret0);
        }
        {
            float arg0 = 789.0f;
            Niflect::InstanceType* args[] = { &arg0 };
            type->m_vecStaticMemberFunctionInfo[0].m_Func(args);
        }

        printf("");
    }

    //TestSerializationOf<CTestLib>();
    TestSerializationOf<CTestApp>();
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}