#pragma once
#include "Engine/Test/Asset/TestAsset.h"
#include "Engine/Test/Mesh/TestMesh_gen.h"

namespace TestEngineEditMode
{
	NIF_T()
	class CTestMesh : public CTestAsset
	{
		GENERATED_BODY()
	public:
		virtual void DebugPrint() const override;

	private:
		NIF_F()
		Niflect::CString m_debugName;
	};
}