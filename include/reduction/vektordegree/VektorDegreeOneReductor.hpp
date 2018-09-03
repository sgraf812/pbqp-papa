#ifndef REDUCTION_VEKTORDEGREE_VEKTORDEGREEONEREDUCTOR_HPP_
#define REDUCTION_VEKTORDEGREE_VEKTORDEGREEONEREDUCTOR_HPP_

#include <vector>
#include <reduction/PBQPReduction.hpp>

template<typename T>
class PBQP_Graph;
template<typename T>
class Dependent_Solution;
template<typename T>
class PBQP_Solution;
template<typename T>
class PBQP_Node;

/**
 * Any nodes with a vektor degree of 0 can be deleted from the graph. No selection needs to be made for them
 */
template<typename T>
class VektorDegreeOneReductor: public PBQP_Reduction<T> {
private:
	std::vector<unsigned int> nodes;

public:
	VektorDegreeOneReductor(PBQP_Graph<T>* graph) :
			PBQP_Reduction<T>(graph) {
		nodes = *new std::vector<int>();
	}

	~VektorDegreeOneReductor() {
	}

	std::vector<PBQP_Graph*>* reduce() {
		std::vector<PBQP_Node*> dependencyNodes =
				*new std::vector<PBQP_Node*>();
		std::vector<PBQP_Node*> solutionNodes = *new std::vector<PBQP_Node*>();
		for (PBQP_Node<T>* node : *(graph->getNodes())) {
			if (node->getVektorDegree() == 1) {
				solutionNodes.push_back(node);
			}
		}
		result->push_back(graph);
		return result;
	}

	PBQP_Solution<T>* solve(PBQP_Solution<T>* solution) {
		return solution;
	}

	static DependentSolution reduceVektorDegreeOne(PBQP_Node<T>* node,
			PBQP_Graph<T>* graph) {
		std::vector<PBQP_Node*> dependencyNodes =
				*new std::vector<PBQP_Node*>();
		std::vector<PBQP_Node*> solutionNodes = *new std::vector<PBQP_Node*>();
		solutionNodes.push_back(node);
		Dependent_Solution<T>* solution = new Dependent_Solution<T>(
				dependencyNodes, solutionNodes);
		std::vector<unsigned short int> nodeSolution = new std::vector<unsigned short int>();
		nodeSolution.push_back(0);
		solution->setSolution(&emptyIntVector, &nodeSolution);
		return solution;
	}
};



#endif /* REDUCTION_VEKTORDEGREE_VEKTORDEGREEONEREDUCTOR_HPP_ */
