#include "Editor/Diff/Diff2.h"
#include <iostream>
#include "Niflect/Util/StringUtil.h"
#include <stdexcept>

namespace QtNodeEditor
{
	void CDiff2::DiffCodes(const CListData& vecCodedData, Hashtable3& h, CDiffData& diffData)
	{
		std::vector<int>& Codes = diffData.data;
		int lastUsedCode = (int)h.size();

		Codes.resize(vecCodedData.size());

		for (size_t i = 0; i < vecCodedData.size(); ++i) {
			const auto& s = vecCodedData[i];

			auto itFound = h.find(s);
			if (itFound == h.end()) {
				lastUsedCode++;
				h[s] = lastUsedCode;
				Codes[i] = lastUsedCode;
			}
			else {
				Codes[i] = itFound->second;
			} // if
		} // for

		diffData.Init();
	}
	static void SplitStreamByChar(const char* data, uint32 dataSize, const char delim, CDiff2::CListData& vecCodedData)
	{
		uint32 pos = 0;
		for (uint32 idx = 0; idx < dataSize; ++idx)
		{
			if (data[idx] == delim)
			{
				vecCodedData.push_back(std::string());
				auto& codedData = vecCodedData.back();
				auto size = idx - pos;
				codedData.resize(size);
				memcpy(&codedData[0], data + pos, size);
				pos = idx + 1;
				++idx;
			}
		}
	}
	void CDiff2::ByteStreamToListData(const char* data, uint32 dataSize, const char* delim, uint32 delimSize, CListData& vecCodedData)
	{
		switch (delimSize)
		{
		case 1: SplitStreamByChar(data, dataSize, delim[0], vecCodedData); break;
		default:
			ASSERT(false);//todo
			break;
		}


		//2022.03.12, 考虑到一旦出现不同时在不同的位置之后, 如果仍用等分的数据比较很可能就为不同, 即等分数据进行diff, 在首个不同之后的数据很可能都是不同的, 因此是错误的实现
		//必须通过一定标识进行分段, 比如文本可通过换行, 而二进制数据则无法仅以一个字节作为分段标识, 须要更多字节作为标识
		//uint32 stepSize = dataSize * 0.01;
		//if (stepSize == 0)
		//{
		//	stepSize = dataSize * 0.1;
		//	if (stepSize == 0)
		//	{
		//		stepSize = dataSize * 0.25;
		//		if (stepSize == 0)
		//		{
		//			stepSize = dataSize * 0.5;
		//			if (stepSize == 0)
		//				stepSize = dataSize;
		//		}
		//	}
		//}
		////限制分割大小, 太大可能导致diff记录数据过多即diff数据中相同的数据过多, 太小可能导致比较次数过多, 目前以文本行为参考, 认为一般json文件一行较少超过256, 因此以256分割, 优点是不需要解析源数据
		////可考虑实现简单的分割方法, 比如文本格式按换行符分割, 二进制格式按指定特殊标识分割
		//stepSize = std::min(stepSize, 256u);

		//uint32 codedSize = 0;
		//while (codedSize < dataSize)
		//{
		//	auto newCodedSize = codedSize + stepSize;
		//	if (newCodedSize > dataSize)
		//		newCodedSize = dataSize;
		//	vecCodedData.push_back(std::string());
		//	auto& codedData = vecCodedData.back();
		//	codedData.resize(newCodedSize - codedSize);
		//	memcpy(&codedData[0], data + codedSize, codedData.size());
		//	codedSize = newCodedSize;
		//}
	}
	void CDiff2::Diff(const CListData& vecCodedDataA, const CListData& vecCodedDataB, std::vector<Item>& vecDiff)
	{
		// prepare the input-text and convert to comparable numbers.
		Hashtable3 h;//Hashtable h = new Hashtable(TextA.length() + TextB.length());

		// The A-Version of the data (original data) to be compared.
		CDiffData DataA;
		DiffCodes(vecCodedDataA, h, DataA);

		// The B-Version of the data (modified data) to be compared.
		CDiffData DataB;
		DiffCodes(vecCodedDataB, h, DataB);

		//h = null; // free up hashtable memory (maybe)

		int MAX = DataA.Length + DataB.Length + 1;
		/// vector for the (0,0) to (x,y) search
		std::vector<int> DownVector;
		DownVector.resize(2 * MAX + 2);
		/// vector for the (u,v) to (N,M) search
		std::vector<int> UpVector;
		UpVector.resize(2 * MAX + 2);

		LCS(DataA, 0, DataA.Length, DataB, 0, DataB.Length, DownVector, UpVector);

		Optimize(DataA);
		Optimize(DataB);
		CreateDiffs(DataA, DataB, vecDiff);
	}
	void CDiff2::GetMergeInfo(const std::vector<Item>& vecDiff, const CListData& vecCodedDataB, std::vector<SMergeInfo>& vecMergeInfo)
	{
		int idxA = 0;
		for (auto& itDiff : vecDiff)
		{
			SMergeInfo info;
			info.startA = itDiff.StartA;
			info.deletedA = itDiff.deletedA;
			if (itDiff.insertedB > 0)
			{
				for (int idxB = itDiff.StartB; idxB < itDiff.StartB + itDiff.insertedB; ++idxB)
					info.vecInsert.push_back(vecCodedDataB[idxB]);
			}
			vecMergeInfo.push_back(info);
		}
	}
	void CDiff2::Merge(const std::vector<SMergeInfo>& vecMergeInfo, const CListData& vecCodedDataA, CListData& vecCodedDataC)
	{
		int idxA = 0;
		for (auto& itMerge : vecMergeInfo)
		{
			for (int idx = idxA; idx < itMerge.startA; ++idx)
				vecCodedDataC.push_back(vecCodedDataA[idx]);
			if (itMerge.deletedA == 0)
				idxA = itMerge.startA;
			else
				idxA = itMerge.startA + itMerge.deletedA;
			for (auto& it : itMerge.vecInsert)
				vecCodedDataC.push_back(it);
		}

		for (size_t idx = idxA; idx < vecCodedDataA.size(); ++idx)
			vecCodedDataC.push_back(vecCodedDataA[idx]);
	}
	void CDiff2::Optimize(CDiffData& Data)
	{
		int StartPos, EndPos;

		StartPos = 0;
		while (StartPos < Data.Length) {
			while ((StartPos < Data.Length) && (Data.modified[StartPos] == false))
				StartPos++;
			EndPos = StartPos;
			while ((EndPos < Data.Length) && (Data.modified[EndPos] == true))
				EndPos++;

			if ((EndPos < Data.Length) && (Data.data[StartPos] == Data.data[EndPos])) {
				Data.modified[StartPos] = false;
				Data.modified[EndPos] = true;
			}
			else {
				StartPos = EndPos;
			} // if
		} // while
	}
	CDiff2::SMSRD CDiff2::SMS(CDiffData& DataA, int LowerA, int UpperA, CDiffData& DataB, int LowerB, int UpperB, std::vector<int>& DownVector, std::vector<int>& UpVector)
	{
		SMSRD ret;
		int MAX = DataA.Length + DataB.Length + 1;

		int DownK = LowerA - LowerB; // the k-line to start the forward search
		int UpK = UpperA - UpperB; // the k-line to start the reverse search

		int Delta = (UpperA - LowerA) - (UpperB - LowerB);
		bool oddDelta = (Delta & 1) != 0;

		// The vectors in the publication accepts negative indexes. the vectors implemented here are 0-based
		// and are access using a specific offset: UpOffset UpVector and DownOffset for DownVektor
		int DownOffset = MAX - DownK;
		int UpOffset = MAX - UpK;

		int MaxD = ((UpperA - LowerA + UpperB - LowerB) / 2) + 1;

		// Debug.Write(2, "SMS", String.Format("Search the box: A[{0}-{1}] to B[{2}-{3}]", LowerA, UpperA, LowerB, UpperB));

		// init vectors
		DownVector[DownOffset + DownK + 1] = LowerA;
		UpVector[UpOffset + UpK - 1] = UpperA;

		for (int D = 0; D <= MaxD; D++) {

			// Extend the forward path.
			for (int k = DownK - D; k <= DownK + D; k += 2) {
				// Debug.Write(0, "SMS", "extend forward path " + k.ToString());

				// find the only or better starting point
				int x, y;
				if (k == DownK - D) {
					x = DownVector[DownOffset + k + 1]; // down
				}
				else {
					x = DownVector[DownOffset + k - 1] + 1; // a step to the right
					if ((k < DownK + D) && (DownVector[DownOffset + k + 1] >= x))
						x = DownVector[DownOffset + k + 1]; // down
				}
				y = x - k;

				// find the end of the furthest reaching forward D-path in diagonal k.
				while ((x < UpperA) && (y < UpperB) && (DataA.data[x] == DataB.data[y])) {
					x++; y++;
				}
				DownVector[DownOffset + k] = x;

				// overlap ?
				if (oddDelta && (UpK - D < k) && (k < UpK + D)) {
					if (UpVector[UpOffset + k] <= DownVector[DownOffset + k]) {
						ret.x = DownVector[DownOffset + k];
						ret.y = DownVector[DownOffset + k] - k;
						// ret.u = UpVector[UpOffset + k];      // 2002.09.20: no need for 2 points 
						// ret.v = UpVector[UpOffset + k] - k;
						return (ret);
					} // if
				} // if

			} // for k

			// Extend the reverse path.
			for (int k = UpK - D; k <= UpK + D; k += 2) {
				// Debug.Write(0, "SMS", "extend reverse path " + k.ToString());

				// find the only or better starting point
				int x, y;
				if (k == UpK + D) {
					x = UpVector[UpOffset + k - 1]; // up
				}
				else {
					x = UpVector[UpOffset + k + 1] - 1; // left
					if ((k > UpK - D) && (UpVector[UpOffset + k - 1] < x))
						x = UpVector[UpOffset + k - 1]; // up
				} // if
				y = x - k;

				while ((x > LowerA) && (y > LowerB) && (DataA.data[x - 1] == DataB.data[y - 1])) {
					x--; y--; // diagonal
				}
				UpVector[UpOffset + k] = x;

				// overlap ?
				if (!oddDelta && (DownK - D <= k) && (k <= DownK + D)) {
					if (UpVector[UpOffset + k] <= DownVector[DownOffset + k]) {
						ret.x = DownVector[DownOffset + k];
						ret.y = DownVector[DownOffset + k] - k;
						// ret.u = UpVector[UpOffset + k];     // 2002.09.20: no need for 2 points 
						// ret.v = UpVector[UpOffset + k] - k;
						return (ret);
					} // if
				} // if

			} // for k

		} // for D

		throw std::runtime_error("the algorithm should never come here.");
	}
	void CDiff2::LCS(CDiffData& DataA, int LowerA, int UpperA, CDiffData& DataB, int LowerB, int UpperB, std::vector<int>& DownVector, std::vector<int>& UpVector)
	{
		// Debug.Write(2, "LCS", String.Format("Analyse the box: A[{0}-{1}] to B[{2}-{3}]", LowerA, UpperA, LowerB, UpperB));

		// Fast walkthrough equal lines at the start
		while (LowerA < UpperA && LowerB < UpperB && DataA.data[LowerA] == DataB.data[LowerB]) {
			LowerA++; LowerB++;
		}

		// Fast walkthrough equal lines at the end
		while (LowerA < UpperA && LowerB < UpperB && DataA.data[UpperA - 1] == DataB.data[UpperB - 1]) {
			--UpperA; --UpperB;
		}

		if (LowerA == UpperA) {
			// mark as inserted lines.
			while (LowerB < UpperB)
				DataB.modified[LowerB++] = true;

		}
		else if (LowerB == UpperB) {
			// mark as deleted lines.
			while (LowerA < UpperA)
				DataA.modified[LowerA++] = true;

		}
		else {
			// Find the middle snakea and length of an optimal path for A and B
			SMSRD smsrd = SMS(DataA, LowerA, UpperA, DataB, LowerB, UpperB, DownVector, UpVector);
			// Debug.Write(2, "MiddleSnakeData", String.Format("{0},{1}", smsrd.x, smsrd.y));

			// The path is from LowerX to (x,y) and (x,y) to UpperX
			LCS(DataA, LowerA, smsrd.x, DataB, LowerB, smsrd.y, DownVector, UpVector);
			LCS(DataA, smsrd.x, UpperA, DataB, smsrd.y, UpperB, DownVector, UpVector);  // 2002.09.20: no need for 2 points 
		}
	}
	void CDiff2::CreateDiffs(const CDiffData& DataA, const CDiffData& DataB, std::vector<Item>& a)
	{

		int StartA, StartB;
		int LineA, LineB;

		LineA = 0;
		LineB = 0;
		while (LineA < DataA.Length || LineB < DataB.Length) {
			if ((LineA < DataA.Length) && (!DataA.modified[LineA])
				&& (LineB < DataB.Length) && (!DataB.modified[LineB])) {
				// equal lines
				LineA++;
				LineB++;

			}
			else {
				// maybe deleted and/or inserted lines
				StartA = LineA;
				StartB = LineB;

				while (LineA < DataA.Length && (LineB >= DataB.Length || DataA.modified[LineA]))
					// while (LineA < DataA.Length && DataA.modified[LineA])
					LineA++;

				while (LineB < DataB.Length && (LineA >= DataA.Length || DataB.modified[LineB]))
					// while (LineB < DataB.Length && DataB.modified[LineB])
					LineB++;

				if ((StartA < LineA) || (StartB < LineB)) {
					// store a new difference-item
					Item aItem;
					aItem.StartA = StartA;
					aItem.StartB = StartB;
					aItem.deletedA = LineA - StartA;
					aItem.insertedB = LineB - StartB;
					a.push_back(aItem);
				} // if
			} // if
		} // while
	}
	////如果要合并则必须复制数据, 实现引用偏移没有意义
	//class CDataRef
	//{
	//public:
	//	CDataRef()
	//		: m_psz(NULL)
	//		, m_offset(0)
	//		, m_count(0)
	//	{

