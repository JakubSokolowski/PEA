#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TSP\SymmetricAdjacencyMatrix.h"
#include "..\TSP\GraphDataParser.h"
#include "..\TSP\HeldKarp.h"
#include "..\TSP\BranchAndBound.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TSP;

namespace TSPTests
{
	TEST_CLASS(GraphDataParsingTests)
	{
	public:
		TEST_METHOD(ParseFromCoordinatesFile)
		{
			std::string filepath = "";
			auto Graph = ParseGraphFile<SymmetricAdjacencyMatrix>("C:\\Users\\jakub\\Documents\\Visual Studio 2017\\Projects\\PEA\\TSP\\Symmetric Instances\\51.txt");
			Assert::AreEqual(uint(51), Graph.GetNumOfVertices());
			Assert::AreEqual(uint(1275), Graph.GetNumOfEdges());
		}
	};
}