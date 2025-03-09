#pragma once
#include "gtest/gtest.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "Niflect/NiflectType.h"

template <typename TTestType>
static void TestSerializationTypeBased()
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