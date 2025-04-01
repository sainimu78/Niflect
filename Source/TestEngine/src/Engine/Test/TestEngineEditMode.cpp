#include "Engine/Test/TestEngineEditMode.h"
#include "Niflect/Util/StringUtil.h"
#include "Engine/Test/Asset/TestAsset.h"
#include "Niflect/Serialization/JsonFormat.h"
#include "Niflect/Util/FileStreamUtil.h"
#include "Niflect/NiflectTable.h"

namespace TestEngineEditMode
{
	static Niflect::TSharedPtr<CTestAsset> LoadAsset(Niflect::CNiflectTable* table, const Niflect::CString& filePath)
	{
		Niflect::TSharedPtr<CTestAsset> asset;
		std::ifstream ifs;
		if (NiflectUtil::OpenFileStream(ifs, filePath))
		{
			using namespace RwTree;
			CRwNode rw;
			CJsonFormat::Read(&rw, ifs);
			auto typeName = FindRwString(&rw, "TypeName");
			auto type = table->FindTypeByTypeName(typeName);
			auto rwData = FindRwNode(&rw, "Data");
			asset = Niflect::NiflectTypeMakeShared<CTestAsset>(type);
			type->LoadInstanceFromRwNode(asset.Get(), rwData);
		}
		return asset;
	}
}

extern Niflect::TSharedPtr<Niflect::CNiflectTable> g_defaultTable;

namespace TestEngineEditMode
{
	enum EArg { ECmd, EVal };
	void Run(const Niflect::CString& input)
	{
		auto table = g_defaultTable.Get();
		auto vecArg = NiflectUtil::Split(input, ' ');
		ASSERT(vecArg.size() % 2 == 0);
		for (uint32 idx = 0; idx < vecArg.size(); idx += 2)
		{
			auto arg = &vecArg[idx];
			if (arg[ECmd] == "r")
			{
				auto& filePath = arg[EVal];
				auto asset = LoadAsset(table, filePath);
				asset->DebugPrint();
			}
		}
	}
}