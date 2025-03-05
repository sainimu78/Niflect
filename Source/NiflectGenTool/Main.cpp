#include <stdio.h>
#include "NiflectGen/Test/TestParseCpp.h"
#include "NiflectGen/Generator/Generator.h"
#include "Niflect/Util/StringUtil.h"
#include "Niflect/Util/DebugUtil.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"
#include "Niflect/Memory/Default/DefaultMemory.h"
#include "NiflectGen/Test/TestGen.h"
#include "NiflectGen/Generator/GenData.h"
#include "NiflectGen/CodeWriter/CppWriter.h"
#include "Niflect/Util/SystemUtil.h"
#include <cstring>//strcmp
#include <iostream>//std::cin.get()
#include "CommandLineArgParser.h"
#include "CMakeProjectFramework/ProjectVersion.h"
#include "CommandLineArgParser.h"

//�����е�����
//1. StaticRegStage, �ھ�̬��ʼ���׶ε�ע�����
//2. InitialRegStage, ���߼���ʼ��(��App�������ϵĳ�ʼ��)�׶ε�ע�����
//3. TypeReg, ��������Ͷ�Ӧ�ķ������ɴ����ע����Ϣ, ��һ��class����ʾ, ����classҲ����TypeRegClass
//4. UntaggedReg, Builtin����, ģ�����͵ķ������ɴ���, ֻ������ģ���ڲ�
//5. ModuleReg, ����ģ�����з������ɴ����ע����Ϣ
//6. SplittedModuleReg, Ϊ���ö��̱߳���, ModuleReg���ܱ��ֳɶ��ע����Ϣ, �ϲ�����ע�ᵽԭModuleReg��
//7. Accessor, ����Field�����л���ʽ, ��ƶ�д��ʽ
//8. FieldLayout, ��Accessor���ɵ����ͽṹ, ��ʾNative�������б���ǳ�Ա����(Field)�Ķ�д��ʽ
//9. AccessorBinding, Field���Ͷ�Ӧ��Accessor��, �������ɴ���ʱָ����Ӧ��Accessor
//10. NiflectType, ��ʾ����ʱ�������Ϣ, �����������Ϣ��, ��ͨ������Ϣ�����Ӧ��Native����
//11. Natimeta, ȫ��Ϊ Native Meta, ͨ��C++ Native������Meta��Ϣ, �����ڶ�����ķ�����Ϣ, ���Եķ�����Ϣ
//12. ContainerTemplate1D, 1ά����ģ��, ���������߼��ϵ�ģ�����ֻ��1��, ��std::vector, std::shared_ptr, ����std::map, �ڴ��������߼���std::pair��Ϊ��ģ�����, ����first��second��Ϊģ�����, ���std::map�ڴ��������߼���Ҳ����1ά����ģ��
//13. StructuralTemplateND, Nά�ṹģ��, ����N>=2, Ϊ����"ά"��������, Ҳ�ɳ���������ṹģ��, ÿ��ģ�������Ӧһ��Field, ��std::pair
//14. TAccesorBinding, Accessor��������͵İ�����, ����ģ��İ�, 1άBindingType�϶�ΪContainerTemplate1D, ����1ά��BindingType�����һ��BindingType�϶�ΪStructuralTemplateND, �����϶�ΪContainerTemplate1D
//15. CursorName, ��ʾ Full Scope ��ʽ�� Cursor ����, ���� Cursor ��ָ AccessorBinding �е� Cursor �� Field Cursor. ����ȫ�ػ�ģ��, ��ʾ�������� Niflect::TArray<float>, TestAccessor2::TMyTransform<float>; ���ں�ģ�������ģ��, ��ʾ�������� Engine::TStlArrayAccessor, std::pair; ���޼ƻ�֧�ֲ����ػ�����ʽ. 
//	1. CursorName Ϊ�Զ������, ���������Ϊ CursorSpelling, CursorSpelling ��һ��Ϊ Full Scope. ������� Builtin ����, �޶�Ӧ��Ч Cursor, ������Ҫͨ�� CXType ��ȡ, ��ͨ�� AccessorBinding ���ɵ� CursorName ���Ϊ Builtin ��������
//16. ResocursorName, ȫ��Ϊ Resolved Cursor Name, ͨ�� CursorName ƴ�������ػ�ģ����������͵�����, �� Niflect::TArray<float> ���а��� Niflect::TArray �� float 2 �� CursorName, ͨ���ػ�ģ�� Cursor �Ľ⹹�ݹ�ƴ������ Niflect::TArray<float>

