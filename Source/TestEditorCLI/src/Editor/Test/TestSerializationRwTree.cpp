#include "Editor/Test/TestSerializationRwTree.h"
#include "Niflect/Memory/Default/DefaultMemory.h"
#include "Niflect/Memory/Default/DefaultMemoryPoolScope.h"
#include "Niflect/NiflectBase.h"
#include "Niflect/Memory/Stats/NiflectMemoryStats.h"
#include "Niflect/Serialization/RwTree.h"
#include "Niflect/Util/DebugUtil.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "Niflect/Serialization/BinaryFormat.h"
#include <fstream>
#include "Niflect/Util/StringUtil.h"
#include "Niflect/Test/TestDefinition.h"
#include "Editor/Diff/DiffArrays.h"
#include "Editor/Diff/Diff2.h"
#include "Niflect/Util/FileStreamUtil.h"

namespace TestSerializationRwTree
{
	using namespace RwTree;

	void TestMemory()
	{
		using namespace Niflect;
		CDefaultMemoryStatsScope memTestScope;
		auto stats = GetDefaultMemoryStats();
		if (true)//TLS Default Allocator
		{
			CDefaultMemoryPoolScope sdddddd;

			TestMemoryStatsOnThreadsBegin();
			DebugPrintTestTree();
			TestMemoryStatsOnThreadsEnd();
		}
	}
	void TestSerialization()
	{
		using namespace Niflect;
		CDefaultMemoryStatsScope memTestScope;
		auto stats = GetDefaultMemoryStats();
		if (true)
		{
			CRwNode root;
			auto rw = &root;
			SetRwValueAs<bool>(AddRwValue(rw, "a"), true);
			SetRwValueAs<int8>(AddRwValue(rw, "b"), 1);
			SetRwValueAs<int16>(AddRwValue(rw, "c"), 2);
			SetRwValueAs<int32>(AddRwValue(rw, "d"), 3);
			SetRwValueAs<int64>(AddRwValue(rw, "e"), 4);
			SetRwValueAs<uint8>(AddRwValue(rw, "f"), 5);
			SetRwValueAs<uint16>(AddRwValue(rw, "g"), 6);
			SetRwValueAs<uint32>(AddRwValue(rw, "h"), 7);
			SetRwValueAs<uint64>(AddRwValue(rw, "i"), 8);
			SetRwValueAs<float>(AddRwValue(rw, "j"), 9);
			SetRwValueAs<double>(AddRwValue(rw, "k"), 10);
			SetRwValueAs<Niflect::CString>(AddRwValue(rw, "l"), "11 in string");
			DebugPrintRecurs2(&root);
			ASSERT(GetRwValueAs<bool>(FindRwValue(rw, "a")) == true);
			ASSERT(GetRwValueAs<int8>(FindRwValue(rw, "b")) == 1);
			ASSERT(GetRwValueAs<int16>(FindRwValue(rw, "c")) == 2);
			ASSERT(GetRwValueAs<int32>(FindRwValue(rw, "d")) == 3);
			ASSERT(GetRwValueAs<int64>(FindRwValue(rw, "e")) == 4);
			ASSERT(GetRwValueAs<uint8>(FindRwValue(rw, "f")) == 5);
			ASSERT(GetRwValueAs<uint16>(FindRwValue(rw, "g")) == 6);
			ASSERT(GetRwValueAs<uint32>(FindRwValue(rw, "h")) == 7);
			ASSERT(GetRwValueAs<uint64>(FindRwValue(rw, "i")) == 8);
			ASSERT(GetRwValueAs<float>(FindRwValue(rw, "j")) == 9);
			ASSERT(GetRwValueAs<double>(FindRwValue(rw, "k")) == 10);
			ASSERT(GetRwValueAs<Niflect::CString>(FindRwValue(rw, "l")) == "11 in string");
			printf("");
		}
		if (true)//写测试用的 JSON 格式文件
		{
			const auto oldData = NiflectUtil::ReadStringFromFile(TestDefinition::FilePath::InputJson_JsonFormat);
			{
				CRwNode root;
				DebugBuildStructure(&root);
				//std::ofstream ofs(TestDefinition::FilePath::InputJson_JsonFormat, std::ios::binary);
				NiflectUtil::COutputFileStream ofs(TestDefinition::FilePath::InputJson_JsonFormat);
				CJsonFormat::Write(&root, ofs);
			}
			if (!oldData.empty())
			{
				auto newData = NiflectUtil::ReadStringFromFile(TestDefinition::FilePath::InputJson_JsonFormat);
				ASSERT(newData == oldData);
			}
		}
		if (true)//JSON 格式读
		{
			{
				CRwNode root;
				//std::ifstream ifs(TestDefinition::FilePath::InputJson_JsonFormat, std::ios::binary);
				NiflectUtil::CInputFileStream ifs(TestDefinition::FilePath::InputJson_JsonFormat);
				CJsonFormat::Read(&root, ifs);

				//std::ofstream ofs(TestDefinition::FilePath::OutputJson_JsonFormat, std::ios::binary);
				NiflectUtil::COutputFileStream ofs(TestDefinition::FilePath::OutputJson_JsonFormat);
				CJsonFormat::Write(&root, ofs);
			}
			auto a = NiflectUtil::ReadStringFromFile(TestDefinition::FilePath::InputJson_JsonFormat);
			auto b = NiflectUtil::ReadStringFromFile(TestDefinition::FilePath::OutputJson_JsonFormat);
			ASSERT(a == b);
		}
		if (true)//JSON 格式读 rapidjson 所写数据
		{
			{
				CRwNode root;
				NiflectUtil::CInputFileStream ifs(TestDefinition::FilePath::InputJson_AnimGraphEditorData);
				CJsonFormat::Read(&root, ifs);

				NiflectUtil::COutputFileStream ofs(TestDefinition::FilePath::OutputJson_AnimGraphEditorData);
				CJsonFormat::Write(&root, ofs);
			}

			//写的结果存在一些差异
			//1. 无法原样转换小数
			//2. 来自 rapidjson 的数据中, 缩进字符为4个空格, CJsonFormat 为1个 Tab

			{
				//仅为测试将tab换成4个空格
				Niflect::TArrayNif<Niflect::CString> vecLineA;
				Niflect::CString line;
				Niflect::CStringStream ss;
				auto strFromFile = NiflectUtil::ReadStringFromFile(TestDefinition::FilePath::OutputJson_AnimGraphEditorData);
				ss << strFromFile;
				while (std::getline(ss, line))
					vecLineA.push_back(line);
				for (auto& it : vecLineA)
				{
					auto pos = it.find_first_not_of("\t");
					if (pos > 0)
					{
						const auto& cnt = pos;
						for (uint32 idx1 = 0; idx1 < cnt; ++idx1)
							it.erase(it.begin());
						Niflect::CString testIndents;
						for (uint32 idx1 = 0; idx1 < cnt; ++idx1)
							testIndents += "    ";
						it = testIndents + it;
					}
				}
				ss.clear();
				Niflect::TArrayNif<Niflect::CString> vecLineB;
				ss << NiflectUtil::ReadStringFromFile(TestDefinition::FilePath::InputJson_AnimGraphEditorData);
				while (std::getline(ss, line))
				{
					//begin
					//2024.11.21, 测试发现数据中换行有 \r, 因此多一步处理
					if (!line.empty() && line.back() == '\r') {
						line.erase(line.size() - 1);
					}
					//end
					vecLineB.push_back(line);
				}
				ASSERT(vecLineA.size() == vecLineB.size());
				bool same = vecLineA.size() > 0;
				ASSERT(same);
				uint32 diffCnt = 0;
				for (auto idx = 0; idx < vecLineA.size(); ++idx)
				{
					auto& a = vecLineA[idx];
					auto& b = vecLineB[idx];
					if (a != b)
					{
						diffCnt++;
						Niflect::CString strA;
						Niflect::CString strB;
						{
							auto pos = a.find_last_of(' ');
							strA = a.substr(pos + 1, a.length() - pos);
						}
						{
							auto pos = b.find_last_of(' ');
							strB = b.substr(pos + 1, b.length() - pos);
						}
						auto numA = std::stod(strA.c_str());
						auto numB = std::stod(strB.c_str());
						if (numA != numB)
						{
							same = false;
							break;
						}
						else
						{
							ASSERT((strA.length() - strB.length()) == 1);
						}
					}
				}
				ASSERT(diffCnt == 4);
				ASSERT(same);
			}
		}
		if (true)//JSON & 自定义 Binary 格式相互转换
		{
			{
				CRwNode root;
				DebugBuildStructure(&root);
				{
					//std::ofstream ofs(TestDefinition::FilePath::OutputBinary_0, std::ios::binary);
					NiflectUtil::COutputFileStream ofs(TestDefinition::FilePath::OutputBinary_0);
					CBinaryFormat::A_2(&root, ofs);
				}
				{
					//std::ofstream ofs(TestDefinition::FilePath::OutputJson_0, std::ios::binary);
					NiflectUtil::COutputFileStream ofs(TestDefinition::FilePath::OutputJson_0);
					CJsonFormat::Write(&root, ofs);
				}
			}
			{
				CRwNode root;
				//std::ifstream ifs(TestDefinition::FilePath::OutputBinary_0, std::ios::binary);
				NiflectUtil::CInputFileStream ifs(TestDefinition::FilePath::OutputBinary_0);
				CBinaryFormat::B_2(&root, ifs);

				//std::ofstream ofs(TestDefinition::FilePath::OutputJson_1, std::ios::binary);
				NiflectUtil::COutputFileStream ofs(TestDefinition::FilePath::OutputJson_1);
				CJsonFormat::Write(&root, ofs);
			}

			ASSERT(NiflectUtil::ReadStringFromFile(TestDefinition::FilePath::OutputJson_0) == NiflectUtil::ReadStringFromFile(TestDefinition::FilePath::OutputJson_1));
		}
		if (true)
		{
			//TestDiffLCS::SimpleNumberArrays();
			//TestDiffLCS::TestDiff();
			TestDiffLCS::TestLargeData();
			printf("");
		}
		if (true)
		{
			TestDiffEditGraph::TestLargeData();
			printf("");
		}
	}
}