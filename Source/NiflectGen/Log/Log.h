#pragma once
#include "Niflect/NiflectBase.h"

namespace NiflectGen
{
	enum class ELogTextType
	{
		Info,
		Warning,
		Error,
	};

	class CLogText
	{
	public:
		CLogText()
			: m_type(ELogTextType::Info)
		{

		}
		CLogText(ELogTextType type)
			: m_type(type)
		{

		}
		ELogTextType m_type;
		Niflect::CString m_text;
	};

	class CGenLogOption
	{
		using CThis = CGenLogOption;
	public:
		CGenLogOption()
			: m_assertionOnAddingItem(true)
			, m_printingAddingItem(true)
			, m_cachedItems(false)
		{

		}
		CThis& SetAssertionOnAddingItem(bool b)
		{
			m_assertionOnAddingItem = b;
			return *this;
		}
		CThis& SetPrintingAddingItem(bool b)
		{
			m_printingAddingItem = b;
			return *this;
		}
		CThis& SetCachedItems(bool b)
		{
			m_cachedItems = b;
			return *this;
		}
		bool m_assertionOnAddingItem;
		bool m_printingAddingItem;
		bool m_cachedItems;
	};

	class CGenLog
	{
	public:
		void Config(const CGenLogOption& opt)
		{
			m_opt = opt;
		}

	public:
		void Info(const Niflect::CString& text);
		void Warning(const Niflect::CString& text);
		void Error(const Niflect::CString& text);

	private:
		CLogText& AllocItem(ELogTextType type);
		void WriteItem(ELogTextType type, const Niflect::CString& text);

	public:
		Niflect::TArrayNif<CLogText> m_vecText;
		CGenLogOption m_opt;
	};

	void GenLogError(CGenLog* log, const Niflect::CString& text);

	void MyLogError(const Niflect::CString& text);
	void MyLogErrorChecked(bool condition, const Niflect::CString& text);
}