//���ɹ���
//1. AccessorBinding ���ù���
//	1. AccessorType
//		1. ����� Niflect::CAccessor �̳�
//		2. ���嶨����ʽ
//			1. ��Ϊ class, �� CFloatAccessor
//			2. ��Ϊ1������ģ����, �� TStlArrayAccessor<TInstance>
//			3. ��Ϊģ���ػ�, �� TMyTransformAccessor<float>
//		3. ������ʽ2, ���ɴ���ʱ, �� BindingType ���������, ���ɴ��� Accessor �Ĵ����� TStlArrayAccessor<TArray<float> >
//	2. BindingType
//		1. ���嶨����ʽ
//			1. ��Ϊ Builtin ����, �� float
//			2. ��Ϊ�������(class), �ṹ��(struct), ö��(enum class)
//			3. ��Ϊ���������������ģ���ṹ��ģ��
//			4. ��Ϊģ���ػ�, �� std::vector<bool>
//			5. ��Ϊ����ģ���ػ�, �� Niflect::TArray<T>, ���� Allocator Ϊ�����ػ�, ��˲��϶��� std::vector �ظ�ָ��, ʵ������ Runtime �� typeid(Niflect::TArray<T>).hash_code() �� typeid(std::vector<float>).hash_code() �ǲ�ͬ��
//		2. ������ʽ2, Ҫ���Ӧ������ָ�����(NIF_T)
//		3. ������ʽ3, 1��������ģ�彫�϶�Ϊ����ģ��, ����1��������ģ�彫�϶�Ϊ�ṹģ��(StructuralTemplateND)
//			1. ���ڽṹģ��, Ҫ��ʵ�ʶ���ĳ�Ա������ scope ȫΪ public, �� std::pair
//		4. ��֧�ֵ���ʽΪ, ����ָ�������ͬ�� BindingType, ������ͬ��ָ Runtime �� typeid(TypeA).hash_code() == typeid(TypeB).hash_code() ����, ��������
//			1. ����ָ����ȫ��ͬ�� BindingType, �� float �� float
//			2. ����ͨ�������ֱ�ָ��ԭʼ������ͬ�� BindingType, �� float �� MyFloat0, MyFloat0 �� MyFloat1
//				1. ���� MyFloat0 Ϊ using MyFloat0 = float;
//				2. ���� MyFloat1 Ϊ using MyFloat1 = MyFloat0;
//			3. ����ͨ������ģ���ػ�����Ӧģ��ʵ����ȫ��ͬ�� BindingType, �� std::vector<T> �� MyVector0<T>
//				1. ���� MyVector0 Ϊ template <typename T> using MyVector0 = std::vecotr<T, MyAllocator0<T> >;
//					1. ���� MyAllocator0 Ϊ template <typename T> using MyAllocator0 = std::allocator<T>;
//			4. ���ɶ��岿��ģ���ػ���Ӧģ��ʵ������ȫ��ͬ�� BindingType, �� �� std::vector<T> �� MyVector1<T>
//				1. ���� MyVector1 Ϊ template <typename T> using MyVector1 = std::vecotr<T, MyAllocator1<T> >;
//					1. ���� MyAllocator1 Ϊ template <typename T> class MyAllocator1 { ... };

namespace CommandLine
{
	static Niflect::CString ParseNextArgValue(CCommandLineArgParser& parser)
	{
		std::string value;
		parser.TryParseNextValue(value);
		return Niflect::CString(value.c_str());
	}
	static Niflect::CString ParseNextArgPath(CCommandLineArgParser& parser)
	{
		std::string path;
		if (parser.TryParseNextValue(path))
			path = NiflectUtil::ResolvePath(path.c_str()).c_str();
		return Niflect::CString(path.c_str());
	}
	static Niflect::CString ParseNextArgIncludeSearchPath(CCommandLineArgParser& parser)
	{
		auto path = ParseNextArgPath(parser);
		if (!path.empty())
			path = NiflectUtil::ConvertToSearchPath(path.c_str()).c_str();
		return Niflect::CString(path.c_str());
	}
}

