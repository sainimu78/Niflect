#include "gtest/gtest.h"
#include "Test2_private.h"
#include "TestGenTool.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "Niflect/Memory/Default/DefaultMemory.h"

TEST(SaveLoad, GenToolTest) {
    auto memTest = Niflect::GetDefaultMemoryStats();
    EXPECT_EQ(memTest->m_allocCount, 0);
    {
        Niflect::CNiflectTable table;
        EXPECT_GT(memTest->m_allocCount, 0);
        Niflect::GeneratedInitialReg(&table);
        Niflect::GeneratedInitTypes();
        table.InitTypesLayout();

        using namespace RwTree;
        CRwNode rw;
        CTest2Class src;
        auto type = Niflect::StaticGetType<CTest2Class>();
        type->SaveInstanceToRwNode(&src, &rw);
        CTest2Class dst;
        type->LoadInstanceFromRwNode(&dst, &rw);
        EXPECT_TRUE(src == dst);
    }
    EXPECT_EQ(memTest->m_bytesRuntime, 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}