	//	}
	//	CDataRef(const char* psz, uint32 offset, uint32 count)
	//		: m_psz(psz)
	//		, m_offset(offset)
	//		, m_count(count)
	//	{

	//	}
	//	std::string GetCopiedData() const
	//	{
	//		ASSERT(m_psz != NULL);
	//		ASSERT(m_count > 0);
	//		std::string data;
	//		data.resize(m_count);
	//		memcpy(&data[0], m_psz + m_offset, m_count);
	//		return data;
	//	}
	//	bool operator==(const CDataRef& rhs) const
	//	{
	//		ASSERT(m_psz != NULL);
	//		ASSERT(m_count > 0);
	//		ASSERT(rhs.m_psz != NULL);
	//		ASSERT(rhs.m_count > 0);
	//		if (m_psz == rhs.m_psz)
	//		{
	//			if (m_offset == rhs.m_offset)
	//			{
	//				if (m_count == rhs.m_count)
	//					return true;
	//			}
	//		}
	//		return false;
	//	}
	//	bool operator<(const CDataRef& rhs) const
	//	{
	//		ASSERT(m_psz != NULL);
	//		ASSERT(m_count > 0);
	//		ASSERT(rhs.m_psz != NULL);
	//		ASSERT(rhs.m_count > 0);
	//		if (m_count < rhs.m_count)
	//			return true;
	//		return memcmp(m_psz + m_offset + m_count, rhs.m_psz + rhs.m_offset + rhs.m_count, rhs.m_count) < 0;
	//	}

