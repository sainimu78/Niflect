#pragma once
#include "Niflect/NiflectBase.h"
#include "NiflectGen/CodeWriter/CodeWriter.h"
#include "NiflectGen/CodeWriter/CodeRequirement.h"

namespace NiflectGen
{
	class CWritingHeaderSearchPaths
	{
	public:
		Niflect::TArrayNif<Niflect::CString> m_vecForRegularConversion;//header search paths for regular header file path conversion at writing code stage
		Niflect::TArrayNif<Niflect::CString> m_vecForGenTimeConversion;//header search paths for bypass header file path conversion at writing code stage
	};

	class CHeaderFilePathData
	{
	public:
		CHeaderFilePathData()
			: m_isPreservedHeadersOrder(false)
		{
			ASSERT(false);
		}
		CHeaderFilePathData(const Niflect::CString& path)
			: m_path(path)
			, m_isPreservedHeadersOrder(false)
		{
		}
		CHeaderFilePathData(const char* pszPath)
			: m_path(pszPath)
			, m_isPreservedHeadersOrder(false)
		{
		}
		CHeaderFilePathData(const Niflect::CString& path, bool isPreservedHeadersOrder)
			: m_path(path)
			, m_isPreservedHeadersOrder(isPreservedHeadersOrder)
		{
		}
		const Niflect::CString m_path;
		const bool m_isPreservedHeadersOrder;
	};
	using CHeaderFilePathDataArray = Niflect::TArrayNif<CHeaderFilePathData>;

	class CIncludesHelper
	{
	public:
		struct SIncludeDerectiveData
		{
			Niflect::CString m_incPath;
			bool m_isFromSystemHeaderSearchPaths;
		};
		static void Write(const Niflect::TArrayNif<SIncludeDerectiveData>& vecIncData, CCodeLines& lines)
		{
			WriteIncludeDirectives(vecIncData, lines);
		}
		static void WriteIncludeDirectives(const Niflect::TArrayNif<SIncludeDerectiveData>& vecIncData, CCodeLines& lines);
		static Niflect::CString ConvertToIncludePath(const Niflect::CString& filePath, const Niflect::TArrayNif<Niflect::CString>& vecSearchPath)
		{
			Niflect::CString incPath;
			InternalConvertToIncludePath(filePath, vecSearchPath, incPath);
			return incPath;
		}
		static void ConvertFromHeaderFilePaths(const CHeaderFilePathDataArray& vecHeaderData, const CWritingHeaderSearchPaths& paths, CCodeLines& linesInclude)
		{
			Niflect::TArrayNif<Niflect::CString> vecIncPathPreservedOrder;
			Niflect::TArrayNif<Niflect::CString> vecIncPathSystem;
			Niflect::TArrayNif<Niflect::CString> vecIncPathUser;
			CNoDupPathCollectorRef collector;
			for (auto& it : vecHeaderData)
			{
				auto& path = it.m_path;
				auto incPath = path;
				bool isBypassFilePath = InternalConvertToIncludePath(path, paths.m_vecForGenTimeConversion, incPath);
				if (!isBypassFilePath)
					InternalConvertToIncludePath(path, paths.m_vecForRegularConversion, incPath);
				if (collector.Cache(incPath))
				{
					auto p = &vecIncPathUser;
					if (isBypassFilePath)
						p = &vecIncPathSystem;
					else if (it.m_isPreservedHeadersOrder)
						p = &vecIncPathPreservedOrder;
					p->push_back(incPath);
				}
			}
			Niflect::TArrayNif<SIncludeDerectiveData> vecIncData;
			for (auto& it : vecIncPathPreservedOrder)
				vecIncData.push_back({ it, false });
			for (auto& it : vecIncPathSystem)
				vecIncData.push_back({ it, true });
			for (auto& it : vecIncPathUser)
				vecIncData.push_back({ it, false });
			Write(vecIncData, linesInclude);
		}
		static void WriteUsingNamespaces(const CNoDupPathCollector& pathCollector, CCodeLines& lines)
		{
			WriteUsingNamespaceDirectives(pathCollector.m_vecPath, lines);
		}
		static void WriteUsingNamespaceDirectives(const CCodeLines& vecIncludePath, CCodeLines& lines);

	private:
		static bool InternalConvertToIncludePath(const Niflect::CString& filePath, const Niflect::TArrayNif<Niflect::CString>& vecSearchPath, Niflect::CString& incPath)
		{
			bool converted = false;//false 表示未经过转换, filePath 可能为正确的相对路径
			for (auto& it : vecSearchPath)
			{
				ASSERT(!it.empty());
				ASSERT(it.back() == '/');
				auto pos = filePath.find(it);
				if (pos != std::string::npos)
				{
					auto prefixLength = pos + it.length();
					incPath = filePath.substr(prefixLength, filePath.length() - prefixLength);
					converted = true;
					break;
				}
			}
			return converted;
		}
	};

	class CSimpleCppWriter
	{
	public:
		CSimpleCppWriter(Niflect::CString& data)
			: m_data(data)
		{
		}
		void AddHeaderFirstLine()
		{
			this->AddLine("#pragma once");
		}
		void AddInclude(const Niflect::CString& filePath);
		void AddSystemInclude(const Niflect::CString& filePath);

	private:
		void AddLine(const Niflect::CString& text)
		{
			m_data += text;
			m_data += EscapeChar::NewLine;
		}

	private:
		Niflect::CString& m_data;
	};

	Niflect::CString ConvertToInternalFilePath(const Niflect::CString& relativeTypeRegFilePathNoExt);
}