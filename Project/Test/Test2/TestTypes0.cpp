#include "TestTypes0.h"
#include "gtest/gtest.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "Niflect/NiflectType.h"

void TestTypes0()
{
    using namespace RwTree;
    CRwNode rw;
    CPublicFields src;
    src.InitForTest();
    auto type = Niflect::StaticGetType<CPublicFields>();
    type->SaveInstanceToRwNode(&src, &rw);
    CPublicFields dst;
    type->LoadInstanceFromRwNode(&dst, &rw);
    EXPECT_TRUE(src == dst);
}