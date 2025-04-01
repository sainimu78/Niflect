#pragma once
#include "Niflect/Default/DefaultAccessorSetting.h"
#include "TestModule1/TestResourceAccessor.h"
#include "TestModule1/TestClass1.h"

namespace EngineTypeBindingSettingScope2
{
	using namespace NiflectAccessorSetting;
	NIF_AS_A() TSetting<TestModule1::TSharedInstanceAccessor<TestModule1::CTestBase1>, TestModule1::CSharedTestBase1>;
}