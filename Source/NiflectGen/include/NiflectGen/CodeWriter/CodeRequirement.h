#pragma once
#include "Niflect/NiflectBase.h"

namespace NiflectGen
{
	class CNoDupPathCollectorRef
	{
	public:
		bool Cache(const Niflect::CString& path)
		{
			auto ret = m_setAlreadyAdded.insert(path);
			if (ret.second)
				return true;
			return false;
		}
		bool Add(const Niflect::CString& path, Niflect::TArrayNif<Niflect::CString>& vecPath)
		{
			if (this->Cache(path))
			{
				vecPath.push_back(path);
				return true;
			}
			return false;
		}

	private:
		Niflect::TSet<Niflect::CString> m_setAlreadyAdded;
	};
	class CNoDupPathCollector : public CNoDupPathCollectorRef
	{
		typedef CNoDupPathCollectorRef inherited;
	public:
		CNoDupPathCollector()
		{
		}
		bool Add(const Niflect::CString& path)
		{
			return inherited::Add(path, m_vecPath);
		}

	public:
		Niflect::TArrayNif<Niflect::CString> m_vecPath;
	};
}