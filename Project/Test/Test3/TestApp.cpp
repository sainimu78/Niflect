#include "TestApp.h"
#include "gtest/gtest.h"
#include "Niflect/Memory/Default/DefaultMemory.h"
#include "TestLib/TestLib.h"
#include "Test3_private.h"

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

TEST(TestAppAndLib, TestMain) {
    auto memTest = Niflect::GetDefaultMemoryStats();
    EXPECT_EQ(memTest->m_allocCount, 0);
    Niflect::CNiflectTable table0;
    InitTestLib(table0);
    Niflect::CNiflectTable table1;
    InitTestApp(table1);

    using namespace TestLibScope;

    auto type = Niflect::StaticGetType<CTestLib>();
    for (uint32 idx = 0; idx < 1000; ++idx)
    {
        float prm0 = 123.0f;
        Niflect::InstanceType* prms[] = { &prm0 };
        auto dummy = Niflect::GenericPlacementMakeShared<CTestBase, Niflect::CMemory>(
            type->GetTypeSize(),
            type->m_InvokeDestructorFunc,
            type->m_vecConstructorInfo[0].m_Func,
            prms);
        //auto& instance = *reinterpret_cast<CTestLib*>(dummy.Get());
        auto& instance = *dummy.Cast<CTestLib>();
        instance.m_array_float_3.push_back(1);
        instance.m_array_float_3.push_back(2);
        instance.m_array_float_3.push_back(3);
        {
            type->m_vecMethodInfo[0].m_Func(&instance, NULL);
        }
        {
            float prm0 = 456.0f;
            Niflect::InstanceType* prms[] = { &prm0 };
            type->m_vecMethodInfo[1].m_Func(&instance, prms);
        }
        {
            Niflect::CString prm0 = "bucuo";
            float ret0 = 0.0f;
            Niflect::InstanceType* prms[] = { &prm0, &ret0 };
            type->m_vecMethodInfo[2].m_Func(&instance, prms);
            printf("ret: %f\n", ret0);
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