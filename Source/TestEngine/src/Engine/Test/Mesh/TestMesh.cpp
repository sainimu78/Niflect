#include "Engine/Test/Mesh/TestMesh.h"

namespace TestEngineEditMode
{
	void CTestMesh::DebugPrint() const
	{
		printf("%s\n", m_debugName.c_str());
	}
}