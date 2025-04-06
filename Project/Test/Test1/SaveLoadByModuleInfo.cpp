#include "gtest/gtest.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "NiflectGen/Generator/Generator.h"
#include "Niflect/Util/StringUtil.h"
#include "Niflect/Util/ConcatHardCodedStrings.h"
#include "Niflect/Util/SystemUtil.h"
#include <thread>

bool WaitForFileCreation(const Niflect::CString& filePath)
{
	printf("Waiting for file creation %s\n", filePath.c_str());
	const uint32 maxTimes = 10;
	uint32 cnt = 0;
	while (!NiflectUtil::FileExists(filePath))
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		cnt++;
		printf("Checking for %u/%u times\n", cnt, maxTimes);
		if (cnt == maxTimes)
			return false;
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return true;
}

TEST(SaveLoad, BuildTest) {
    using namespace NiflectGen;
	auto log = CreateGenLog();
	auto exeDirPath = NiflectUtil::GetCurrentWorkingDirPath();
#ifdef WIN32
	auto rootDirPath = NiflectUtil::ResolvePath("../../../../..");
#else
	auto rootDirPath = NiflectUtil::ResolvePath("../../../../../..");
#endif
	auto test1BeingTestedSourceDirPath = NiflectUtil::ConcatPath(rootDirPath, "Project/Test/Test1/BeingTested");
	auto niflectSourceDirPath = NiflectUtil::ConcatPath(rootDirPath, "Source/Niflect/include");
	auto generatedDirPath = NiflectUtil::ConcatPath(exeDirPath, "Test1_Generated");
	auto beingTestedBuildDirPath = NiflectUtil::ConcatPath(generatedDirPath, "Build");
	auto niflectGeneratedDirPath = NiflectUtil::ConcatPath(generatedDirPath, "NiflectGenerated");

	CModuleRegInfo info;
	info.m_moduleName = "Test1BeingTestedModule";
	info.m_vecModuleHeader2.push_back(NiflectUtil::ConcatPath(test1BeingTestedSourceDirPath, "Test1BeingTested.h"));
	info.m_vecAccessorSettingHeader.push_back(NiflectUtil::ConcatPath(test1BeingTestedSourceDirPath, "Test1BeingTestedAccessorSetting.h"));
	info.m_vecModuleHeaderSearchPath2.push_back(NiflectUtil::ConcatPath(test1BeingTestedSourceDirPath, "/"));
	info.m_toolHeaderSearchPath = NiflectUtil::ConcatPath(niflectSourceDirPath, "/");
	info.m_genOutputDirPath = niflectGeneratedDirPath;
	info.m_toGenStaticallyRegisterToRegistry = true;
	auto gen = CreateGenerator();
	if (gen->Init(info, log.Get()))
	{
		CCodeGenData genData;
		gen->Generate(genData);
		gen->Save2(genData);
		gen->Cleanup();
	}
	EXPECT_TRUE(WaitForFileCreation(NiflectUtil::ConcatPath(niflectGeneratedDirPath, "FinishedFlag.txt")));

	NiflectUtil::DeleteDirectory(beingTestedBuildDirPath);
#ifdef WIN32
	NiflectUtil::MakeDirectories(NiflectUtil::ConcatPath(beingTestedBuildDirPath, "/"));
	NiflectUtil::SetCurrentWorkingDirPath(beingTestedBuildDirPath);
	NiflectUtil::CmdExec(NiflectUtil::FormatString("cmake %s", test1BeingTestedSourceDirPath.c_str()));
	EXPECT_TRUE(WaitForFileCreation(NiflectUtil::ConcatPath(beingTestedBuildDirPath, "BeingTestedExe.sln")));
	NiflectUtil::CmdExec("cmake --build .");
	NiflectUtil::SetCurrentWorkingDirPath(exeDirPath);

	Niflect::CString testedResult;
	auto generateedExePath = NiflectUtil::ConcatPath(beingTestedBuildDirPath, "Debug/bin/BeingTestedExe");
	auto winPath = generateedExePath + ".exe";
	EXPECT_TRUE(WaitForFileCreation(winPath));
	std::replace(winPath.begin(), winPath.end(), '/', '\\');
	NiflectUtil::CmdExec(winPath, testedResult);
#else
	auto beingTestedBuildDirPathDebug = NiflectUtil::ConcatPath(beingTestedBuildDirPath, "Debug");
	NiflectUtil::MakeDirectories(NiflectUtil::ConcatPath(beingTestedBuildDirPathDebug, "/"));
	NiflectUtil::SetCurrentWorkingDirPath(beingTestedBuildDirPathDebug);
	NiflectUtil::CmdExec(NiflectUtil::FormatString("cmake %s -G \"Unix Makefiles\" -DCMAKE_BUILD_TYPE=Debug", test1BeingTestedSourceDirPath.c_str()));
	EXPECT_TRUE(WaitForFileCreation(NiflectUtil::ConcatPath(beingTestedBuildDirPathDebug, "Makefile")));
	NiflectUtil::SetCurrentWorkingDirPath(beingTestedBuildDirPath);
	NiflectUtil::CmdExec("cmake --build ./Debug");
	NiflectUtil::SetCurrentWorkingDirPath(exeDirPath);

	Niflect::CString testedResult;
	auto generateedExePath = NiflectUtil::ConcatPath(beingTestedBuildDirPath, "Debug/bin/BeingTestedExe");
	EXPECT_TRUE(WaitForFileCreation(generateedExePath));
	NiflectUtil::CmdExec(generateedExePath, testedResult);

	EXPECT_TRUE(WaitForFileCreation(generateedExePath));
	NiflectUtil::CmdExec(generateedExePath, testedResult);
#endif
    EXPECT_STREQ(testedResult.c_str(), "Modules count: 1");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}