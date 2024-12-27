#include "NiflectGen/Test/TestForProjectSetup.h"
#include <stdio.h>
#include "clang-c/Index.h"

void TestFunctionInDll()
{
    auto Idx = clang_createIndex(/* excludeDeclsFromPCH */
        0,
        /* displayDiagnostics=*/1);
    clang_disposeIndex(Idx);
    printf("nihao, %p\n", Idx);
}