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
    TestSerializationOf<CTestLib>();
    TestSerializationOf<CTestApp>();
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}