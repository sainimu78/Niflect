#include "Niflect/NiflectBindingSetting.h"
#include "Engine/BuiltinAccessor.h"
#include "Engine/Test/TestMyTransform.h"
#include "NiflectGen/Test/TestSomeTypes.h"

namespace EngineTypeBindingSettingScope
{
	using namespace Niflect;
	using namespace Engine;

	using MyFloat0 = float;
	using MyFloat1 = MyFloat0;
	using MyFloat2 = float;
	//NIF_BS() TBindingSetting<CFloatAccessor, float>;
	//NIF_BS() TBindingSetting<CFloatAccessor, MyFloat0>;//fail
	//NIF_BS() TBindingSetting<CFloatAccessor, MyFloat1>;//fail
	//NIF_BS() TBindingSetting<CFloatAccessor, MyFloat2>;//fail

	using MyString0 = Niflect::CString;
	using MyString1 = MyString0;
	using MyString2 = Niflect::CString;
	
	//NIF_BS() TBindingSetting<CFloatAccessor, Niflect::CString>;
	//NIF_BS() TBindingSetting<CFloatAccessor, MyString0>;//fail
	//NIF_BS() TBindingSetting<CFloatAccessor, MyString1>;//fail
	//NIF_BS() TBindingSetting<CFloatAccessor, MyString2>;//fail

	//������ TArray �� std::vector, TStdPairAlias �� std::pair �ķ���Ϊ���ԭʼģ�嶨��Ĳ��������Ƿ���ͬ
	//����ζ���޷����ֲַ��ػ��ı���ģ��, �� TArray �� TArrayNifAlias
	//����Ҫʵ�ִ�����, �ɿ��Ǳ��� aliasChain ÿ��ʱ���ػ���ʽ, ���漰�ǳ������ĵݹ����

	//template <typename T0, typename T1>
	//NIF_BS() TBindingSetting<CCompoundAccessor, std::pair<T0, T1> >;
	//template <typename T0, typename T1>
	//NIF_BS() TBindingSetting<CCompoundAccessor, TStdPairAlias<T0, T1> >;//fail
	//template <typename T0, typename T1>
	//NIF_BS() TBindingSetting<CCompoundAccessor, TestSomeScope::SMyPair<T0, T1> >;
	//template <typename T0, typename T1>
	//NIF_BS() TBindingSetting<CCompoundAccessor, TestSomeScope::SMyPairAlias<T0, T1> >;//fail

	using MyBit = bool;
	using MyBits0 = std::vector<bool>;
	using MyBits1 = MyBits0;
	using MyBits2 = std::vector<MyBit>;
	using MyArrayFloat0 = std::vector<float>;
	template <typename T>
	using StdVectorAlias0 = std::vector<T, std::allocator<T> >;
	template <typename T>
	using StdAllocatorAlias0 = std::allocator<T>;
	template <typename T>
	using StdVectorAlias1 = std::vector<T, StdAllocatorAlias0<T> >;
	template <typename T, typename TAllocator>
	using StdVectorAlias2 = std::vector<T, TAllocator>;
	template <typename T>
	using StdVectorAlias3 = StdVectorAlias0<T>;
	template <typename T>
	class MyAllocator0
	{
	};
	template <typename T>
	using MyVector2 = std::vector<T, MyAllocator0<T> >;
	template <typename T>
	class TScope
	{
	public:
		class CSubClass
		{
		};
		using SubFloat = float;
		template <typename T2>
		class TSubArray
		{
			template <typename T3, typename TAllocator>
			using TSubStdVectorAlias3 = std::vector<T3, TAllocator>;
		};
	};
	template <typename TInstance, typename T>
	NIF_BS() TBindingSetting<TStlArrayAccessor<TInstance>, std::vector<T> >;
	template <typename TInstance, typename T>
	NIF_BS() TBindingSetting<TStlArrayAccessor<TInstance>, Niflect::TArray<T> >;//ok
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, std::vector<bool> >;//ok
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, MyBits0>;//fail, ע, ��������ӦΪ�� std::vector<T> �ظ�
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, std::vector<MyBit>>;//fail, ע, ��������ӦΪ�� std::vector<T> �ظ�
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, MyBits1>;//fail
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, std::vector<float> >;//ok
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, MyArrayFloat0>;//fail
	//template <typename TInstance, typename T>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, StdVectorAlias0<T> >;//fail
	//template <typename TInstance, typename T>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, StdVectorAlias1<T> >;//fail
	//template <typename TInstance, typename T>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, MyVector2<T> >;//ok
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, TScope<float>::CSubClass>;//ok
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, TScope<float>::SubFloat>;//fail
	//template <typename TInstance, typename T>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, TScope<float>::TSubArray<T> >;//ok
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, std::vector<bool> >;//ok
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, StdVectorAlias1<bool> >;//ok	
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, MyVector2<bool> >;//ok
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, std::vector<bool, std::allocator<bool> > >;//ok
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, StdVectorAlias2<bool, std::allocator<bool> > >;//ok
	//template <typename TInstance>
	//NIF_BS() TBindingSetting<TStlBitsArrayAccessor<TInstance>, TScope<Niflect::TArray<Niflect::CString> >::TSubArray<int32>::TSubStdVectorAlias3<bool, std::allocator<bool> > >;//ok
}