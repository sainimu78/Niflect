#include "gtest/gtest.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "NiflectGen/Generator/Generator.h"

TEST(SaveLoad, ExampleJson2) {
    using namespace NiflectGen;
	auto log = CreateGenLog();
	CModuleRegInfo info;
	auto gen = CreateGenerator();
	if (gen->Init(info, log.Get()))
	{
		CCodeGenData genData;
		gen->Generate(genData);
		gen->Save2(genData);
		gen->Cleanup();
	}
    EXPECT_EQ(1, 1);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}