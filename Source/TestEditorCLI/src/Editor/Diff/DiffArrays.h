#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>//for std::min with vs2017
#include "Niflect/NiflectBase.h"

namespace DiffArrays
{
	enum class EDiffEditAction : char { None, Insert, Delete, Modify };

	template <typename TItem>
	class TDiffItem
	{
	public:
		EDiffEditAction action;
		int index;
		TItem oldValue;
		TItem newValue;
	};

	template <typename TItem>
	static Niflect::TArrayNif<TDiffItem<TItem> > GenerateDiff(const TItem* arr1, int m, const TItem* arr2, int n)
	{
		Niflect::TArrayNif<Niflect::TArrayNif<EDiffEditAction> > actions(m + 1, Niflect::TArrayNif<EDiffEditAction>(n + 1));
		Niflect::TArrayNif<int> curr(n + 1);
		for (int i = 1; i <= m; ++i)
			actions[i][0] = EDiffEditAction::Delete;
		for (int j = 1; j <= n; ++j)
			actions[0][j] = EDiffEditAction::Insert;
		for (int j = 0; j <= n; ++j)
			curr[j] = j;
		for (int i = 1; i <= m; ++i) {
			auto previous = curr[0];
			curr[0] = i;
			for (int j = 1; j <= n; ++j) {
				auto temp = curr[j];
				if (arr1[i - 1] == arr2[j - 1]) {
					curr[j] = previous;
					assert(actions[i][j] == EDiffEditAction::None);
				}
				else {
					const auto& delete_cost = curr[j];
					const auto& insert_cost = curr[j - 1];
					const auto& modify_cost = previous;

					int cost = 1;
					if (delete_cost <= std::min(insert_cost, modify_cost)) {
						cost += delete_cost;
						actions[i][j] = EDiffEditAction::Delete;
					}
					else if (insert_cost <= modify_cost) {
						cost += insert_cost;
						actions[i][j] = EDiffEditAction::Insert;
					}
					else {
						cost += modify_cost;
						actions[i][j] = EDiffEditAction::Modify;
					}
					curr[j] = cost;
				}
				previous = temp;
			}
		}
		const auto& editDistance = curr[n];

		Niflect::TArrayNif<TDiffItem<TItem> > diff;
		diff.reserve(editDistance);
		int i = m, j = n;
		while (i > 0 || j > 0) {
			const auto& action = actions[i][j];
			switch (action)
			{
			case EDiffEditAction::Delete:
				diff.insert(diff.begin(), { action, i - 1, arr1[i - 1], TItem() });
				i--;
				break;
			case EDiffEditAction::Insert:
				diff.insert(diff.begin(), { action, i, TItem(), arr2[j - 1] });
				j--;
				break;
			case EDiffEditAction::Modify:
				diff.insert(diff.begin(), { action, i - 1, arr1[i - 1], arr2[j - 1] });
				i--;
				j--;
				break;
			default:
				i--;
				j--;
				break;
			}
		}
		assert(diff.size() == editDistance);
		return diff;
	}
	template <typename TItem>
	static Niflect::TArrayNif<TDiffItem<TItem> > GenerateDiff(const Niflect::TArrayNif<TItem>& arr1, const Niflect::TArrayNif<TItem>& arr2)
	{
		int m = static_cast<int>(arr1.size());
		int n = static_cast<int>(arr2.size());
		return GenerateDiff<TItem>(arr1.data(), m, arr2.data(), n);
	}
	template <typename TArray, typename TItem>
	static void ApplyDiff(TArray& arr1, const Niflect::TArrayNif<TDiffItem<TItem> >& diff)
	{
		int offset = 0;
		for (const auto& item : diff) {
			switch (item.action) {
			case EDiffEditAction::Insert:
			{
				//arr1.insert(arr1.begin() + item.index + offset, item.newValue);


				//auto i = item.index + offset;
				//if (arr1.size() >= i)
				//    arr1.resize(i + 1);
				//arr1[i] = item.newValue;
				//offset++;  // Increment offset as we have added an element


				auto it = arr1.begin();
				it += item.index + offset;
				arr1.insert(it, item.newValue);
				offset++;  // Increment offset as we have added an element
				break;
			}
			case EDiffEditAction::Delete:
				arr1.erase(arr1.begin() + item.index + offset);
				offset--;  // Decrement offset as we have removed an element
				break;
			case EDiffEditAction::Modify:
				arr1[item.index + offset] = item.newValue;
				break;
			default:
				break;
			}
		}
	}
	static Niflect::TArrayNif<TDiffItem<char> > GenerateDiff(const std::string& arr1, const std::string& arr2)
	{
		int m = static_cast<int>(arr1.size());
		int n = static_cast<int>(arr2.size());
		return GenerateDiff<char>(arr1.data(), m, arr2.data(), n);
	}
}


