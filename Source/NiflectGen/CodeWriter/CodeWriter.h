#pragma once
#include "Niflect/NiflectBase.h"

namespace NiflectGen
{
	using CCodeLines = Niflect::TArrayNif<Niflect::CString>;
	
	namespace EscapeChar
	{
		constexpr const char* NewLine = "\n";
		constexpr const char* EmptyLine = "";
	}

	class CCodeWriter
	{
	public:
		CCodeWriter()
		{

		}

	public:
		void WriteLine(const Niflect::CString& line)
		{
			m_code += line + EscapeChar::NewLine;
		}
		void WriteLines(const CCodeLines& lines)
		{
			for (auto& it : lines)
				m_code += it + EscapeChar::NewLine;
		}
		void Write(const Niflect::CString& code)
		{
			m_code += code;
		}

	public:
		Niflect::CString m_code;
	};

	static void DebugPrintCodeLines(const CCodeLines& lines)
	{
		CCodeWriter writer;
		writer.WriteLines(lines);
		printf("%s", writer.m_code.c_str());
	}
}