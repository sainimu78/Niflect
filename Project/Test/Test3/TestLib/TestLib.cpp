#include "TestLib.h"
#include "TestLib_private.h"

void InitTestLib(Niflect::CNiflectTable& table)
{
	Niflect::GeneratedRegisterTypes(&table);
	Niflect::GeneratedInitTypes();
	table.InitTypesLayout();
}