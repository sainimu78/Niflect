#include "gtest/gtest.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "NiflectGen/Generator/Generator.h"
#include "Niflect/Util/StringUtil.h"
#include "Niflect/Util/ConcatHardCodedStrings.h"
#include "Niflect/Util/SystemUtil.h"
#include <thread>

TEST(SaveLoad, BuildTest) {
    using namespace NiflectGen;
	auto log = CreateGenLog();
	auto exeDirPath = NiflectUtil::GetCurrentWorkingDirPath();
	auto rootDirPath = NiflectUtil::ResolvePath("../../../../..");
	auto test1BeingTestedSourceDirPath = NiflectUtil::ConcatPath(rootDirPath, "Project/Test/Test1/BeingTested");
	auto niflectSourceDirPath = NiflectUtil::ConcatPath(rootDirPath, "Source/Niflect/include");
	auto generatedDirPath = NiflectUtil::ConcatPath(exeDirPath, "Test1_Generated");
	auto beingTestedBuildDirPath = NiflectUtil::ConcatPath(generatedDirPath, "Build");
	auto niflectGeneratedDirPath = NiflectUtil::ConcatPath(generatedDirPath, "NiflectGenerated");

	CModuleRegInfo info;
	info.m_moduleName = "Test1BeingTestedModule";
	info.m_vecModuleHeader2.push_back(NiflectUtil::ConcatPath(test1BeingTestedSourceDirPath, "Test1BeingTestedHeader.h"));
	info.m_vecAccessorSettingHeader.push_back(NiflectUtil::ConcatPath(test1BeingTestedSourceDirPath, "Test1BeingTestedAccessorSetting.h"));
	info.m_vecModuleHeaderSearchPath2.push_back(NiflectUtil::ConcatPath(test1BeingTestedSourceDirPath, "/"));
	info.m_toolHeaderSearchPath = NiflectUtil::ConcatPath(niflectSourceDirPath, "/");
	info.m_genOutputDirPath = niflectGeneratedDirPath;
	info.m_toGenCreateModuleInfoFunction = true;
	auto gen = CreateGenerator();
	if (gen->Init(info, log.Get()))
	{
		CCodeGenData genData;
		gen->Generate(genData);
		gen->Save2(genData);
		gen->Cleanup();
	}
	while (!NiflectUtil::FileExists(NiflectUtil::ConcatPath(niflectGeneratedDirPath, "FinishedFlag.txt")))
		std::this_thread::sleep_for(std::chrono::seconds(1));

	NiflectUtil::DeleteDirectory(beingTestedBuildDirPath);
#ifdef WIN32
	NiflectUtil::MakeDirectories(NiflectUtil::ConcatPath(beingTestedBuildDirPath, "/"));
	NiflectUtil::SetCurrentWorkingDirPath(beingTestedBuildDirPath);
	NiflectUtil::CmdExec(NiflectUtil::FormatString("cmake %s", test1BeingTestedSourceDirPath.c_str()));
	while (!NiflectUtil::FileExists(NiflectUtil::ConcatPath(beingTestedBuildDirPath, "BeingTestedExe.sln")))
		std::this_thread::sleep_for(std::chrono::seconds(1));
	NiflectUtil::CmdExec("cmake --build .");
	NiflectUtil::SetCurrentWorkingDirPath(exeDirPath);

	Niflect::CString testedResult;
	auto generateedExePath = NiflectUtil::ConcatPath(beingTestedBuildDirPath, "Debug/bin/BeingTestedExe");
	auto winPath = generateedExePath + ".exe";
	while (!NiflectUtil::FileExists(winPath))
		std::this_thread::sleep_for(std::chrono::seconds(1));
	std::replace(winPath.begin(), winPath.end(), '/', '\\');
	NiflectUtil::CmdExec(winPath, testedResult);
#else
	auto beingTestedBuildDirPathDebug = NiflectUtil::ConcatPath(beingTestedBuildDirPath, "Debug");
	NiflectUtil::MakeDirectories(NiflectUtil::ConcatPath(beingTestedBuildDirPathDebug, "/"));
	NiflectUtil::SetCurrentWorkingDirPath(beingTestedBuildDirPathDebug);
	NiflectUtil::CmdExec(NiflectUtil::FormatString("cmake %s -G \"Unix Makefiles\" -DCMAKE_BUILD_TYPE=Debug", test1BeingTestedSourceDirPath.c_str()));
	while (!NiflectUtil::FileExists(NiflectUtil::ConcatPath(beingTestedBuildDirPathDebug, "Makefile")))
		std::this_thread::sleep_for(std::chrono::seconds(1));
	NiflectUtil::SetCurrentWorkingDirPath(beingTestedBuildDirPath);
	NiflectUtil::CmdExec("cmake --build ./Debug");
	auto beingTestedBuildDirPathDebugBin = NiflectUtil::ConcatPath(beingTestedBuildDirPathDebug, "bin");
	NiflectUtil::SetCurrentWorkingDirPath(beingTestedBuildDirPathDebugBin);

	Niflect::CString testedResult;
	auto generateedExePath = NiflectUtil::ConcatPath(beingTestedBuildDirPathDebugBin, "BeingTestedExe");
	while (!NiflectUtil::FileExists(generateedExePath))
		std::this_thread::sleep_for(std::chrono::seconds(1));
	NiflectUtil::CmdExec(generateedExePath, testedResult);

	while (!NiflectUtil::FileExists(generateedExePath))
		std::this_thread::sleep_for(std::chrono::seconds(1));
	NiflectUtil::CmdExec(generateedExePath, testedResult);
#endif
    EXPECT_STREQ(testedResult.c_str(), "Modules count: 1");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}