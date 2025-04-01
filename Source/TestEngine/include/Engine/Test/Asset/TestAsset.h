#pragma once
#include "Engine/Test/Asset/TestAsset_gen.h"

namespace TestEngineEditMode
{
	NIF_T()
	class CTestAsset
	{
		GENERATED_BODY()
	public:
		virtual void DebugPrint() const {}
	};
}