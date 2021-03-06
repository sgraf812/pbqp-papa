#ifndef REDUCTION_HEURISTIC_DEGREENREDUCTOR_HPP_
#define REDUCTION_HEURISTIC_DEGREENREDUCTOR_HPP_

#include <vector>
#include <reduction/PBQPReduction.hpp>
#include <reduction/solutions/DependentSolution.hpp>

namespace pbqppapa {

template<typename T>
class PBQPGraph;
template<typename T>
class NtoNDependentSolution;
template<typename T>
class PBQPSolution;
template<typename T>
class PBQPNode;
template<typename T>
class PBQP_Reduction;

template<typename T>
class DegreeNReducer: PBQP_Reduction<T> {

public:
	static ImmediateSolution<T>* reduceRNEarlyDecision(PBQPNode<T>* node,
			PBQPGraph<T>* graph) {
		unsigned short minSelection = 0;
		T minCost = node->getVector().get(0);
		for (PBQPEdge<T>* edge : node->getAdjacentEdges()) {
			bool isSource = edge->isSource(node);
			T rowColMin = edge->getOtherEnd(node)->getVector().get(0);
			rowColMin += edge->getMatrix().get(0, 0);
			PBQPNode<T>* otherEnd = edge->getOtherEnd(node);
			for (unsigned short k = 1; k < otherEnd->getVectorDegree(); k++) {
				T localRowColMin = otherEnd->getVector().get(k);
				if (isSource) {
					localRowColMin += edge->getMatrix().get(0, k);
				} else {
					localRowColMin += edge->getMatrix().get(k, 0);
				}
				if (localRowColMin < rowColMin) {
					rowColMin = localRowColMin;
				}
			}
			minCost += rowColMin;
		}
		for (unsigned short i = 1; i < node->getVectorDegree(); i++) {
			T curr = node->getVector().get(i);
			if (curr >= minCost) {
				continue;
			}
			for (PBQPEdge<T>* edge : node->getAdjacentEdges()) {
				bool isSource = edge->isSource(node);
				T rowColMin = edge->getOtherEnd(node)->getVector().get(0);
				if (isSource) {
					rowColMin += edge->getMatrix().get(i, 0);
				} else {
					rowColMin += edge->getMatrix().get(0, i);
				}
				PBQPNode<T>* otherEnd = edge->getOtherEnd(node);
				for (unsigned short k = 1; k < otherEnd->getVectorDegree();
						k++) {
					T localRowColMin = otherEnd->getVector().get(k);
					if (isSource) {
						localRowColMin += edge->getMatrix().get(i, k);
					} else {
						localRowColMin += edge->getMatrix().get(k, i);
					}
					if (localRowColMin < rowColMin) {
						rowColMin = localRowColMin;
					}
				}
				curr += rowColMin;
			}
			if (curr < minCost) {
				minCost = curr;
				minSelection = i;
			}
		}
		//add to each adjacent node
		for (PBQPEdge<T>* edge : node->getAdjacentEdges()) {
			bool isSource = edge->isSource(node);
			PBQPNode<T>* otherEnd = edge->getOtherEnd(node);
			for(int i = 0; i < otherEnd->getVectorDegree(); i++) {
				if (isSource) {
					otherEnd->getVector().get(i) += edge->getMatrix().get(minSelection, i);
				}
				else {
					otherEnd->getVector().get(i) += edge->getMatrix().get(i, minSelection);
				}
			}
		}
		ImmediateSolution<T>* sol = new ImmediateSolution<T>(node, minSelection);
		graph->removeNode(node);
		return sol;
	}

};

}

#endif /* REDUCTION_HEURISTIC_DEGREENREDUCTOR_HPP_ */
