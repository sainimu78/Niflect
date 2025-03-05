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

//代码中的用语
//1. StaticRegStage, 在静态初始化阶段的注册过程
//2. InitialRegStage, 在逻辑初始化(或App等流程上的初始化)阶段的注册过程
//3. TypeReg, 被标记类型对应的反射生成代码的注册信息, 由一种class所表示, 这种class也称作TypeRegClass
//4. UntaggedReg, Builtin类型, 模板类型的反射生成代码, 只作用于模块内部
//5. ModuleReg, 包含模块所有反射生成代码的注册信息
//6. SplittedModuleReg, 为利用多线程编译, ModuleReg可能被分成多个注册信息, 合并定义注册到原ModuleReg中
//7. Accessor, 定义Field的序列化方式, 或称读写方式
//8. FieldLayout, 由Accessor构成的树型结构, 表示Native对象所有被标记成员变量(Field)的读写方式
//9. AccessorBinding, Field类型对应的Accessor类, 用于生成代码时指定对应的Accessor
//10. NiflectType, 表示运行时的类的信息, 或称作反射信息类, 可通过该信息构造对应的Native对象
//11. Natimeta, 全称为 Native Meta, 通过C++ Native风格定义的Meta信息, 如用于定义类的反射信息, 属性的反射信息
//12. ContainerTemplate1D, 1维容器模板, 代码生成逻辑上的模板参数只有1个, 如std::vector, std::shared_ptr, 对于std::map, 在代码生成逻辑上std::pair作为其模板参数, 而非first与second作为模板参数, 因此std::map在代码生成逻辑上也属于1维容器模板
//13. StructuralTemplateND, N维结构模板, 其中N>=2, 为避免"维"概念歧义, 也可称作多参数结构模板, 每个模板参数对应一个Field, 如std::pair
//14. TAccesorBinding, Accessor与解析类型的绑定配置, 对于模板的绑定, 1维BindingType认定为ContainerTemplate1D, 大于1维的BindingType其最后一个BindingType认定为StructuralTemplateND, 其余认定为ContainerTemplate1D
//15. CursorName, 表示 Full Scope 形式的 Cursor 名称, 其中 Cursor 特指 AccessorBinding 中的 Cursor 与 Field Cursor. 对于全特化模板, 表示名称形如 Niflect::TArray<float>, TestAccessor2::TMyTransform<float>; 对于含模板参数的模板, 表示名称形如 Engine::TStlArrayAccessor, std::pair; 现无计划支持部分特化的形式. 
//	1. CursorName 为自定义概念, 可能易误解为 CursorSpelling, CursorSpelling 不一定为 Full Scope. 特殊的如 Builtin 类型, 无对应有效 Cursor, 名称需要通过 CXType 获取, 而通过 AccessorBinding 生成的 CursorName 则可为 Builtin 类型名称
//16. ResocursorName, 全称为 Resolved Cursor Name, 通过 CursorName 拼接生成特化模板或其它类型的名称, 如 Niflect::TArray<float> 其中包含 Niflect::TArray 与 float 2 个 CursorName, 通过特化模板 Cursor 的解构递归拼接生成 Niflect::TArray<float>

//生成规则
//1. AccessorBinding 设置规则
//	1. AccessorType
//		1. 必须从 Niflect::CAccessor 继承
//		2. 具体定义形式
//			1. 可为 class, 如 CFloatAccessor
//			2. 可为1个参数模板类, 如 TStlArrayAccessor<TInstance>
//			3. 可为模板特化, 如 TMyTransformAccessor<float>
//		3. 对于形式2, 生成代码时, 将 BindingType 套如其参数, 生成创建 Accessor 的代码如 TStlArrayAccessor<TArray<float> >
//	2. BindingType
//		1. 具体定义形式
//			1. 可为 Builtin 类型, 如 float
//			2. 可为定义的类(class), 结构体(struct), 枚举(enum class)
//			3. 可为任意参数个数的类模板或结构体模板
//			4. 可为模板特化, 如 std::vector<bool>
//			5. 可为部分模板特化, 如 Niflect::TArray<T>, 其中 Allocator 为部分特化, 因此不认定与 std::vector 重复指定, 实际上在 Runtime 的 typeid(Niflect::TArray<T>).hash_code() 与 typeid(std::vector<float>).hash_code() 是不同的
//		2. 对于形式2, 要求对应的类型指定标记(NIF_T)
//		3. 对于形式3, 1个参数的模板将认定为容器模板, 多于1个参数的模板将认定为结构模板(StructuralTemplateND)
//			1. 对于结构模板, 要求实际定义的成员变量的 scope 全为 public, 如 std::pair
//		4. 不支持的形式为, 不可指定多个相同的 BindingType, 其中相同是指 Runtime 中 typeid(TypeA).hash_code() == typeid(TypeB).hash_code() 成立, 举例如下
//			1. 不可指定完全相同的 BindingType, 如 float 与 float
//			2. 不可通过别名分别指定原始类型相同的 BindingType, 如 float 与 MyFloat0, MyFloat0 与 MyFloat1
//				1. 其中 MyFloat0 为 using MyFloat0 = float;
//				2. 其中 MyFloat1 为 using MyFloat1 = MyFloat0;
//			3. 不可通过部分模板特化但对应模板实例完全相同的 BindingType, 如 std::vector<T> 与 MyVector0<T>
//				1. 其中 MyVector0 为 template <typename T> using MyVector0 = std::vecotr<T, MyAllocator0<T> >;
//					1. 其中 MyAllocator0 为 template <typename T> using MyAllocator0 = std::allocator<T>;
//			4. 但可定义部分模板特化对应模板实例不完全相同的 BindingType, 如 如 std::vector<T> 与 MyVector1<T>
//				1. 其中 MyVector1 为 template <typename T> using MyVector1 = std::vecotr<T, MyAllocator1<T> >;
//					1. 其中 MyAllocator1 为 template <typename T> class MyAllocator1 { ... };

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