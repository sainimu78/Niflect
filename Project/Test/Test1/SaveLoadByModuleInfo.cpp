#include "gtest/gtest.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "NiflectGen/Generator/Generator.h"
#include "Niflect/Util/StringUtil.h"
#include "Niflect/Util/ConcatHardCodedStrings.h"
#include "Niflect/Util/SystemUtil.h"
#define ROOT_DIR_PATH "../../../../../.."

TEST(SaveLoad, ExampleJson2) {
    using namespace NiflectGen;
	auto log = CreateGenLog();
	auto rootDirPath = NiflectUtil::ResolvePath(ROOT_DIR_PATH);
	auto test1SourceDirPath = NiflectUtil::ConcatPath(rootDirPath, "/Project/Test/Test1");
	auto niflectSourceDirPath = NiflectUtil::ConcatPath(rootDirPath, "/Source/Niflect/include");
	auto buildDirPath = NiflectUtil::GetCurrentWorkingDirPath();
	CModuleRegInfo info;
	info.m_moduleName = "Test1_Module";
	info.m_vecModuleHeader2.push_back(NiflectUtil::ConcatPath(test1SourceDirPath, "SaveLoadByModuleInfo.h"));
	info.m_vecAccessorSettingHeader.push_back(NiflectUtil::ConcatPath(test1SourceDirPath, "Test1AccessorSetting.h"));
	info.m_vecModuleHeaderSearchPath2.push_back(NiflectUtil::ConcatPath(test1SourceDirPath, "/"));
	info.m_toolHeaderSearchPath = NiflectUtil::ConcatPath(niflectSourceDirPath, "/");
	info.m_genOutputDirPath = NiflectUtil::ConcatPath(buildDirPath, "/Test1_Generated");
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