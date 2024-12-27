#include "NiflectGen/Log/Log.h"
#include "Niflect/Util/StringUtil.h"

namespace NiflectGen
{
	using namespace Niflect;
	void CGenLog::Info(const CString& text)
	{
		this->WriteItem(ELogTextType::Info, text);
	}
	void CGenLog::Warning(const CString& text)
	{
		this->WriteItem(ELogTextType::Warning, text);
	}
	void CGenLog::Error(const CString& text)
	{
		this->WriteItem(ELogTextType::Error, text);
	}
	CLogText& CGenLog::AllocItem(ELogTextType type)
	{
		m_vecText.push_back(CLogText(type));
		return m_vecText.back();
	}
	void CGenLog::WriteItem(ELogTextType type, const CString& text)
	{
		const char* pszTextType = NULL;
		switch (type)
		{
		case ELogTextType::Info: pszTextType = "Info"; break;
		case ELogTextType::Warning: pszTextType = "Warning"; break;
		case ELogTextType::Error: pszTextType = "Error"; break;
		default:
			break;
		}

		if (m_opt.m_printingAddingItem)
		{
			printf("[%s] %s\n", pszTextType, text.c_str());
		}
		if (m_opt.m_assertionOnAddingItem)
		{
			ASSERT(false);
		}
		if (m_opt.m_cachedItems)
		{
			this->AllocItem(type).m_text = text;
		}
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
	void GenLogError(CGenLog* log, const Niflect::CString& text)
	{
		if (log != NULL)
			log->Error(text);
	}
}