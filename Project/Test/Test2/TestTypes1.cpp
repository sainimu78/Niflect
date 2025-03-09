#include "TestTypes1.h"
#include "gtest/gtest.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "Niflect/NiflectType.h"

void TestTypes1()
{
    using namespace RwTree;
    CRwNode rw;
    CPrivateFields src;
    src.InitForTest();
    auto type = Niflect::StaticGetType<CPrivateFields>();
    type->SaveInstanceToRwNode(&src, &rw);
    CPrivateFields dst;
    type->LoadInstanceFromRwNode(&dst, &rw);
    EXPECT_TRUE(src == dst);
}