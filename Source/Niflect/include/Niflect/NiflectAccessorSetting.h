#pragma once
#include "Niflect/Util/ConcatSymbols.h"

//General Accessor ����, ��� A ������
//�ɱ���Ϊ������ָ�� Accessor ��������
//1. ����ģ��(Container Template 1D), �� std::vector, std::map
//2. �ṹģ��(Structural Template nD, n>=2), �� std::pair
//3. �ػ�ģ��, �� std::vector<bool>
//4. �����ܱ������������󶨵�����. ���, A ������Ҳ������Ϊһ�ָ�������
//��������ģ�鷶Χ�ڿɶ��ʹ��, ��ָ����ͬ�����͵� Accessor �򸲸���������ָ���� Accessor
//TSetting ģ����ָ����ģ���������Ϊ2��3
#define NIF_AS_A() using CONCAT_SYMBOLS_3(__NiflectGen_AccessorSetting_A,_,__LINE__) = 

//Compound Accessor ����, ��� B ������
//class �� struct ���������, �Ҵ���� NIF_T, �ɰ󶨴�����ָ���� Accessor
//��δָ�����͵� A ������ʱ, �����Ͱ󶨴�����ָ���� Accessor
//��������ģ�鷶Χ��ֻ��ʹ��1��
//TSetting ģ����ָ����ģ���������Ϊ1
#define NIF_AS_B() using CONCAT_SYMBOLS_3(__NiflectGen_AccessorSetting_B,_,__LINE__) = 

//Enum Class Accessor ����, ��� C ������
//�� enum class (�� Scoped enum) �����ö������, �ɰ󶨴�����ָ���� Accessor
//��δָ�����͵� A ������ʱ, �����Ͱ󶨴�����ָ���� Accessor
//��������ģ�鷶Χ��ֻ��ʹ��1��
//TSetting ģ����ָ����ģ���������Ϊ1
#define NIF_AS_C() using CONCAT_SYMBOLS_3(__NiflectGen_AccessorSetting_C,_,__LINE__) = 

//Enum Bits Mask Accessor ����, ��� D ������
//�� enum �����ö������, �ɰ󶨴�����ָ���� Accessor
//��δָ�����͵� A ������ʱ, �����Ͱ󶨴�����ָ���� Accessor
//��������ģ�鷶Χ��ֻ��ʹ��1��
//TSetting ģ����ָ����ģ���������Ϊ1
#define NIF_AS_D() using CONCAT_SYMBOLS_3(__NiflectGen_AccessorSetting_D,_,__LINE__) = 

//Pointer Template Accessor ����, ��� E ������
//�ɱ���Ϊ������ָ�� Accessor ������Ϊָ��ģ���� std::shared_ptr
//��������ģ�鷶Χ�ڿɶ��ʹ��, ��ָ����ͬ�����͵� Accessor �򸲸���������ָ���� Accessor
//TSetting ģ����ָ����ģ���������Ϊ2
#define NIF_AS_E() using CONCAT_SYMBOLS_3(__NiflectGen_AccessorSetting_E,_,__LINE__) = 

namespace NiflectAccessorSetting
{
	using TemplaateArgPlaceholder = void*;
	template <typename TAccessorType, typename TBindingType = TemplaateArgPlaceholder, typename TElementBindingType = TemplaateArgPlaceholder>
	class TSetting
	{
	};
}