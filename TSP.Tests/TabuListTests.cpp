#include "stdafx.h"
#include "CppUnitTest.h"
#include <limits>
#include <algorithm>
#include <random>
#include "..\TSP\Graphs\AdjacencyMatrix\SymmetricAdjacencyMatrix.h"
#include "..\TSP\Solvers\TabuSearch\TabuList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;

using std::vector;
using std::string;

namespace TSPTests
{
	TEST_CLASS(TabuListTests)
	{
	public:

		TEST_METHOD(IsForbiddenTest)
		{
			TabuList list{ 10,1 };
			list.ForbidMove(MoveParameters{ 0,0 });
			Assert::IsTrue(list.IsForbidden(MoveParameters{ 0,0 }));
		}

		TEST_METHOD(ForbidMoveTest)
		{
			TabuList list{ 15,15 };
			for (int i = 0; i < 10; i++)			
				list.ForbidMove(MoveParameters{ i,i + 1 });			
			for (int i = 0; i < 10; i++)
				Assert::IsTrue( list.IsForbidden(MoveParameters{ i,i + 1 }));
			Assert::IsFalse( list.IsForbidden(MoveParameters{ 20,20 }));
		}

		TEST_METHOD(MaxLengthConstraintTest)
		{
			TabuList list = TabuList(10, 15);
			for (int i = 0; i < 10; i++)
				list.ForbidMove(MoveParameters{ i,i });
			Assert::IsTrue(list.IsForbidden(MoveParameters{ 0,0 }));
			list.ForbidMove(MoveParameters{ 10,10 });
			Assert::IsFalse(list.IsForbidden(MoveParameters{ 0,0 }));
		}
		
		TEST_METHOD(DecreaseTenureTest)
		{
			TabuList list = TabuList(10, 5);
			for (int i = 0; i < 10; i++)
				list.ForbidMove(MoveParameters{ i,i });
			for (int i = 0; i < 10; i++)
				Assert::IsTrue(list.IsForbidden(MoveParameters{ i,i }));
			for (int i = 0; i < 5; i++)
				list.DecreaseTenure();
			Assert::AreEqual(uint(0), list.GetListLenght());
			for (int i = 0; i < 10; i++)
				Assert::IsFalse(list.IsForbidden(MoveParameters{ i,i }));
		}
		
	};
}