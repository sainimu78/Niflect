#include "Engine/Test/TestStaticGetType.h"

namespace TestRegistration
{
	void SSSSSSSS()
	{
		auto a = Niflect::StaticGetTypeOld<CMyRegClass>();
		auto b = Niflect::StaticGetTypeOld<float>();
		printf("%s, %s\n", a->GetTypeName().c_str(), b->GetTypeName().c_str());
	}
}