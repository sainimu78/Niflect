#include "NiflectGen/Test/Exp/Exp.h"
//#include <vector>

#define ENABLE_NIHAO
#ifdef ENABLE_NIHAO
#define NIHAO float
#endif

//namespace std
//{
//	template <typename T>
//	class vector;
//}

namespace MyExp0
{
	class CMyClass
	{
	public:
		NIHAO m_myFloat1;
		//std::vector<float> m_vecMyFloatValue;
	};
}

namespace MyStd
{
	template <typename T>
	class TMyArray
	{

	};
}

namespace MyExp1
{
	//template <typename TClass, typename ...TArgs>
	//inline static void GenericInstanceInvokeConstructor(void* obj, TArgs&& ...args)

	template <typename ...TAccessors>
	class TAccessorsTuple
	{
	};

	template <typename TType, typename TTuple>
	class TTypeBinding
	{
	};

	template <template <typename ...> class ContainerT, typename TTuple>
	class TTemplateTypeBinding
	{
	};

	class CMyAccessorBase
	{

	};

	class CMyAccessor0
	{

	};

	class CMyAccessor1 : public CMyAccessorBase
	{

	};

	class CMyAccessor2
	{

	};

	class CMyAccessor3
	{

	};

#define NIFLECTGENINTERNAL_COMBINE_INNER(A,B,C) A##B##C
#define NIFLECTGENINTERNAL_COMBINE(A,B,C) NIFLECTGENINTERNAL_COMBINE_INNER(A,B,C)
#define BIND_END NIFLECTGENINTERNAL_COMBINE(__NIFLECTINTERNALVARNAME,_,__LINE__)

#define BIND_BEGIN typedef 

	BIND_BEGIN TTypeBinding<float, TAccessorsTuple<bool, int> > BIND_END;
	BIND_BEGIN TTypeBinding<bool, TAccessorsTuple<CMyAccessor0, CMyAccessor1, CMyAccessor2, CMyAccessor3> > BIND_END;
	using namespace MyStd;
	BIND_BEGIN TTemplateTypeBinding<TMyArray, TAccessorsTuple<CMyAccessor0, CMyAccessor1> > BIND_END;
}