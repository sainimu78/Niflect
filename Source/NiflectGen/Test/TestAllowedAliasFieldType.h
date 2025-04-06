#pragma once
#include "Niflect/NiflectMacroTag.h"
#include "Niflect/Base/Array.h"

using MyFloat = float;
using MyFloat2 = MyFloat;

template <typename T>
using MyArray = Niflect::TArray<T>;
template <typename T>
using MyArray2 = MyArray<T>;

using MyFloatArray = Niflect::TArray<float>;
using MyFloatArray2 = MyArray<float>;

using MyMyFloatArray = Niflect::TArray<MyFloat>;
using MyMyFloatArray2 = MyArray<MyFloat2>;

NIF_T()
class CSampleObject
{
public:
	CSampleObject()
		: m_field_0(0.0f)
		, m_field_1(0.0f)
		, m_field_2(0.0f)
	{
	}

private:
	NIF_F()
	float m_field_0;
	NIF_F()
	MyFloat m_field_1;
	NIF_F()
	MyFloat2 m_field_2;
	NIF_F()
	MyArray<float> m_field_3;
	NIF_F()
	MyArray2<float> m_field_4;
	NIF_F()
	MyArray<MyFloat> m_field_5;
	NIF_F()
	MyArray2<MyFloat> m_field_6;
	NIF_F()
	MyArray<MyFloat2> m_field_7;
	NIF_F()
	MyArray2<MyFloat2> m_field_8;
	NIF_F()
	MyFloatArray m_field_9;
	NIF_F()
	MyFloatArray2 m_field_10;
	NIF_F()
	MyMyFloatArray m_field_11;
	NIF_F()
	MyMyFloatArray2 m_field_12;
};