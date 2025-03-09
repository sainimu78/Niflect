#include "gtest/gtest.h"
#include "Test2_private.h"
#include "Niflect/Memory/Default/DefaultMemory.h"

extern void TestTypes0();

typedef void (*UnitTestFunc)();
static void InvokeTest(UnitTestFunc Func)
{
    auto memTest = Niflect::GetDefaultMemoryStats();
    EXPECT_EQ(memTest->m_allocCount, 0);
    {
        Niflect::CNiflectTable table;
        EXPECT_GT(memTest->m_allocCount, 0);
        Niflect::GeneratedRegisterTypes(&table);
        Niflect::GeneratedInitTypes();
        table.InitTypesLayout();

        Func();
    }
    EXPECT_EQ(memTest->m_bytesRuntime, 0);
}

TEST(TestGenTool, Test0) {
    InvokeTest(&TestTypes0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}