#include "Niflect/Serialization/RwTree.h"
#include "Niflect/Test/TestDefinition.h"
#include <vector>

namespace TestDiffLCS
{
	using namespace DiffArrays;

	template <typename T>
	static std::string DebugForDiff_ValueToString(const T& val)
	{
		return std::to_string(val);
	}
	template <typename TItem>
	void DebugForDiff_PrintDiff(const Niflect::TArrayNif<TDiffItem<TItem> >& diff) {
		for (const auto& item : diff) {
			switch (item.action) {
			case EDiffEditAction::Insert:
				printf("Insert value %s at index %d\n", DebugForDiff_ValueToString(item.newValue).c_str(), item.index);
				break;
			case EDiffEditAction::Delete:
				printf("Delete value at index %d\n", item.index);
				break;
			case EDiffEditAction::Modify:
				printf("Modify value at index %d from %s to %s\n", item.index, DebugForDiff_ValueToString(item.oldValue).c_str(), DebugForDiff_ValueToString(item.newValue).c_str());
				break;
			default:
				break;
			}
		}
	}

	template <>
	std::string DebugForDiff_ValueToString<double>(const double& val)
	{
		char a[1024];
		sprintf(a, "%.1lf", val);
		return a;
	}
	template <>
	std::string DebugForDiff_ValueToString<char>(const char& val)
	{
		std::string a;
		a = val;
		return a;
	}
}

namespace TestDiffLCS
{
	void SimpleNumberArrays()
	{

		//std::vector<double> arr1 = { 1.0, 2.0, 2.1, 3.0, 4.0 };
		//std::vector<double> arr2 = { 1.0, 2.1, 4.0, 5.0 };

		//std::vector<double> arr1 = { 1.0, 2.0, 3.0, 4.0 };
		//std::vector<double> arr2 = { 1.0, 2.0, 4.0 };

		Niflect::TArrayNif<double> arr1 = { 1.0, 3.0, 4.1, 4.0 };
		Niflect::TArrayNif<double> arr2 = { 1.0, 2.0, 3.0, 4.0 };

		//std::vector<double> arr1 = { 1.0, 2.0, 3.0, 4.0 };
		//std::vector<double> arr2 = { 1.0, 2.1, 3.0, 4.0 };

		//std::string arr1 = "GEEXSFRGEEKKS";
		//std::string arr2 = "GEEKSFORGEEKS";

		std::cout << "Original array 1: " << std::endl;
		for (const auto& val : arr1) {
			std::cout << DebugForDiff_ValueToString(val) << ' ';
		}
		std::cout << std::endl;

		std::cout << "Target array 2: " << std::endl;
		for (const auto& val : arr2) {
			std::cout << DebugForDiff_ValueToString(val) << ' ';
		}
		std::cout << std::endl;

		auto diff = GenerateDiff(arr1, arr2);

		std::cout << "Generated diff:" << std::endl;
		DebugForDiff_PrintDiff(diff);

		ApplyDiff(arr1, diff);

		std::cout << "Array after applying diff: " << std::endl;
		for (const auto& val : arr1) {
			std::cout << DebugForDiff_ValueToString(val) << ' ';
		}
		std::cout << std::endl;
		ASSERT(arr1 == arr2);
	}
}

namespace TestDiffLCS
{
	using namespace RwTree;

