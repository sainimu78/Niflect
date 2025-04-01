#pragma once
#include "Asset/EdAsset.h"
#include "Mesh/EdMesh_gen.h"

namespace TestEngineEditMode
{
	class CTestMesh;
}

namespace Editor
{
	class CTestEditorTypeNata : public Niflect::CNata
	{
		typedef Niflect::CNata inherited;
		typedef CTestEditorTypeNata CThis;
	public:
		CTestEditorTypeNata()
			: m_val(0.0f)
			, m_bindingType(NULL)
		{
		}
		CThis& SetExampleValue(float val)
		{
			m_val = val;
			return *this;
		}
		template <typename T>
		CThis& SetBindingType()
		{
			m_bindingType = Niflect::StaticGetType<T>();
			return *this;
		}
		Niflect::CNiflectType* GetBindingType() const
		{
			return m_bindingType;
		}

	public:
		static CTestEditorTypeNata* Cast(inherited* base)
		{
			ASSERT(dynamic_cast<CTestEditorTypeNata*>(base) != NULL);
			return static_cast<CTestEditorTypeNata*>(base);
		}

	public:
		Niflect::CNiflectType* m_bindingType;
		float m_val;
	};

	NIF_T(CTestEditorTypeNata()
		.SetBindingType<TestEngineEditMode::CTestMesh>()
	)
	class CEdMesh : public CEdAsset
	{
		AAAAAAAA()
	public:
		NIF_F()
		Niflect::CString m_debugName;
	};
}