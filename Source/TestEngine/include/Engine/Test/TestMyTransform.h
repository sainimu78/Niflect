#pragma once
#include "Niflect/Serialization/RwTree.h"

namespace TestAccessor2
{
	using namespace RwTree;

	template <typename T>
	class TMyVector
	{
	public:
		//TMyVector()
		//	: m_x(0)
		//	, m_y(0)
		//	, m_z(0)
		//{

		//}
		T m_x;
		T m_y;
		T m_z;
	};

	template <typename T>
	class TMyQuaternion
	{
	public:
		//TMyQuaternion()
		//	: m_x(0)
		//	, m_y(0)
		//	, m_z(0)
		//	, m_w(0)
		//{

		//}
		T m_x;
		T m_y;
		T m_z;
		T m_w;
	};

	template <typename T>
	class TMyTransform
	{
	public:
		TMyVector<T> m_translation;
		TMyQuaternion<T> m_rotation;
		TMyVector<T> m_scale;
	};

	template <typename T>
	static bool operator==(const TMyVector<T>& lhs, const TMyVector<T>& rhs)
	{
		return
			lhs.m_x == rhs.m_x &&
			lhs.m_y == rhs.m_y &&
			lhs.m_z == rhs.m_z
			;
	}
	template <typename T>
	static bool operator==(const TMyQuaternion<T>& lhs, const TMyQuaternion<T>& rhs)
	{
		return
			lhs.m_x == rhs.m_x &&
			lhs.m_y == rhs.m_y &&
			lhs.m_z == rhs.m_z &&
			lhs.m_w == rhs.m_w
			;
	}
	template <typename T>
	static bool operator==(const TMyTransform<T>& lhs, const TMyTransform<T>& rhs)
	{
		return
			lhs.m_translation == rhs.m_translation &&
			lhs.m_rotation == rhs.m_rotation &&
			lhs.m_scale == rhs.m_scale
			;
	}

	typedef TMyTransform<float> CMyTransformFloat;

	template <typename T>
	static void TestInit_TMyTransform(TMyTransform<T>& srcData)
	{
		srcData.m_translation.m_x = 1.0f;
		srcData.m_translation.m_y = 2.0f;
		srcData.m_translation.m_z = 3.0f;
		srcData.m_rotation.m_x = 4.0f;
		srcData.m_rotation.m_y = 5.0f;
		srcData.m_rotation.m_z = 6.0f;
		srcData.m_rotation.m_w = 7.0f;
		srcData.m_scale.m_x = 8.0f;
		srcData.m_scale.m_y = 9.0f;
		srcData.m_scale.m_z = 10.0f;
	}

	template <typename T>
	static Niflect::CString MyVectorToString(const TMyVector<T>& v)
	{
		std::basic_stringstream<Niflect::CString::value_type, Niflect::CString::traits_type, Niflect::CString::allocator_type> ss;
		ss << v.m_x << ',' << v.m_y << ',' << v.m_z;
		return ss.str();
	}
	template <typename T>
	static TMyVector<T> StringToMyVector(const Niflect::CString& str)
	{
		std::basic_stringstream<Niflect::CString::value_type, Niflect::CString::traits_type, Niflect::CString::allocator_type> ss(str);
		TMyVector<T> v;
		char d;
		ss >> v.m_x >> d >> v.m_y >> d >> v.m_z;
		return v;
	}
	template <typename T>
	static Niflect::CString MyQuaternionToString(const TMyQuaternion<T>& v)
	{
		std::basic_stringstream<Niflect::CString::value_type, Niflect::CString::traits_type, Niflect::CString::allocator_type> ss;
		ss << v.m_x << ',' << v.m_y << ',' << v.m_z << ',' << v.m_w;
		return ss.str();
	}
	template <typename T>
	static TMyQuaternion<T> StringToMyQuaternion(const Niflect::CString& str)
	{
		std::basic_stringstream<Niflect::CString::value_type, Niflect::CString::traits_type, Niflect::CString::allocator_type> ss(str);
		TMyQuaternion<T> v;
		char d;
		ss >> v.m_x >> d >> v.m_y >> d >> v.m_z >> d >> v.m_w;
		return v;
	}
	template <typename T>
	static void SaveMyTransformToRwNode(const TMyTransform<T>& instance, CRwNode* rw)
	{
		//序列化方式是任意的, 因此可认为支持自定义编码
		AddRwString(rw, "m_translation", MyVectorToString<T>(instance.m_translation));
		AddRwString(rw, "m_rotation", MyQuaternionToString<T>(instance.m_rotation));
		AddRwString(rw, "m_scale", MyVectorToString<T>(instance.m_scale));
	}
	template <typename T>
	static void LoadMyTransformFromRwNode(const CRwNode* rw, TMyTransform<T>& instance)
	{
		instance.m_translation = StringToMyVector<T>(FindRwString(rw, "m_translation"));
		instance.m_rotation = StringToMyQuaternion<T>(FindRwString(rw, "m_rotation"));
		instance.m_scale = StringToMyVector<T>(FindRwString(rw, "m_scale"));
	}

	template <typename T>
	class TMyTransformAccessor : public Niflect::CAccessor
	{
	public:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
		{
			auto& instance = *static_cast<const TMyTransform<T>*>(base);
			SaveMyTransformToRwNode<T>(instance, rw);
			return true;
		}
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
		{
			auto& instance = *static_cast<TMyTransform<T>*>(base);
			LoadMyTransformFromRwNode<T>(rw, instance);
			return true;
		}
	};
}