#include "gtest/gtest.h"
#include "Test2_private.h"
#include "Niflect/Memory/Default/DefaultMemory.h"

extern void TestTypes0();

TEST(SaveLoad, GenToolTest) {
    auto memTest = Niflect::GetDefaultMemoryStats();
    EXPECT_EQ(memTest->m_allocCount, 0);
    {
        Niflect::CNiflectTable table;
        EXPECT_GT(memTest->m_allocCount, 0);
        Niflect::GeneratedInitialReg(&table);
        Niflect::GeneratedInitTypes();
        table.InitTypesLayout();

        TestTypes0();
    }
    EXPECT_EQ(memTest->m_bytesRuntime, 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}