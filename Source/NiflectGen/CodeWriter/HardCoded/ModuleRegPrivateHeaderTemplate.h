#pragma once
#include "NiflectGen/CodeWriter/HardCoded/HardCodedTemplate.h"

namespace NiflectGen
{
#define LABEL_2 "InvokationRegisterTypes"
#define LABEL_3 "InvokationInitTypes"
#define LABEL_7 "IncludesTypeRegPrivateHeader"

	namespace HardCodedTemplate
	{
		static const char* ModuleRegPrivateH =
R"(#pragma once
#include "Niflect/NiflectRegistration.h"
)" MAKELABEL(LABEL_7) R"(

static Niflect::TSharedPtr<Niflect::CNiflectRegistration>* s_reg = NULL;

Niflect::CNiflectRegistration* GeneratedNiflectRegistrationGet()
{
	static auto s_holder(Niflect::MakeShared<Niflect::CNiflectRegistration>());
	if (s_reg == NULL)
	{
		s_reg = &s_holder;
		s_holder->InitMiscTable();
	}
	return s_holder.Get();
}
void GeneratedNiflectRegistrationRelease()
{
	*s_reg = NULL;
}
void GeneratedNiflectRegistrationInitialReg()
{
	)" MAKELABEL(LABEL_2) R"(
}
void GeneratedNiflectRegistrationInitTypes()
{
	)" MAKELABEL(LABEL_3) R"(
})"
;
	}
}