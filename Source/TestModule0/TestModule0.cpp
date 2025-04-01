#include "TestModule0/TestModule0.h"
#include "Engine/Test/TestRegistration.h"

namespace TestModule0
{
	void LoadTypes()
	{
		////auto a = Niflect::StaticGetType<float>();
		////auto b = Niflect::StaticGetType<bool>();
		////auto c = Niflect::StaticGetType<TestRegistration::CMyRegClass>();
		////printf("Dependency Type: %s, %s, %s\n", a->GetTypeName().c_str(), b->GetTypeName().c_str(), c->GetTypeName().c_str());
		//auto c = Niflect::StaticGetType<TestRegistration::CMyRegClass>();
		//printf("Dependency Type: %s\n", c->GetTypeName().c_str());
		////auto d = Niflect::StaticGetType<Niflect::TArrayNif<float> >();
		////printf("Dependency Type: %s\n", d->GetTypeName().c_str());
	}
	void UnloadTypes()
	{
	}
}