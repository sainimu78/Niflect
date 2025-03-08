#pragma once

namespace Niflect
{	
	class CAddrOffset
	{
	public:
		using DummyType = void;
		using OffsetType = ptrdiff_t;
		enum __InternalConst : OffsetType { None };

	public:
		CAddrOffset()
			: m_addrDiff(None)
		{
		}
		CAddrOffset(const OffsetType& offset)
			: m_addrDiff(offset)
		{
		}
		void SetOffset(const OffsetType& offset)
		{
			m_addrDiff = offset;
		}
		const OffsetType& GetOffset() const
		{
			return m_addrDiff;
		}

	public:
		inline const DummyType* GetAddr(const DummyType*& base) const
		{
			return static_cast<const char*>(base) + m_addrDiff;
		}
		inline DummyType* GetAddr(DummyType*& base) const
		{
			return static_cast<char*>(base) + m_addrDiff;
		}

	private:
		OffsetType m_addrDiff;
	};

	template <typename U, typename T>
	inline static CAddrOffset::OffsetType GetMemberVariableOffset(U T::*member)
	{
		return (char*)&((T*)nullptr->*member) - (char*)nullptr;
	}

	template <typename U, typename T>
	inline static CAddrOffset::OffsetType GetFieldOffset(U T::* member)
	{
		return (char*)&((T*)nullptr->*member) - (char*)nullptr;
	}
}