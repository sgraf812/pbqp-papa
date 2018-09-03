#ifndef PBQPNODE_H_
#define PBQPNODE_H_

#include <vector>
#include <algorithm>
//#include "graph/Matrix.hpp"

template<typename T>
class PBQP_Edge;
template<typename T>
class Vektor;

template<typename T>
class PBQP_Node {
private:
	Vektor<T>* const values;
	std::vector<PBQP_Edge<T>*>* const incidentEdges = new std::vector<PBQP_Edge<T>*>();
	unsigned const int index;

public:
	/**
	 * Should only be used by PBQP_Graph internally. Index counter is held by PBQP_Graph instance
	 */
	PBQP_Node(unsigned const int index, Vektor<T>* const values) : values(values), index(index) {
	}

	/**
	 * Deletes only this node and its cost vektor. Cleanup of edges must be done by a higher level (usually PBQP_Graph)
	 */
	~PBQP_Node() {
		delete values;
		delete incidentEdges;
	}

	//TODO make proper iterators here
	std::vector<PBQP_Edge<T>*>* getAdjacentEdges(const bool respectDirection = false) const {
		if (!respectDirection) {
			return incidentEdges;
		}
		std::vector<PBQP_Edge<T>*>* outgoingEdges = new std::vector<PBQP_Edge<T>*>();
		//TODO Should we maybe just store this explicitly? Check back later how often we actually have to recompute this vector
		for (PBQP_Edge<T>* edge : *incidentEdges) {
			if (edge->isSource(this)) {
				outgoingEdges->push_back(edge);
			}
		}
		return outgoingEdges;
	}

	std::vector<PBQP_Node<T>*>* getAdjacentNodes(const bool respectDirection = false) const {
		//TODO Same as in the adjacent edge function, maybe we should just store all of this explictly to save computation time?
		//separate loops so we only check respectDirection once, instead of during every loop iteration
		std::set <PBQP_Node<T>*>* resultSet = new std::set <PBQP_Node<T>*>();
		std::vector <PBQP_Node<T>*>* nodes = new std::vector <PBQP_Node<T>*>();
		if (respectDirection) {
			for (PBQP_Edge<T>* edge : *incidentEdges) {
				PBQP_Node<T>* other = edge->getOtherEnd(this);
				if (edge->isSource(this) && resultSet->insert(other).second) {
					nodes->push_back(other);
				}
			}
		} else {
			for (PBQP_Edge<T>* edge : *incidentEdges) {
				PBQP_Node<T>* other = edge->getOtherEnd(this);
				if(resultSet->insert(other).second) {
					nodes->push_back(other);
				}
			}
		}
		return nodes;
	}

	/**
	 * Index identifies this node uniquely in the graph it was created in, even after the node is deleted
	 */
	unsigned int getIndex() const {
		return index;
	}

	/**
	 * Gets the length (amount of rows) of the cost vektor
	 */
	unsigned short int getVektorDegree() const {
		return values->getRowCount();
	}

	/**
	 * Gets the amount of edges connected to this node
	 */
	unsigned int getDegree() const {
		return incidentEdges->size();
	}

	/**
	 * Gets the cost vektor associated with this node
	 */
	Vektor<T>* getVektor() const {
		return values;
	}

	/**
	 * Compares based on index, needed so we can sort nodes into maps
	 */
	bool operator< (const PBQP_Node<T>& e) const {
		return this->index < e.index;
	}

	/**
	 * Compares based on index,
	 */
	bool operator==(const PBQP_Node<T>& e) const {
		return this->index == e.index;
	}

	/**
	 * Should only be used by PBQP_Graph internally.
	 */
	void addEdge(PBQP_Edge<T>* edge) {
		incidentEdges->push_back(edge);
	}

	/**
	 * Should only be used by PBQP_Graph internally.
	 */
	void removeEdge(PBQP_Edge<T>* edge) {
		//Erase�remove idiom
		incidentEdges->erase(
				std::remove(incidentEdges->begin(), incidentEdges->end(), edge),
				incidentEdges->end());
	}
};

#endif /* PBQPNODE_H_ */
