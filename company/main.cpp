#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>
#include <fstream>

class Graph {
private:
	std::vector< std::list<size_t> > edges;
public:
	typedef std::list<size_t>::const_iterator EdgeIterator;

	explicit Graph(size_t numVertices = 0) :
		edges(numVertices, std::list<size_t>())
	{
	}

	void Reset(size_t newNumVertices) {
	edges.assign(newNumVertices, std::list<size_t>());
	}

	void AddEdge(size_t source, size_t target) {
		edges[source].push_back(target);
	}

	size_t GetNumVertices() const {
		return edges.size();
	}

	size_t GetCounterComp(size_t vertex) {
		return 0;
	}

	EdgeIterator EdgesBegin(size_t source) const {
		return edges[source].begin();
	}

	EdgeIterator EdgesEnd(size_t source) const {
		return edges[source].end();
	}
};

void Initialize(std::vector<size_t> &order, size_t numVertices) {
	order.resize(numVertices);
	for (size_t i = 0; i < numVertices; ++i) {
		order[i] = i;
	}
}

void InvertGraph(const Graph &graph, Graph &newGraph) {
	newGraph.Reset(graph.GetNumVertices());
	for (size_t source = 0; source < graph.GetNumVertices(); ++source)
		for (Graph::EdgeIterator target = graph.EdgesBegin(source),
			end = graph.EdgesEnd(source); target != end; ++target ) {
				newGraph.AddEdge(*target, source);
		}
}

void ReadGraph(std::istream &stream, Graph *graph) {
	size_t numVertices, numEdges;
	stream >> numVertices >> numEdges;
	graph->Reset(numVertices);
	for (size_t edge_index = 0; edge_index < numEdges; ++edge_index) {
		size_t vertex1, vertex2, edge_type;
		stream >> vertex1 >> vertex2 >> edge_type;
		if (edge_type == 1) {
			graph->AddEdge(--vertex1, --vertex2);
		} else if (edge_type == 2) {
			graph->AddEdge(--vertex2, --vertex1);
		}
	}
}

class BasicDFSVisitor {
public:
	void VertexEnter(size_t vertex) {}
	void VertexExit(size_t vertex) {}
};

class TopSortVisitor : public BasicDFSVisitor {
private:
	std::vector<size_t> order;
public:
	void VertexExit(size_t vertex) {
		order.push_back(vertex);
	}
	std::vector<size_t> GetOrder() const {
		return order;
	}
};

class CondensationVisitor : public BasicDFSVisitor {
private:
	std::vector<size_t> colors;
	size_t compNum;
public:
	CondensationVisitor(size_t NumVertices) {
		compNum = 0;
		colors.resize(NumVertices);
	}
	void VertexEnter(size_t vertex) {
		++compNum;
	}
	void VertexExit(size_t curVertex) {
		colors[curVertex] = compNum;
	}
	std::vector<size_t> GetColors() const {
		return colors;
	}
	size_t GetNumComp() const {
		return compNum;
	}
};

template<typename DFSVisitor>
class DFS {
public:
	enum State { NOT_PROCESSED, PROCESSED };
private:
	const Graph *graph;
	std::vector<State> states;
public:
	explicit DFS(const Graph &graph) :
		graph(&graph),
		states(graph.GetNumVertices(), NOT_PROCESSED)
	{
	}

	State GetState(size_t vertex) const {
		return states[vertex];
	}

	void ProcessComponent(const size_t curVertex, DFSVisitor &visitor) {
		if (states[curVertex] != NOT_PROCESSED) {
			return;
		}
		states[curVertex] = PROCESSED;
		for (Graph::EdgeIterator target = graph->EdgesBegin(curVertex),
			end = graph->EdgesEnd(curVertex); target != end; ++target) {
				ProcessComponent(*target, visitor);
		}
		visitor.VertexExit(curVertex);
	}
	
	template<typename IteratorType>
	void ProcessAll(IteratorType begin, IteratorType end, DFSVisitor &visitor) {
		for (IteratorType curVertex = begin; curVertex < end; ++curVertex) {
			if (states[*curVertex] != PROCESSED) {
				ProcessComponent(*curVertex, visitor);
				visitor.VertexEnter(*curVertex);
			}
		}
	}
};

size_t GetMinComp(const Graph& graph) {
	DFS<TopSortVisitor> topSortDFS(graph);
	TopSortVisitor topSortVisitor;
	std::vector<size_t> order;
	Initialize(order, graph.GetNumVertices());
	topSortDFS.ProcessAll(order.begin(), order.end(), topSortVisitor);

	Graph reversedGraph;
	InvertGraph(graph, reversedGraph);

	DFS<CondensationVisitor> condensationDFS(reversedGraph);
	order = topSortVisitor.GetOrder();
	CondensationVisitor condensationVisitor(reversedGraph.GetNumVertices());
	condensationDFS.ProcessAll(order.rbegin(), order.rend(), condensationVisitor);

	std::vector<size_t> color = condensationVisitor.GetColors();
	std::vector<size_t> counterComp;
	counterComp.assign(condensationVisitor.GetNumComp(), 0);
	for (size_t vertex = 0; vertex < graph.GetNumVertices(); ++vertex) {
		++counterComp[color[vertex]];
	}

	std::vector<bool> roots(condensationVisitor.GetNumComp(), true);
	for (size_t vertex = 0; vertex < graph.GetNumVertices(); ++vertex)
		for (Graph::EdgeIterator target = graph.EdgesBegin(vertex),
			end = graph.EdgesEnd(vertex); target != end; ++target)
				if (color[vertex] != color[*target]) { 
					roots[color[*target]] = false;
				}

	size_t minComp = std::numeric_limits<size_t>::max();
	for (size_t vertex = 0; vertex < condensationVisitor.GetNumComp(); ++vertex)
		if (roots[vertex]) {
			minComp = std::min(minComp, counterComp[vertex]);
		}

	return minComp;
}

int main() {
	Graph graph;
	ReadGraph(std::cin, &graph);
	std::cout << graph.GetNumVertices() - GetMinComp(graph) + 1 << '\n';
	return 0;
}
