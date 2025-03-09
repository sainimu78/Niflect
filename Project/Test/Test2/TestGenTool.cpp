#include "gtest/gtest.h"
#include "Test2_private.h"
#include "Niflect/Memory/Default/DefaultMemory.h"

extern void TestTypes0();
extern void TestTypes1();

typedef void (*UnitTestFunc)();
static void InvokeTest(UnitTestFunc Func)
{
    auto memTest = Niflect::GetDefaultMemoryStats();
    {
        Niflect::CNiflectTable table;
        Niflect::GeneratedRegisterTypes(&table);
        Niflect::GeneratedInitTypes();
        table.InitTypesLayout();
        Func();
    }
    EXPECT_EQ(memTest->m_bytesRuntime, 0);
}

TEST(TestGenTool, Test0) {
    auto memTest = Niflect::GetDefaultMemoryStats();
    EXPECT_EQ(memTest->m_allocCount, 0);
    InvokeTest(&TestTypes0);
}

TEST(TestGenTool, Test1) {
    InvokeTest(&TestTypes1);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}