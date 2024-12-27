#pragma once

namespace Niflect
{
	#define AddrType void*
	using AddrOffsetType = ptrdiff_t;
	
	class CAddrOffset
	{
	public:
		enum __InternalConst : AddrOffsetType { None };

	public:
		CAddrOffset()
			: m_addrDiff(None)
		{
		}
		CAddrOffset(const AddrOffsetType& offset)
			: m_addrDiff(offset)
		{
		}
		void SetOffset(const AddrOffsetType& offset)
		{
			m_addrDiff = offset;
		}
		const AddrOffsetType& GetOffset() const
		{
			return m_addrDiff;
		}

	public:
		inline const AddrType GetAddr(const AddrType& base) const
		{
			return static_cast<const char*>(base) + m_addrDiff;
		}
		inline AddrType GetAddr(AddrType& base) const
		{
			return static_cast<char*>(base) + m_addrDiff;
		}

	private:
		AddrOffsetType m_addrDiff;
	};

	template <typename U, typename T>
	inline static AddrOffsetType GetMemberVariableOffset(U T::*member)
	{
		return (char*)&((T*)nullptr->*member) - (char*)nullptr;
	}

	template <typename U, typename T>
	inline static AddrOffsetType GetFieldOffset(U T::* member)
	{
		return (char*)&((T*)nullptr->*member) - (char*)nullptr;
	}
}