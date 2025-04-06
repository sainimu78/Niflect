#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/NiflectAccessor.h"
#include "NiflectGen/Test/TestModuleHeaderAccessorFinding.h"

namespace MyAccessor
{
	class CMyClassAccessor : public Niflect::CAccessor
	{
	};

	class CMyFloatAccessor : public Niflect::CAccessor
	{
	};
}

namespace EngineTypeBindingSettingScope
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;
	using namespace TestMyFinding;
	using namespace MyAccessor;
	NIFAS_A() TSetting<CMyClassAccessor, CMyClass_0>;
	NIFAS_A() TSetting<CMyFloatAccessor, float>;
}