int main(int argc, const char** argv)
{
	if (false)//if (true)//
	{
		TestGen::TestCollector();
	}

	if (true)//if (false)//
	{
		using namespace NiflectGen;

		using namespace Niflect;
		//auto table = CreateTypeBindingTable();
		auto memTest = GetDefaultMemoryStats();
		{
			auto gen = CreateGenerator();
			if (true)
			{
				CModuleRegInfo info;
				
				//const char* argvTest[] = {
				//	"Placeholder",
				//	"-n", "TestEngine",
				//	"-h", "\"../../../../../../Source/TestEngine/include/Engine/EngineObject.h\"",
				//	"-h", "../../../../../../Source/TestEngine/include/Engine/DerivedObject.h",
				//	"-am", "ENGINE_API",
				//	"-amh", "../../../../../../Source/TestEngine/include/Engine/EngineCommon.h",
				//	"-a", "../../../../../../Source/TestEngine/include/EngineAccessorSetting.h",
				//	"-I", "../../../../../../Source/TestEngine/include",
				//	"-I", "../../../../../../Source/TestModule1/include",
				//	//"-I", "../../../../../../Source/Engine/ResolverCustom",
				//	"-t", "../../../../../../Source/Niflect/include",
				//	"-g", "../../../../../../Generated/NiflectGenerated",
				//	//"-fs",
				//};
				//argc = sizeof(argvTest) / sizeof(const char*);
				//argv = argvTest;

				//const char* argvTest[] = {
				//	"Placeholder",
				//	"-n", "TestModule1",
				//	"-h", "../../../../../../../Wishing/Source/TestModule1/include/TestModule1/TestClass1.h",
				//	"-am", "TESTMODULE1_API",
				//	"-amh", "../../../../../../../Wishing/Source/TestModule1/include/TestModule1Common.h",
				//	"-a", "../../../../../../Source/Niflect/include/Niflect/CommonlyUsed/DefaultAccessorSetting.h",
				//	"-I", "../../../../../../../Wishing/Source/TestModule1/include",
				//	"-t", "../../../../../../Source/Niflect/include",
				//	"-g", "../../../../../../../Wishing/Build/TestEditorCLI/Windows/DefaultBuild/Generated/TestModule1",
				//	//"-fs",
				//};
				//argc = sizeof(argvTest) / sizeof(const char*);
				//argv = argvTest;

				//const char* argvTest[] = {
				//	"Placeholder",
				//	"-n", "Sample0",
				//	"-h", "MyClass.h",
				//	"-a", "../../ThirdParty/Niflect/include/Niflect/CommonlyUsed/DefaultAccessorSetting.h",
				//	"-I", "../../ThirdParty/Niflect/include",
				//	"-I", "./",
				//	"-g", "../../Build/NiflectGenerated",
				//};
				//argc = sizeof(argvTest) / sizeof(const char*);
				//argv = argvTest;

				//const char* argvTest[] = {
				//	"Placeholder",
				//	"-n", "Sample1",
				//	"-h", "MySingleton.h",
				//	"-a", "Sample1AccessorSetting.h",
				//	"-I", "../../ThirdParty/Niflect/include",
				//	"-I", "./",
				//	"-g", "../../Build/NiflectGenerated",
				//};
				//argc = sizeof(argvTest) / sizeof(const char*);
				//argv = argvTest;
				
				//const char* argvTest[] = {
				//	"Placeholder",
				//	"-n", "SampleAPI",
				//	"-h", "../../../../../../../Wishing/Source/Sample/SampleProjectTemplate/SampleAPI/include/SampleAPI.h",
				//	"-h", "../../../../../../../Wishing/Source/Sample/SampleProjectTemplate/SampleAPI/include/SampleAPICommon.h",
				//	"-h", "../../../../../../../Wishing/Source/Sample/SampleProjectTemplate/SampleAPI/include/SampleObject.h",
				//	//"-am", "SAMPLEAPI_API",
				//	//"-amh", "../../../../../../../Wishing/Source/Sample/SampleProjectTemplate/SampleAPI/include/SampleAPICommon.h",
				//	"-gam",
				//	"-a", "../../../../../../../Wishing/ThirdParty/Niflect/Niflect/include/Niflect/CommonlyUsed/DefaultAccessorSetting.h",
				//	"-t", "../../../../../../../Wishing/ThirdParty/Niflect/Niflect/include",
				//	"-I", "../../../../../../../Wishing/Source/Sample/SampleProjectTemplate/SampleAPI/include",
				//	"-I", "../../../../../../../Wishing/Source/Sample/SampleProjectTemplate/SampleAPI/src",
				//	"-I", "../../../../../../../Wishing/Source/Sample/SampleProjectTemplate/SampleAPI/include",
				//	"-I", "../../../../../../../Wishing/ThirdParty/Niflect/Niflect/include",
				//	"-g", "../../../../../../../Wishing/Build/Sample/SampleProjectTemplate/Windows/DefaultBuild/Generated/SampleAPI",
				//	"-gbt",
				//	//"-gsm",
				//	"-aft",
				//};
				//argc = sizeof(argvTest) / sizeof(const char*);
				//argv = argvTest;

				bool waitingForDebuggerAttaching = false;
				using namespace CommandLine;
				CCommandLineArgParser parser(argc, argv);
				parser.RegisterDefaultHelps(
					R"(Example usage on Windows:
@NiflectGenTool ^
-n TestModule1 ^
-h F:/Source/TestModule1/include/TestModule1/TestClass1.h ^
-am TESTMODULE1_API ^
-amh F:/Source/TestModule1/include/TestModule1Common.h ^
-a F:/Source/Niflect/include/Niflect/CommonlyUsed/DefaultAccessorSetting.h ^
-I F:/Source/TestModule1/include ^
-t F:/Source/Niflect/include ^
-g F:/Generated/NiflectGenerated 
)", g_versionMajor, g_versionMinor, g_versionPatch);

				parser.Register("-n", CArgDefinition()
					.SetRequirementType(EArgRequirementType::Required)
					.SetDescription("Module name").SetExample("TestModule1")
					.SetOnFoundArgFunc([&] { info.m_moduleName = ParseNextArgValue(parser); }));
				parser.Register("-h", CArgDefinition()
					.SetDescription("File path of the header to generate reflection code").SetExample("F:/Source/TestModule1/include/TestModule1/TestClass1.h")
					.SetRequirementType(EArgRequirementType::MultipleOptional)
					.SetOnFoundArgFunc([&] { info.m_vecModuleHeader2.push_back(ParseNextArgPath(parser)); }));
				parser.Register("-ph", CArgDefinition()
					.SetDescription("File path of the precompile header using by the module").SetExample("F:/Source/TestModule1/include/TestModule1PrecompileHeader.h")
					.SetRequirementType(EArgRequirementType::MultipleOptional)
					.SetOnFoundArgFunc([&] { info.m_vecModulePrecompileHeader.push_back(ParseNextArgPath(parser)); }));
				parser.Register("-am", CArgDefinition()
					.SetDescription("Module API macro").SetExample("TESTMODULE1_API")
					.SetRequirementType(EArgRequirementType::Optional)
					.SetOnFoundArgFunc([&] { info.m_specifiedModuleApiMacro = ParseNextArgValue(parser); }));
				parser.Register("-amh", CArgDefinition()
					.SetDescription("File path of the header defined the module API macro, there is no need to specified if the file path is included in the precompile header(s)")
					.SetRequirementType(EArgRequirementType::Optional)
					.SetOnFoundArgFunc([&] { info.m_specifiedModuleApiMacroHeaderFilePath = ParseNextArgPath(parser); }));
				parser.Register("-a", CArgDefinition()
					.SetDescription("File path of the accessor setting header using by the module")
					.SetRequirementType(EArgRequirementType::MultipleOptional)
					.SetOnFoundArgFunc([&] { info.m_vecAccessorSettingHeader.push_back(ParseNextArgPath(parser)); }));
				parser.Register("-I", CArgDefinition()
					.SetDescription("Include path containing the specified headers").SetExample("F:/Source/TestModule1/include")
					.SetRequirementType(EArgRequirementType::SpecifiedAtLeastOne)
					.SetOnFoundArgFunc([&] { info.m_vecModuleHeaderSearchPath2.push_back(ParseNextArgIncludeSearchPath(parser)); }));
				parser.Register("-c", CArgDefinition()
					.SetDescription("Resolver customization headers bypassing the correspoinding original ones for the headers parsing(by libclang) time optimization")
					.SetRequirementType(EArgRequirementType::MultipleOptional)
					.SetOnFoundArgFunc([&] { info.m_vecResolverCustomizationHeader.push_back(ParseNextArgPath(parser)); }));
				parser.Register("-t", CArgDefinition()
					.SetDescription("Runtime include path of Niflect").SetExample("F:/Source/Niflect/include")
					.SetRequirementType(EArgRequirementType::Required)
					.SetOnFoundArgFunc([&] { info.m_toolHeaderSearchPath = ParseNextArgIncludeSearchPath(parser); }));
				parser.Register("-g", CArgDefinition()
					.SetDescription("Root output directory path of the generated files").SetExample("F:/Generated/NiflectGenerated ")
					.SetRequirementType(EArgRequirementType::Required)
					.SetOnFoundArgFunc([&] { info.m_genOutputDirPath = ParseNextArgPath(parser); }));
				//else if (strcmp(pszV, "-gs") == 0)
				//{
				//	info.m_genSourceOutputDirPath = GetNextArgPath(argv, idx);
				//}
				parser.Register("-gbt", CArgDefinition()
					.SetDescription("Generate an alias of CThis for the reflected class in GENERATED_BODY expansion")
					.SetRequirementType(EArgRequirementType::Optional)
					.SetNoValue()
					.SetOnFoundArgFunc([&] { info.m_toGenGeneratedBodyThisType = true; }));
				parser.Register("-gsm", CArgDefinition()
					.SetDescription("Generate the statically registering types function with C API")
					.SetRequirementType(EArgRequirementType::Optional)
					.SetNoValue()
					.SetOnFoundArgFunc([&] { info.m_toGenStaticModuleReg = true; }));
				parser.Register("-gsr", CArgDefinition()
					.SetDescription("Generate a static object for statically registering types to the global singleton module registry which can be get by invoking GetModuleRegistry")
					.SetRequirementType(EArgRequirementType::Optional)
					.SetNoValue()
					.SetOnFoundArgFunc([&] { info.m_toGenStaticallyRegisterToRegistry = true; }));
				parser.Register("-fs", CArgDefinition()
					.SetDescription("Generating header(.h) and source(.cpp) files rather than only header(.h) files")
					.SetRequirementType(EArgRequirementType::Optional)
					.SetNoValue()
					.SetOnFoundArgFunc([&] { info.m_genFileMode = EGeneratingHeaderAndSourceFileMode::ESourceAndHeader; }));
#ifdef FIELD_TYPE_CAN_BE_ALIAS_OF_BINDING_TYPE_IN_AS
				parser.Register("-aft", CArgDefinition()
					.SetDescription("Allowed field type as alias for corresponding TSetting's BindingType in Accessor Setting")
					.SetRequirementType(EArgRequirementType::Optional)
					.SetNoValue()
					.SetOnFoundArgFunc([&] { info.m_allowedFieldTypeAsForBindingTypeInAS = true; }));
#else
#endif
				parser.Register("-gam", CArgDefinition()
					.SetDescription("Generating API macro header exports StaticGetType functions of reflected types. Exporting requires defining the control macro (e.g., -D_TESTMODULE1_EXPORTS) in compiler preprocessor options")
					.SetRequirementType(EArgRequirementType::Optional)
					.SetNoValue()
					.SetOnFoundArgFunc([&] { info.m_exportedStaticGetTypeFunctions = true; }));
				parser.Register("-debuggerattaching", CArgDefinition()
					.SetDescription("Waiting for debugger attaching")
					.SetRequirementType(EArgRequirementType::Optional)
					.SetNoValue()
					.SetOnFoundArgFunc([&] { waitingForDebuggerAttaching = true; }));
				if (parser.Parse())
				{
					if (waitingForDebuggerAttaching)
					{
						printf(R"(Debugging Tip:
#1. Set the output directory path for all projects to the bin directory, eg. "Wishing\ThirdParty\NiflectGenTool\NiflectGenTool\build\Windows\x64\Release\bin".
#2. Set breakpoints after the line containing std::cin.get().
#3. Attach the debugger to the NiflectGenTool.exe process.
#4. Press any key on this console.
#5. You can now debug NiflectGenTool.
#6. When you finished debugging, you can restore the projects' settings by Save All in VS and then running Update.bat.
)");
						std::cin.get();
					}

					auto log = CreateGenLog();
					if (gen->Init(info, log.Get()))
					{
						CCodeGenData genData;
						gen->Generate(genData);
						gen->Save2(genData);
						gen->Cleanup();
					}
					else
					{
						ASSERT(false);
					}
				}
			}
		}
	}
	return 0;
}