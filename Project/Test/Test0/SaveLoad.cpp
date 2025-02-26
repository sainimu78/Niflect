#include "gtest/gtest.h"
#include "Niflect/Serialization/JsonFormat.h"

TEST(SaveLoad, ExampleJson) {
    using namespace RwTree;
    Niflect::CStringStream ssA;
    {
        CRwNode rw;
        AddRwFloat(&rw, "nihao", 1.23f);
        CJsonFormat::Write(&rw, ssA);
    }
    Niflect::CStringStream ssB;
    {
        CRwNode rw;
        CJsonFormat::Read(&rw, ssA);
        CJsonFormat::Write(&rw, ssB);
    }
    auto a = ssA.str();
    auto b = ssB.str();
    EXPECT_EQ(a, b);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}