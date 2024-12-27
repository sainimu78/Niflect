#include "NiflectGen/NiflectGen.h"
#include <stdio.h>
#include <vector>
#include <typeinfo>

void TestFunc()
{
	printf("NiflectGen Test, %lu\n", static_cast<unsigned long>(typeid(std::vector<float>).hash_code()));
}
