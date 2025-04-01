#include "Editor/Test/TestEdAsset.h"
#include "TestEditorCLI_private.h"
#include "Niflect/Memory/Default/DefaultMemory.h"
#include "Niflect/Util/ConcatHardCodedStrings.h"
#include "Niflect/Util/StringUtil.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "Niflect/Util/FileStreamUtil.h"
#include "Niflect/Util/SystemUtil.h"
#include "Engine/Test/TestEngineEditMode.h"
#include "Engine/Engine.h"
#include <iostream>//std::getline

#define TEST_ROOT_PATH "../../../../../.."
#define TEST_CONTENT_ROOT_PATH CONCAT_HARDCODED_STRINGS_2(TEST_ROOT_PATH, "/MyContent")
constexpr const char* MyContent = TEST_CONTENT_ROOT_PATH;

static void CreateAsset(const Niflect::CString& filePath)
{
	Editor::CEdMesh mesh;
	mesh.m_debugName = "nihao";
	auto type = mesh.GetType();
	using namespace RwTree;
	CRwNode rw;
	AddRwString(&rw, "TypeName", type->GetTypeName());
	auto rwData = AddRwNode(&rw, "Data");
	type->SaveInstanceToRwNode(&mesh, rwData);
	std::ofstream ofs;
	NiflectUtil::MakeDirectories(filePath);
	if (NiflectUtil::OpenFileStream(ofs, filePath))
	{
		CJsonFormat::Write(&rw, ofs);
	}
	else
	{
		ASSERT(false);
	}
}
static Niflect::TSharedPtr<Editor::CEdAsset> LoadAsset(Niflect::CNiflectTable* table, const Niflect::CString& filePath)
{
	Niflect::TSharedPtr<Editor::CEdAsset> asset;
	using namespace RwTree;
	std::ifstream ifs;
	if (NiflectUtil::OpenFileStream(ifs, filePath))
	{
		CRwNode rw;
		CJsonFormat::Read(&rw, ifs);
		auto typeName = FindRwString(&rw, "TypeName");
		if (auto type = table->FindTypeByTypeName(typeName))
		{
			auto rwData = FindRwNode(&rw, "Data");
			asset = Niflect::NiflectTypeMakeShared<Editor::CEdAsset>(type);
			type->LoadInstanceFromRwNode(asset.Get(), rwData);
		}
	}
	else
	{
		ASSERT(false);
	}
	return asset;
}

static void CookAsset(Niflect::CNiflectTable* table, const Niflect::CString& originalFilePath, const Niflect::CString& cookedFilePath)
{
	auto asset = LoadAsset(table, originalFilePath);
	auto type = asset->GetType();
	if (auto nata = Editor::CTestEditorTypeNata::Cast(type->GetNata()))
	{
		if (nata->m_bindingType != NULL)
		{
			using namespace RwTree;
			CRwNode rw;
			AddRwString(&rw, "TypeName", nata->m_bindingType->GetTypeName());
			auto rwData = AddRwNode(&rw, "Data");
			type->SaveInstanceToRwNode(asset.Get(), rwData);
			std::ofstream ofs;
			NiflectUtil::MakeDirectories(cookedFilePath);
			if (NiflectUtil::OpenFileStream(ofs, cookedFilePath))
			{
				CJsonFormat::Write(&rw, ofs);
			}
			else
			{
				ASSERT(false);
			}
		}
		else
		{
			ASSERT(false);
		}
	}
	else
	{
		ASSERT(false);
	}
}

void TestEdAsset()
{
	auto memTest = Niflect::GetDefaultMemoryStats();
	{
		TestEngineCreate();
	}
	{
		auto tableHolder = Niflect::MakeShared<Niflect::CNiflectTable>();
		auto table = tableHolder.Get();
		Niflect::GeneratedRegisterTypes(table);
		Niflect::GeneratedInitTypes();
		table->InitTypesLayout();

		auto meshFilePath = NiflectUtil::ConcatPath(MyContent, "Mesh.json");
		auto cookedFilePath = NiflectUtil::RemoveFileExt(meshFilePath) + "_cooked.json";
		if (false)
		{
			{
				CreateAsset(meshFilePath);
			}
			{
				auto asset = LoadAsset(table, meshFilePath);
				auto mesh = asset.Cast<Editor::CEdMesh>();
				printf("");
			}
		}
		if (true)
		{
			printf("Type 'q' to quit\n");
			while (true)
			{
				char cmd;
				std::cin >> cmd;
				
				//Niflect::CString input;
				//std::getline(std::cin, input);
				//if ((input.size() == 1) && (input[0] == 'q'))
				//	break;

				bool done = false;
				Niflect::CString valueInString;
				switch (cmd)
				{
				case 'q':
					done = true;
					break;
				case 'r':
					CookAsset(table, meshFilePath, cookedFilePath);
					valueInString = cookedFilePath;
					break;
				default:
					break;
				}
				if (done)
					break;

				Niflect::CString input;
				input = NiflectUtil::FormatString("%c %s", cmd, valueInString.c_str());
				TestEngineEditMode::Run(input);
			}
		}

		tableHolder = NULL;
	}
	{
		TestEngineDestroy();
	}
}