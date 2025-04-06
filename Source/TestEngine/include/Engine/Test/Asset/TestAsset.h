#pragma once
#include "Engine/Test/Asset/TestAsset_gen.h"

namespace TestEngineEditMode
{
	NIF_T()
	class CTestAsset
	{
		NIFGNSD()
	public:
		virtual void DebugPrint() const {}
	};
}