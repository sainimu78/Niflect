#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/NiflectAccessor.h"
#include "Engine/Test/TestMyTransform.h"
#include "NiflectGen/NiflectGenDevelopmentMacro.h"

namespace TestGenMyScope
{
	using MyAliasInt8_0 = int8;
	using MyAliasInt8_1 = int8;
	using MyAliasInt8_2 = int8;
	typedef int8 MyAliasInt8_3;
	typedef int8 MyAliasInt8_4;

	namespace SubScope
	{
		using SubMyAliasInt8_0 = int8;
	}

	class CSub_0
	{
	public:
		class CSubSub_0
		{
		};
	};

	class CSub_1
	{
	public:
		class CSubSub_0
		{
		};
	};

	template <typename T>
	class TSub_2
	{
	public:
		class CSubSub_0
		{

		};
	};

	using MyAliasSub_0 = CSub_0;
	using MyAliasSub_1 = Niflect::TArray<float>;
	typedef Niflect::TArray<float> MyAliasSub_2;
}

namespace EngineTypeBindingSettingScope
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;
	using namespace TestAccessor2;

	NIF_AS_A() TSetting<TMyTransformAccessor<float>, TMyTransform<float> >;

	using CMyTM = TestAccessor2::TMyTransform<float>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, CMyTM>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TArray<float> >;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TArray<EngineTypeBindingSettingScope::CMyTM> >;

	using namespace TestGenMyScope;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestGenMyScope::MyAliasInt8_0>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, MyAliasInt8_1>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, Niflect::TMap<Niflect::CString, CMyTM> >;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TMap<float, CMyTM> >;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TMap<bool, EngineTypeBindingSettingScope::CMyTM> >;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TMap<float, bool> >;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TMap<std::string, bool> >;

	using namespace std;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, map<string, int8> >;

	using namespace TestAccessor2;
	NIF_AS_A() TSetting<TMyTransformAccessor<float>, MyAliasInt8_2>;

	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestGenMyScope::SubScope::SubMyAliasInt8_0>;

#ifdef CLASS_TYPE_SCOPE
	//流程缺陷无法区分类Scope与模板定义
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestGenMyScope::CSub_0::CSubSub_0>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, CSub_1::CSubSub_0>;
#else
#endif
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, MyAliasInt8_3>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TestGenMyScope::MyAliasInt8_4>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, MyAliasSub_0>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, MyAliasSub_1>;
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, MyAliasSub_2>;
#ifdef TEMPLATE_INSTANCE_SCOPE
	NIF_AS_A() TSetting<TestAccessor2::TMyTransformAccessor<float>, TSub_2<float>::CSubSub_0>;//不支持模板实例的Scope
#else
#endif
}