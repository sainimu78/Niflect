#include "NiflectGen/Log/Log.h"
#include "Niflect/Util/StringUtil.h"

namespace NiflectGen
{
	using namespace Niflect;
	CGenLog::CGenLog()
	{

	}
	void CGenLog::Info(const CLogLocationInfo& indicator, const CString& text)
	{
		this->WriteItem(indicator, ELogTextType::Info, text);
	}
	void CGenLog::Warning(const CLogLocationInfo& indicator, const CString& text)
	{
		this->WriteItem(indicator, ELogTextType::Warning, text);
	}
	void CGenLog::Error(const CLogLocationInfo& indicator, const CString& text)
	{
		this->WriteItem(indicator, ELogTextType::Error, text);
	}
	CLogText& CGenLog::AllocItem(ELogTextType type)
	{
		m_vecText.push_back(CLogText(type));
		return m_vecText.back();
	}
	void CGenLog::WriteItem(const CLogLocationInfo& indicator, ELogTextType type, const CString& text)
	{
		const char* pszTextType = NULL;
		bool isNormal = false;
		switch (type)
		{
		case ELogTextType::Info: pszTextType = ""; isNormal = true; break;
		case ELogTextType::Warning: pszTextType = "warning: "; break;
		case ELogTextType::Error: pszTextType = "error: "; break;
		default:
			break;
		}

		if (m_opt.m_printingAddingItem)
		{
			Niflect::CString strLoc;
			if (indicator.IsValid())
				strLoc = NiflectUtil::FormatString("%s(%u,%u): ", indicator.m_filePath.c_str(), indicator.m_line, indicator.m_column);
			printf("%s%s%s\n", strLoc.c_str(), pszTextType, text.c_str());
			fflush(stdout);
		}
		if ((!isNormal) && m_opt.m_assertionOnAddingItem)
		{
			ASSERT(false);
		}
		if (m_opt.m_cachedItems)
		{
			this->AllocItem(type).m_text = text;
		}
	}
	Niflect::TSharedPtr<CGenLog> CreateGenLog()
	{
		return Niflect::MakeShared<CGenLog>();
	}

	void MyLogError(const Niflect::CString& text)
	{
		ASSERT(false);
	}
	void MyLogErrorChecked(bool condition, const Niflect::CString& text)
	{
		if (!condition)
			MyLogError(text);
	}
	void GenLogInfo(CGenLog* log, const CLogLocationInfo& indicator, const Niflect::CString& text)
	{
		if (log != NULL)
			log->Info(indicator, text);
	}
	void GenLogInfo(CGenLog* log, const Niflect::CString& text)
	{
		if (log != NULL)
			log->Info(CLogLocationInfo(), text);
	}
	void GenLogWarning(CGenLog* log, const CLogLocationInfo& indicator, const Niflect::CString& text)
	{
		if (log != NULL)
			log->Warning(indicator, text);
	}
	void GenLogWarning(CGenLog* log, const Niflect::CString& text)
	{
		if (log != NULL)
			log->Warning(CLogLocationInfo(), text);
	}
	void GenLogError(CGenLog* log, const CLogLocationInfo& indicator, const Niflect::CString& text)
	{
		if (log != NULL)
			log->Error(indicator, text);
	}
	void GenLogError(CGenLog* log, const Niflect::CString& text)
	{
		if (log != NULL)
			log->Error(CLogLocationInfo(), text);
	}
}