	class CRwDiffItem
	{
	public:
		CRwDiffItem()
			: m_node(NULL)
			, m_ownerIdx(INDEX_NONE)
			, m_itemIdx(INDEX_NONE)
		{
		}
		CRwDiffItem(CRwNode* node, uint32 ownerIdx, uint32 itemIdx)
			: m_node(node)
			, m_ownerIdx(ownerIdx)
			, m_itemIdx(itemIdx)
		{
		}
		//bool operator==(const CRwDiffItem& rhs) const
		//{
		//	bool typeOrValueEqual = false;
		//	if (m_node->IsValue())
		//	{
		//		if (rhs.m_node->IsValue())
		//			typeOrValueEqual = m_node->GetData() == rhs.m_node->GetData();
		//	}
		//	else if (auto arrayA = m_node->GetArray())
		//	{
		//		if (auto arrayB = rhs.m_node->GetArray())
		//			typeOrValueEqual = arrayA->GetItemsCount() == arrayB->GetItemsCount();
		//	}
		//	else
		//	{
		//		if (!rhs.m_node->IsValue() && !rhs.m_node->IsArray())
		//			typeOrValueEqual = m_node->GetChildrenCount() == rhs.m_node->GetChildrenCount();
		//	}
		//	if (typeOrValueEqual)
		//		return m_node->GetName() == rhs.m_node->GetName();
		//	return false;
		//}

		//bool operator==(const CRwDiffItem& rhs) const
		//{
		//	//比较相等具有一定的特殊性, 在比较时, 只以节点类型和Value的值作比较, 这是为了使序列Diff产生最优编辑结果(Edit Distance)
		//	//另外如果将Array或Node的子节点用于比较, 则无法产生最优编辑结果, 这是因为Diff算法是针对序列的, 如果引入子层级关系, 则破坏了序列比较
		//	//子节点的比较应由父节点之后的序列进行比较
		//	bool typeOrValueEqual = false;
		//	if (m_node->IsValue())
		//	{
		//		if (rhs.m_node->IsValue())
		//			typeOrValueEqual = m_node->GetData() == rhs.m_node->GetData();
		//	}
		//	else if (m_node->IsArray())
		//	{
		//		typeOrValueEqual = rhs.m_node->IsArray();
		//	}
		//	else
		//	{
		//		typeOrValueEqual = !(rhs.m_node->IsValue() || rhs.m_node->IsArray());
		//	}
		//	if (typeOrValueEqual)
		//		return m_node->GetName() == rhs.m_node->GetName();
		//	return false;
		//}

		bool operator==(const CRwDiffItem& rhs) const
		{
			bool typeOrValueEqual = false;
			if (m_node->IsValue())
			{
				if (rhs.m_node->IsValue())
					typeOrValueEqual = m_node->GetData() == rhs.m_node->GetData();
			}
			else if (m_node->IsArray())
			{
				typeOrValueEqual = rhs.m_node->IsArray();
			}
			else
			{
				typeOrValueEqual = !(rhs.m_node->IsValue() || rhs.m_node->IsArray());
			}
			if (typeOrValueEqual)
			{
				auto& nameA = m_node->GetName();
				auto& nameB = rhs.m_node->GetName();
				if (!nameA.empty())
				{
					return nameA == nameB;
				}
				else if (nameB.empty())
				{
					//为Value时, Data相同, 为Array或Node则以ChildrenCount比较, 因此不论为Value, Array或Node
					return m_node->GetNodesCount() == rhs.m_node->GetNodesCount();
				}
			}
			return false;
		}

		CRwNode* m_node;
		uint32 m_ownerIdx;
		uint32 m_itemIdx;
	};

	struct CRwDiffItemHash {
		std::size_t operator()(const CRwDiffItem& item) const {
			std::size_t hash = 0;
			std::hash<Niflect::CString> hash_fn;
			if (item.m_node->IsValue()) {
				// 如果是 Value 节点, 使用数据值的哈希
				hash ^= hash_fn(item.m_node->GetData()) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			}
			else if (item.m_node->IsArray()) {
				// 如果是 Array 节点, 可以使用某个常量值 + ChildrenCount 进行哈希
				hash ^= hash_fn("Array") + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			}
			else {
				// 如果是其它节点, 使用 ChildrenCount 的哈希
				hash ^= std::hash<size_t>()(item.m_node->GetNodesCount()) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			}

			// 如果有名字，用名字进行哈希
			auto& name = item.m_node->GetName();
			if (!name.empty()) {
				hash ^= hash_fn(name) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			}
			return hash;
		}
	};
	
