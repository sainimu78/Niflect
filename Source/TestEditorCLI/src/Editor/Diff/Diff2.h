#pragma once
#include "Niflect/NiflectCommon.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace QtNodeEditor
{
	class CDiff2
	{
	/// <summary>
	/// This Class implements the Difference Algorithm published in
	/// "An O(ND) Difference Algorithm and its Variations" by Eugene Myers
	/// Algorithmica Vol. 1 No. 2, 1986, p 251.  
	/// 
	/// There are many C, Java, Lisp implementations public available but they all seem to come
	/// from the same source (diffutils) that is under the (unfree) GNU public License
	/// and cannot be reused as a sourcecode for a commercial application.
	/// There are very old C implementations that use other (worse) algorithms.
	/// Microsoft also published sourcecode of a diff-tool (windiff) that uses some tree data.
	/// Also, a direct transfer from a C source to C# is not easy because there is a lot of pointer
	/// arithmetic in the typical C solutions and i need a managed solution.
	/// These are the reasons why I implemented the original published algorithm from the scratch and
	/// make it avaliable without the GNU license limitations.
	/// I do not need a high performance diff tool because it is used only sometimes.
	/// I will do some performace tweaking when needed.
	/// 
	/// The algorithm itself is comparing 2 arrays of numbers so when comparing 2 text documents
	/// each line is converted into a (hash) number. See DiffText(). 
	/// 
	/// Some chages to the original algorithm:
	/// The original algorithm was described using a recursive approach and comparing zero indexed arrays.
	/// Extracting sub-arrays and rejoining them is very performance and memory intensive so the same
	/// (readonly) data arrays are passed arround together with their lower and upper bounds.
	/// This circumstance makes the LCS and SMS functions more complicate.
	/// I added some code to the LCS function to get a fast response on sub-arrays that are identical,
	/// completely deleted or inserted.
	/// 
	/// The result from a comparisation is stored in 2 arrays that flag for modified (deleted or inserted)
	/// lines in the 2 data arrays. These bits are then analysed to produce a array of Item objects.
	/// 
	/// Further possible optimizations:
	/// (first rule: don't do it; second: don't do it yet)
	/// The arrays DataA and DataB are passed as parameters, but are never changed after the creation
	/// so they can be members of the class to avoid the paramter overhead.
	/// In SMS is a lot of boundary arithmetic in the for-D and for-k loops that can be done by increment
	/// and decrement of local variables.
	/// The DownVector and UpVector arrays are alywas created and destroyed each time the SMS gets called.
	/// It is possible to reuse tehm when transfering them to members of the class.
	/// See TODO: hints.
	/// 
	/// diff.cs: A port of the algorythm to C#
	/// Copyright (c) by Matthias Hertel, http://www.mathertel.de
	/// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
	/// 
	/// Changes:
	/// 2002.09.20 There was a "hang" in some situations.
	/// Now I undestand a little bit more of the SMS algorithm. 
	/// There have been overlapping boxes; that where analyzed partial differently.
	/// One return-point is enough.
	/// A assertion was added in CreateDiffs when in debug-mode, that counts the number of equal (no modified) lines in both arrays.
	/// They must be identical.
	/// 
	/// 2003.02.07 Out of bounds error in the Up/Down vector arrays in some situations.
	/// The two vetors are now accessed using different offsets that are adjusted using the start k-Line. 
	/// A test case is added. 
	/// 
	/// 2006.03.05 Some documentation and a direct Diff entry point.
	/// 
	/// 2006.03.08 Refactored the API to static methods on the Diff class to make usage simpler.
	/// 2006.03.10 using the standard Debug class for self-test now.
	///            compile with: csc /target:exe /out:diffTest.exe /d:DEBUG /d:TRACE /d:SELFTEST Diff.cs
	/// 2007.01.06 license agreement changed to a BSD style license.
	/// 2007.06.03 added the Optimize method.
	/// 2007.09.23 UpVector and DownVector optimization by Jan Stoklasa ().
	/// 2008.05.31 Adjusted the testing code that failed because of the Optimize method (not a bug in the diff algorithm).
	/// 2008.10.08 Fixing a test case and adding a new test case.
	/// </summary>
	
	/// <summary>Data on one input file being compared.  
	/// </summary>
	private:
		class CDiffData
		{
		public:
			/// <summary>Number of elements (lines).</summary>
			int Length;

			/// <summary>Buffer of numbers that will be compared.</summary>
			std::vector<int> data;

			/// <summary>
			/// Array of booleans that flag for modified data.
			/// This is the result of the diff.
			/// This means deletedA in the first Data or inserted in the second Data.
			/// </summary>
			std::vector<bool> modified;

			/// <summary>
			/// Initialize the Diff-Data buffer.
			/// </summary>
			/// <param name="data">reference to the buffer</param>
			void Init() {
				Length = (int)data.size();
				modified.resize(Length + 2);
			} // DiffData
		};

		/// <summary>
		/// Shortest Middle Snake Return Data
		/// </summary>
		struct SMSRD
		{
			int x, y;
			// internal int u, v;  // 2002.09.20: no need for 2 points 
		};

		typedef std::unordered_map<std::string, int> Hashtable3;

	public:
		/// <summary>details of one difference.</summary>
		struct Item
		{
			/// <summary>Start Line number in Data A.</summary>
			int StartA;
			/// <summary>Start Line number in Data B.</summary>
			int StartB;

			/// <summary>Number of changes in Data A.</summary>
			int deletedA;
			/// <summary>Number of changes in Data B.</summary>
			int insertedB;
		}; // Item

		typedef std::vector<std::string> CListData;

		struct SMergeInfo
		{
			int startA;
			int deletedA;
			CListData vecInsert;
		};
		
	public:
		static void ByteStreamToListData(const char* data, uint32 dataSize, const char* delim, uint32 delimSize, CListData& vecCodedData);
		static void Diff(const CListData& vecCodedDataA, const CListData& vecCodedDataB, std::vector<Item>& vecDiff);
		static void GetMergeInfo(const std::vector<Item>& vecDiff, const CListData& vecCodedDataB, std::vector<SMergeInfo>& vecMergeInfo);
		static void Merge(const std::vector<SMergeInfo>& vecMergeInfo, const CListData& vecCodedDataA, CListData& vecCodedDataC);

	private:
		static void DiffCodes(const CListData& vecCodedData, Hashtable3& h, CDiffData& diffData); // DiffCodes
		static void Optimize(CDiffData& Data); // Optimize
		static SMSRD SMS(CDiffData& DataA, int LowerA, int UpperA, CDiffData& DataB, int LowerB, int UpperB, std::vector<int>& DownVector, std::vector<int>& UpVector); // SMS
		static void LCS(CDiffData& DataA, int LowerA, int UpperA, CDiffData& DataB, int LowerB, int UpperB, std::vector<int>& DownVector, std::vector<int>& UpVector); // LCS()
		static void CreateDiffs(const CDiffData& DataA, const CDiffData& DataB, std::vector<Item>& a);

	public:
		static int Test();
	};
}