	//public:
	//	const char* m_psz;
	//	const uint32 m_offset;
	//	const uint32 m_count;
	//};

	static std::string FormatStdString(const char* format, ...)
	{
		va_list args;

		// 第一次获取格式化长度
		va_start(args, format);
		const int length = std::vsnprintf(nullptr, 0, format, args);
		va_end(args);

		if (length <= 0) return std::string();  // 处理无效格式或空输出

		// 准备足够大小的缓冲区
		std::string result;
		result.resize(static_cast<size_t>(length));

		// 实际执行格式化
		va_start(args, format);
		std::vsnprintf(&result[0], result.size() + 1, format, args); // +1给终止符留空间
		va_end(args);

		return result;
	}
	static std::string TestHelper(const std::vector<CDiff2::Item>& f) {
		std::vector<std::string> ret;
		for (int n = 0; n < (int)f.size(); n++) {
			ret.push_back(FormatStdString("%d.%d.%d.%d*", f[n].deletedA, f[n].insertedB, f[n].StartA, f[n].StartB));
		}
		std::string result;
		for (auto& it : ret)
			result = result + it;
		return result;
	}
	static std::vector<std::string> SplitStrings(const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0)
	{
		std::vector<std::string> ret;
		// Pre-allocate some space for performance
		ret.reserve(maxSplits ? maxSplits + 1 : 10);    // 10 is guessed capacity for most case

		unsigned int numSplits = 0;

		// Use STL methods 
		size_t start, pos;
		start = 0;
		do
		{
			pos = str.find_first_of(delims, start);
			if (pos == start)
			{
				// Do nothing
				start = pos + 1;
			}
			else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
			{
				// Copy the rest of the string
				ret.push_back(str.substr(start));
				break;
			}
			else
			{
				// Copy up to delimiter
				ret.push_back(str.substr(start, pos - start));
				start = pos + 1;
			}
			// parse up to next real data
			start = str.find_first_not_of(delims, start);
			++numSplits;

		} while (pos != std::string::npos);



		return ret;
	}
	int CDiff2::Test()
	{
		std::string ret;
		CListData a, b;
		std::cout << "Diff Self Test..." << std::endl;

		// test all changes
		a = SplitStrings("a,b,c,d,e,f,g,h,i,j,k,l", ",");
		b = SplitStrings("0,1,2,3,4,5,6,7,8,9", ",");
		{
			std::vector<Item> vecDiff;
			CDiff2::Diff(a, b, vecDiff);
			ret = TestHelper(vecDiff);
		}
		ASSERT(ret == "12.10.0.0*");
		std::cout << "all-changes test passed." << std::endl;

		// test all same
		a = SplitStrings("a,b,c,d,e,f,g,h,i,j,k,l", ",");
		b = a;
		{
			std::vector<Item> vecDiff;
			CDiff2::Diff(a, b, vecDiff);
			ret = TestHelper(vecDiff);
		}
		ASSERT(ret == "");
		std::cout << "all-same test passed." << std::endl;

		// test snake
		a = SplitStrings("a,b,c,d,e,f", ",");
		b = SplitStrings("b,c,d,e,f,x", ",");
		{
			std::vector<Item> vecDiff;
			CDiff2::Diff(a, b, vecDiff);
			ret = TestHelper(vecDiff);
		}
		ASSERT(ret == "1.0.0.0*0.1.6.5*");
		std::cout << "snake test passed." << std::endl;

		// 2002.09.20 - repro
		a = SplitStrings("c1,a,c2,b,c,d,e,g,h,i,j,c3,k,l", ",");
		b = SplitStrings("C1,a,C2,b,c,d,e,I1,e,g,h,i,j,C3,k,I2,l", ",");
		{
			std::vector<Item> vecDiff;
			CDiff2::Diff(a, b, vecDiff);
			ret = TestHelper(vecDiff);
		}
		ASSERT(ret == "1.1.0.0*1.1.2.2*0.2.7.7*1.1.11.13*0.1.13.15*");
		std::cout << "repro20020920 test passed." << std::endl;

		// 2003.02.07 - repro
		a = SplitStrings("F", ",");
		b = SplitStrings("0,F,1,2,3,4,5,6,7", ",");
		{
			std::vector<Item> vecDiff;
			CDiff2::Diff(a, b, vecDiff);
			ret = TestHelper(vecDiff);
		}
		ASSERT(ret == "0.1.0.0*0.7.1.2*");
		std::cout << "repro20030207 test passed." << std::endl;

		// Muegel - repro
		a.clear();
		a.push_back("HELLO");
		a.push_back("WORLD");
		b.clear();
		b.push_back("");
		b.push_back("");
		b.push_back("hello");
		b.push_back("");
		b.push_back("");
		b.push_back("");
		b.push_back("");
		b.push_back("world");
		{
			std::vector<Item> vecDiff;
			CDiff2::Diff(a, b, vecDiff);
			ret = TestHelper(vecDiff);
		}
		ASSERT(ret == "2.8.0.0*");
		std::cout << "repro20030409 test passed." << std::endl;

		// test some differences
		a = SplitStrings("a,b,-,c,d,e,f,f", ",");
		b = SplitStrings("a,b,x,c,e,f", ",");
		{
			std::vector<Item> vecDiff;
			CDiff2::Diff(a, b, vecDiff);
			ret = TestHelper(vecDiff);
		}
		ASSERT(ret == "1.1.2.2*1.0.4.4*1.0.7.6*");
		std::cout << "some-changes test passed." << std::endl;

		// test one change within long chain of repeats
		a = SplitStrings("a,a,a,a,a,a,a,a,a,a", ",");
		b = SplitStrings("a,a,a,a,-,a,a,a,a,a", ",");
		{
			std::vector<Item> vecDiff;
			CDiff2::Diff(a, b, vecDiff);
			ret = TestHelper(vecDiff);
		}
		ASSERT(ret == "0.1.4.4*1.0.9.10*");
		std::cout << "long chain of repeats test passed." << std::endl;

		std::cout << "End." << std::endl;

		{
			CListData lstA;
			CListData lstB;
			//lstA.push_back("a");
			//lstA.push_back("b");
			//lstA.push_back("c");
			//lstA.push_back("d");
			//lstA.push_back("d");
			//lstB.push_back("q");
			//lstB.push_back("c");
			//lstB.push_back("b");
			//lstB.push_back("d");

			lstA = SplitStrings("a,b,c,a,b,b,a", ",");
			lstB = SplitStrings("c,b,a,b,a,c", ",");

			//LoadList("E:/diff/begin_old.txt", lstA);
			//LoadList("E:/diff/end_lstCur.txt", lstB);
			std::vector<Item> vecDiff;
			CDiff2::Diff(lstA, lstB, vecDiff);
			std::vector<SMergeInfo> vecMergeInfo;
			CDiff2::GetMergeInfo(vecDiff, lstB, vecMergeInfo);
			CListData lstMerged;
			CDiff2::Merge(vecMergeInfo, lstA, lstMerged);
			//Merge(lstA, lstB, vecDiff, lstMerged);
			ASSERT(lstMerged == lstB);
		}

		return (0);
	}
}