	static void TestDiffItemHash()
	{
		//只定义测试编译, 原实现未实现基于此map的diff方法
		std::unordered_map<CRwDiffItem, std::string, CRwDiffItemHash> myUnorderedMap;
	}

	static void FlattenNodeRecurs2(CRwNode* rwNode, uint32 ownerIdx, uint32 itemIdx, Niflect::TArrayNif<CRwDiffItem>& vecNode)
	{
		auto indexingId = static_cast<uint32>(vecNode.size());
		vecNode.push_back({ rwNode, ownerIdx, itemIdx });

		for (uint32 idx = 0; idx < rwNode->GetNodesCount(); ++idx)
			FlattenNodeRecurs2(rwNode->GetNode(idx), indexingId, idx, vecNode);
	}

	struct SRwDiffData
	{
		DiffArrays::EDiffEditAction m_editAction;
		uint32 m_indexToEdit;
		std::stringstream m_ss;
		uint32 m_oldOwnerIdx;
		uint32 m_oldItemIdx;
		uint32 m_newOwnerIdx;
		uint32 m_newItemIdx;
	};

	struct SRwAppliedData111111
	{
		CRwNode* m_existingNode;
		CSharedRwNode m_holder;
	};

	//todo: rwNodeA应为const
	static void DiffMy4(CRwNode* rwNodeA, CRwNode* rwNodeB)
	{
		using namespace DiffArrays;
		using namespace Niflect;

		Niflect::TArrayNif<CRwDiffItem> vecNodeA;
		Niflect::TArrayNif<CRwDiffItem> vecNodeB;
		FlattenNodeRecurs2(rwNodeA, INDEX_NONE, INDEX_NONE, vecNodeA);
		FlattenNodeRecurs2(rwNodeB, INDEX_NONE, INDEX_NONE, vecNodeB);
		auto vecInfo = GenerateDiff(vecNodeA, vecNodeB);
		Niflect::TArrayNif<SRwDiffData> vecData;
		for (auto& it : vecInfo)
		{
			vecData.push_back(SRwDiffData());
			auto& item = vecData.back();
			item.m_editAction = it.action;
			item.m_indexToEdit = it.index;
			if (it.newValue.m_node != NULL)
				CBinaryFormat::A_0(it.newValue.m_node, item.m_ss);
			item.m_oldOwnerIdx = it.oldValue.m_ownerIdx;
			item.m_oldItemIdx = it.oldValue.m_itemIdx;
			item.m_newOwnerIdx = it.newValue.m_ownerIdx;
			item.m_newItemIdx = it.newValue.m_itemIdx;
		}
		Niflect::TArrayNif<SRwAppliedData111111> arr1;
		arr1.resize(vecNodeA.size());
		for (uint32 idx = 0; idx < vecNodeA.size(); ++idx)
			arr1[idx].m_existingNode = vecNodeA[idx].m_node;
		int offset = 0;
		Niflect::TArrayNif<uint32> vecOldParentIndexToItemIndexOffset;
		vecOldParentIndexToItemIndexOffset.resize(arr1.size());
		for (const auto& item : vecData)
		{
			switch (item.m_editAction)
			{
			case EDiffEditAction::Delete:
			{
				if (item.m_oldOwnerIdx != INDEX_NONE)
				{
					auto& itemIdxOffset = vecOldParentIndexToItemIndexOffset[item.m_oldOwnerIdx];
					auto& applied = arr1[item.m_oldOwnerIdx];
					applied.m_holder = applied.m_existingNode->GetSharedNode(item.m_oldItemIdx + itemIdxOffset);
					applied.m_existingNode->Delete(item.m_oldItemIdx + itemIdxOffset);
					itemIdxOffset--;
				}
				break;
			}
			default:
				break;
			}
		}
		for (auto& item : vecData)
		{
			switch (item.m_editAction)
			{
			case EDiffEditAction::Insert:
			{
				auto it = arr1.begin();
				it += item.m_indexToEdit + offset;
				auto node = CreateRwNode();
				CBinaryFormat::B_0(node.Get(), item.m_ss);
				arr1.insert(it, { node.Get(), node });
				offset++;  // Increment offset as we have added an element
				break;
			}
			case EDiffEditAction::Delete:
			{
				auto it = arr1.begin();
				it += item.m_indexToEdit + offset;
				arr1.erase(it);
				offset--;  // Decrement offset as we have removed an element
				break;
			}
			case EDiffEditAction::Modify:
			{
				auto it = arr1.begin();
				it += item.m_indexToEdit + offset;
				auto& node = (*it).m_existingNode;
				auto wasValue = node->IsValue();
				auto wasArray = node->IsArray();

				//CBinaryFormat::B_0_Reset(node, item.m_ss);
				node->ResetType();
				CBinaryFormat::B_0(node, item.m_ss);

				ASSERT(node->IsValue() == wasValue || node->IsArray() == wasArray || ((!node->IsValue() && !node->IsArray()) == (!wasValue && !wasArray)));
				break;
			}
			default:
				break;
			}
		}
		Niflect::TArrayNif<uint32> vecNewParentIndexToItemIndexOffset;
		vecNewParentIndexToItemIndexOffset.resize(arr1.size());
		offset = 0;
		for (auto& item : vecData)
		{
			switch (item.m_editAction)
			{
			case EDiffEditAction::Insert:
			{
				if (item.m_newOwnerIdx != INDEX_NONE)
				{
					auto& parent = arr1[item.m_newOwnerIdx];
					auto& itemIdxOffset = vecNewParentIndexToItemIndexOffset[item.m_newOwnerIdx];
					auto& aaaaa = arr1[item.m_indexToEdit + offset];
					ASSERT(aaaaa.m_holder != NULL);
					parent.m_existingNode->InsertNode(aaaaa.m_holder, item.m_newItemIdx);
					itemIdxOffset++;
				}
				offset++;  // Increment offset as we have added an element
				break;
			}
			case EDiffEditAction::Delete:
			{
				offset--;  // Decrement offset as we have removed an element
				break;
			}
			default:
				break;
			}
		}
		{
			std::ofstream ofs(CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/Diff/Output/Merged.json"), std::ios::binary);
			CJsonFormat::Write(arr1[0].m_existingNode, ofs);

			std::stringstream ssA;
			CJsonFormat::Write(rwNodeB, ssA);

			std::stringstream ssB;
			CJsonFormat::Write(arr1[0].m_existingNode, ssB);

			ASSERT(ssA.str() == ssB.str());
		}
		printf("");
	}
	static void InternalTestDiffForCase(const char* fileA, const char* fileB)
	{
		using namespace Niflect;
		CRwNode root;
		CRwNode root2;
		{
			std::ifstream ifs(fileA, std::ios::binary);
			CJsonFormat::Read(&root, ifs);
		}
		{
			std::ifstream ifs(fileB, std::ios::binary);
			CJsonFormat::Read(&root2, ifs);
		}
		{
			DiffMy4(&root, &root2);
		}
	}
	static void TestDiff()
	{
		char filePathA[1024];
		char filePathB[1024];
		for (uint32 idx = 0; idx <= 11; ++idx)
		{
			sprintf(filePathA, CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/Diff/EditCases/%ua.json"), idx);
			sprintf(filePathB, CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/Diff/EditCases/%ub.json"), idx);
			InternalTestDiffForCase(filePathA, filePathB);
		}
	}
	static void TestLargeData()
	{
		using namespace Niflect;

		CRwNode root;
		CRwNode root2;
		{
			std::ifstream ifs(CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/Diff/LargeData/a.json"), std::ios::binary);
			CJsonFormat::Read(&root, ifs);
		}
		{
			std::ifstream ifs(CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/Diff/LargeData/b.json"), std::ios::binary);
			CJsonFormat::Read(&root2, ifs);
		}
		{
			//Niflect::TArrayNif<SMyDiffInfo> vecDiffInfo;
			//DiffMy3(root.Get(), root2.Get(), vecDiffInfo);
			DiffMy4(&root, &root2);
			printf("");
			//计划首层用IndexingMap一对一diff, 即diff数据为每次修改的对象基本信息, 不需要LCS
			//每个对象配一个上一次悠的rwNode拉平缓存, 用于和当前拉平缓存LCS diff
			//方案灵活性在于第2层的diff也可用快速的memcpy或一对一树型比较, 但占用空间多, 也可用复杂度高的LCS, 减少保存数据量
		}
	}
}