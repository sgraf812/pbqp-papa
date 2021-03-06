#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DegreeOneReducerTests
#include <boost/test/unit_test.hpp>

#include <vector>
#include <set>

#include "graph/PBQPGraph.hpp"
#include "graph/Vector.hpp"
#include "graph/PBQPNode.hpp"
#include "graph/PBQPEdge.hpp"
#include "reduction/PBQPReduction.hpp"
#include "reduction/degree/DegreeOneReducer.hpp"
#include "graph/PBQPSolution.hpp"

#include "util/TestUtils.hpp"

namespace pbqppapa {

BOOST_AUTO_TEST_CASE(emptyGraphTest) {
	//make sure this doesnt explode
	PBQPGraph<int> graph;
	DegreeOneReducer<int> oneReducer(&graph);
	std::vector<PBQPGraph<int>*> result = oneReducer.reduce();
}

/*
BOOST_AUTO_TEST_CASE(simpleNodeReductionCount) {
	PBQPGraph<int> graph = PBQPGraph<int>();
	for (int i = 0; i < 100; i++) {
		int arr[] { 3, 1 };
		Vector<int> vek(2, arr);
		PBQPNode<int>* node = graph.addNode(vek);
	}
	auto iter = graph.getNodeBegin();
	PBQPNode<int>* first = *(graph.getNodeBegin());
	while (iter != graph.getNodeEnd()) {
		PBQPNode<int>* node = *iter++;
		int arr [] {2, 1, 5, 3};
		Matrix<int> mat (2, 2, arr);
		graph.addEdge(first, node, mat);
	}
	DegreeOneReducer<int> oneReducer(&graph);
	std::vector<PBQPGraph<int>*> result = oneReducer.reduce();
	BOOST_CHECK_EQUAL(result.size(), 1);
	PBQPGraph<int>* resultGraph = result[0];
	BOOST_CHECK_EQUAL(resultGraph->getNodeCount(), 1);
} */

BOOST_AUTO_TEST_CASE(simpleCalculation) {
	PBQPGraph<int> graph = PBQPGraph<int>();
	int node1Arr[] { 1, 2 };
	int node2Arr[] { 2, 3 };
	Vector<int> vek1 (2, node1Arr);
	Vector<int> vek2 (2, node2Arr);
	PBQPNode<int>* first = graph.addNode(vek1);
	PBQPNode<int>* second = graph.addNode(vek2);
	unsigned long firstIndex = first->getIndex();
	unsigned long secondIndex = second->getIndex();
	int edge1Arr[] {2,6,2,0};
	Matrix<int> mat1 (2, 2, edge1Arr);
	graph.addEdge(first, second, mat1);
	OnetoOneDependentSolution<int>* retSol = DegreeOneReducer<int>::reduceDegreeOne(second, &graph);
	BOOST_CHECK_EQUAL(graph.getNodeCount(), 1);
	BOOST_CHECK_EQUAL(graph.getEdgeCount(), 0);
	BOOST_CHECK_EQUAL(first->getVectorDegree(), 2);
	BOOST_CHECK_EQUAL(first->getVector().get(0), 5);
	BOOST_CHECK_EQUAL(first->getVector().get(1), 5);
	PBQPSolution<int> sol (2);
	sol.setSolution(firstIndex, 0);
	retSol->solve(&sol);
	BOOST_CHECK_EQUAL(sol.getSolution(secondIndex), 0);
	PBQPSolution<int> sol2 (2);
	sol2.setSolution(firstIndex, 1);
	retSol->solve(&sol2);
	BOOST_CHECK_EQUAL(sol2.getSolution(secondIndex), 1);
}

}