#include "Niflect/Serialization/RwTree.h"
#include "Niflect/Test/TestDefinition.h"
#include <fstream>
#include "Niflect/Serialization/JsonFormat.h"

namespace TestDiffEditGraph
{
	static void TestLargeData()
	{
		using namespace QtNodeEditor;
		using namespace Niflect;
		using namespace RwTree;
		CDiff2::CListData lstA;
		{
			CRwNode root;
			std::ifstream ifs(CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/Diff/LargeData/a.json"), std::ios::binary);
			CJsonFormat::Read(&root, ifs);
			std::stringstream ss;
			CJsonFormat::Write(&root, ss);
			auto data = ss.str();
			CDiff2::ByteStreamToListData(data.data(), static_cast<uint32>(data.size()), "\n", 1, lstA);
		}
		CDiff2::CListData lstB;
		{
			CRwNode root;
			std::ifstream ifs(CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/Diff/LargeData/b.json"), std::ios::binary);
			CJsonFormat::Read(&root, ifs);
			std::stringstream ss;
			CJsonFormat::Write(&root, ss);
			auto data = ss.str();
			CDiff2::ByteStreamToListData(data.data(), static_cast<uint32>(data.size()), "\n", 1, lstB);
		}
		std::vector<CDiff2::Item> vecDiff;
		CDiff2::Diff(lstA, lstB, vecDiff);
		std::vector<CDiff2::SMergeInfo> vecMergeInfo;
		CDiff2::GetMergeInfo(vecDiff, lstB, vecMergeInfo);
		CDiff2::CListData lstMerged;
		CDiff2::Merge(vecMergeInfo, lstA, lstMerged);
		//Merge(lstA, lstB, vecDiff, lstMerged);
		ASSERT(lstMerged == lstB);
		printf("");
	}
}