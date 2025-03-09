#pragma once
#include "Niflect/NiflectBase.h"
#include "NiflectGen/NiflectGenCommon.h"

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

	class CLogLocationInfo
	{
	public:
		CLogLocationInfo()
			: m_line(0)
			, m_column(0)
		{

		}
		CLogLocationInfo(const Niflect::CString& filePath, uint32 line, uint32 column)
			: m_filePath(filePath)
			, m_line(line)
			, m_column(column)
		{

		}
		bool IsValid() const
		{
			return !m_filePath.empty();
		}
		Niflect::CString m_filePath;
		uint32 m_line;
		uint32 m_column;
	};

	class CGenLog
	{
	public:
		CGenLog();
		void Config(const CGenLogOption& opt)
		{
			m_opt = opt;
		}

	public:
		void Info(const CLogLocationInfo& indicator, const Niflect::CString& text);
		void Warning(const CLogLocationInfo& indicator, const Niflect::CString& text);
		void Error(const CLogLocationInfo& indicator, const Niflect::CString& text);

	private:
		CLogText& AllocItem(ELogTextType type);
		void WriteItem(const CLogLocationInfo& indicator, ELogTextType type, const Niflect::CString& text);

	public:
		Niflect::TArrayNif<CLogText> m_vecText;
		CGenLogOption m_opt;
	};

	NIFLECTGEN_API Niflect::TSharedPtr<CGenLog> CreateGenLog();

	void GenLogInfo(CGenLog* log, const CLogLocationInfo& indicator, const Niflect::CString& text);
	void GenLogInfo(CGenLog* log, const Niflect::CString& text);
	void GenLogWarning(CGenLog* log, const CLogLocationInfo& indicator, const Niflect::CString& text);
	void GenLogWarning(CGenLog* log, const Niflect::CString& text);
	void GenLogError(CGenLog* log, const CLogLocationInfo& indicator, const Niflect::CString& text);
	NIFLECTGEN_API void GenLogError(CGenLog* log, const Niflect::CString& text);

	void MyLogError(const Niflect::CString& text);
	void MyLogErrorChecked(bool condition, const Niflect::CString& text);
}