#include <cstdio>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <algorithm>
#include <ranges>

// --------------------------------------------------------------------------------
// Graph
// 
// A graph is a set of vertices and edges connecting those vertices.
// 
// Graphs can be used to model and solve a wide range of problems, including path finding,
// data clustering, network analysis, and machine learning.
// 
// - Directed or Undirected: Edges have direction or not.
// - Max edges in directed graph: n(n-1)
// - Max edges in undirected graph: n(n-1)/2
// - Weighted or non-weighted: Edges have some value that is the cost of going from
//                             one vertex to the other.
// - Strongly connected graph: There is always a path from any vertex to any other vertex.
// - Cycle: Path that starts and ends in the same vertex.
// - Simple graph: graph with no cycles
// - Directed Acyclic Graph (DAG): Directed graph with no cycles.
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Graph Representations
// --------------------------------------------------------------------------------

struct Edge
{
    int m_vertex1 = 0;
    int m_vertex2 = 0;
    float m_weight = 0.0f;
};

class Graph
{
public:
    Graph(bool isDirected = true)
        : m_isDirected(isDirected)
    {
    }
    virtual ~Graph() = default;

    virtual std::vector<Edge> GetEdges(int v) const = 0;
    virtual float GetEdge(int v1, int v2) const = 0;
    virtual void SetEdge(int v1, int v2, float weight) = 0;
    virtual void Print() const = 0;

protected:
    bool m_isDirected = true;
};

// --------------------------------------------------------------------------------
// Graph Representation: Edge List
// 
// It keeps a list of edges between vertices.
// 
// + Simple
// - To get the list of edges of a vertex or to check if a vertex is connected to another
//   we have to search the entire edge list. O(e)
// - Number of nodes information is not directly stored, extra data is necessary to keep it.
// --------------------------------------------------------------------------------

using EdgeList = std::vector<Edge>;

class GraphEdgeList : public Graph
{
public:
    GraphEdgeList(bool isDirected = true)
        : Graph(isDirected)
    {
    }

    std::vector<Edge> GetEdges(int v) const override
    {
        std::vector<Edge> edges;
        edges.reserve(m_edgeList.size());

        if (m_isDirected)
        {
            for (const auto& edge : m_edgeList)
            {
                if (edge.m_vertex1 == v)
                {
                    edges.push_back(edge);
                }
            }
        }
        else
        {
            for (const auto& edge : m_edgeList)
            {
                if (edge.m_vertex1 == v ||
                    edge.m_vertex2 == v)
                {
                    edges.push_back(edge);
                }
            }
        }

        edges.shrink_to_fit();
        return edges;
    }

    float GetEdge(int v1, int v2) const override
    {
        auto it = std::ranges::find_if(m_edgeList,
            [&](const Edge& edge)
            {
                if (m_isDirected)
                {
                    return (edge.m_vertex1 == v1 && edge.m_vertex2 == v2);
                }
                else
                {
                    return
                        (edge.m_vertex1 == v1 && edge.m_vertex2 == v2) ||
                        (edge.m_vertex1 == v2 && edge.m_vertex2 == v1);
                }
            });

        return (it != m_edgeList.end())
            ? it->m_weight
            : 0.0f;
    }

    void SetEdge(int v1, int v2, float weight) override
    {
        if (weight == 0.0f)
        {
            return;
        }

        // NOTE: It should check if edge already exists. Not doing it for simplicity.
        m_edgeList.emplace_back(v1, v2, weight);
    }

    void Print() const override
    {
        for (const auto& edge : m_edgeList)
        {
            std::printf("%d --(%.1f)--%s %d\n", 
                edge.m_vertex1, edge.m_weight, (m_isDirected) ? ">" : "", edge.m_vertex2);
        }
        std::printf("\n");
    }

private:
    EdgeList m_edgeList;
};

void GraphsAsEdgeList()
{
    GraphEdgeList graph;
    graph.SetEdge(0, 1, 7.0f);
    graph.SetEdge(1, 3, 5.0f);
    graph.SetEdge(2, 0, 2.0f);
    graph.SetEdge(2, 1, 1.0f);
    graph.SetEdge(2, 4, 6.0f);
    graph.SetEdge(3, 5, 7.0f);
    graph.SetEdge(4, 1, 3.0f);
    graph.SetEdge(4, 3, 9.0f);
    graph.SetEdge(4, 5, 4.0f);

    graph.Print();
}

// ---------------------------------------------
// Graph Representation: Adjacency Matrix
// 
// It keeps a matrix of vertices x vertices with the weight (0 is not connected).
// If graph is indirected the matrix will be symmetric.
// 
// + To check if a vertex is connected to another is O(1)
// - To get the list of edges of a vertex we have to search
//   its entire row of vertices. O(v)
// - Uses O(v^2) space, which is a waste if graph has few edges.
// ---------------------------------------------

using AdjecencyMatrix = std::vector<std::vector<float>>;

class GraphAdjecencyMatrix : public Graph
{
public:
    GraphAdjecencyMatrix(int vertexCount, bool isDirected = true)
        : Graph(isDirected)
        , m_matrix(vertexCount)
    {
        std::ranges::for_each(m_matrix,
            [vertexCount](std::vector<float>& vertexEdges)
            {
                vertexEdges.resize(vertexCount, 0.0f);
            });
    }

    std::vector<Edge> GetEdges(int v) const override
    {
        if (v >= m_matrix.size())
        {
            return {};
        }

        std::vector<Edge> edges;
        edges.reserve(m_matrix[v].size());

        for (int v2 = 0; v2 < m_matrix[v].size(); ++v2)
        {
            if (m_matrix[v][v2] != 0.0f)
            {
                edges.emplace_back(v, v2, m_matrix[v][v2]);
            }
        }

        edges.shrink_to_fit();
        return edges;
    }

    float GetEdge(int v1, int v2) const override
    {
        if (v1 >= m_matrix.size() ||
            v2 >= m_matrix.size())
        {
            return 0.0f;
        }

        return m_matrix[v1][v2];
    }

    void SetEdge(int v1, int v2, float weight) override
    {
        if (weight == 0.0f)
        {
            return;
        }

        if (v1 >= m_matrix.size() ||
            v2 >= m_matrix.size())
        {
            return;
        }

        m_matrix[v1][v2] = weight;
        if (!m_isDirected)
        {
            m_matrix[v2][v1] = weight;
        }
    }

    void Print() const override
    {
        std::printf("    ");
        for (int i = 0; i < m_matrix.size(); ++i)
        {
            std::printf("%d   ", i);
        }
        std::printf("\n");
        std::printf("    ");
        for (int i = 0; i < m_matrix.size(); ++i)
        {
            std::printf("----");
        }
        std::printf("\n");
        for (int i = 0; i < m_matrix.size(); ++i)
        {
            std::printf("%d | ", i);
            for (int j = 0; j < m_matrix[i].size(); ++j)
            {
                std::printf("%.1f ", m_matrix[i][j]);
            }
            std::printf("\n");
        }
        std::printf("\n");
    }

private:
    AdjecencyMatrix m_matrix;
};

void GraphsAsAdjacencyMatrix()
{
    const int graphVertexCount = 6;
    GraphAdjecencyMatrix graph(graphVertexCount);
    graph.SetEdge(0, 1, 7.0f);
    graph.SetEdge(1, 3, 5.0f);
    graph.SetEdge(2, 0, 2.0f);
    graph.SetEdge(2, 1, 1.0f);
    graph.SetEdge(2, 4, 6.0f);
    graph.SetEdge(3, 5, 7.0f);
    graph.SetEdge(4, 1, 3.0f);
    graph.SetEdge(4, 3, 9.0f);
    graph.SetEdge(4, 5, 4.0f);

    graph.Print();
}

// ---------------------------------------------
// Graph Representation: Adjacency List
// 
// For each vertex it keeps a list of edges.
// 
// + To get the list of edges of a vertex is O(1)
// + To check if a vertex is connected to another is O(d),
//   where d is the degree of connection of the vertex (0 <= d < v).
// - When graph is undirected it stores the edges twice.
// ---------------------------------------------

// The list of edges can be represented using different
// data structures: vector, list, balanced binary search tree, hash table.
using AdjecencyList = std::vector<Edge>;

class GraphAdjecencyList : public Graph
{
public:
    GraphAdjecencyList(int vertexCount, bool isDirected = true)
        : Graph(isDirected)
        , m_vertices(vertexCount)
    {
    }

    std::vector<Edge> GetEdges(int v) const override
    {
        if (v >= m_vertices.size())
        {
            return {};
        }

        return m_vertices[v];
    }

    float GetEdge(int v1, int v2) const override
    {
        if (v1 >= m_vertices.size() ||
            v2 >= m_vertices.size())
        {
            return 0.0f;
        }

        auto it = std::ranges::find_if(m_vertices[v1],
            [v2](const Edge& edge)
            {
                return edge.m_vertex2 == v2;
            });

        return (it != m_vertices[v1].end())
            ? it->m_weight
            : 0.0f;
    }

    void SetEdge(int v1, int v2, float weight) override
    {
        if (weight == 0.0f)
        {
            return;
        }

        if (v1 >= m_vertices.size() ||
            v2 >= m_vertices.size())
        {
            return;
        }

        // NOTE: It should check if edge already exists. Not doing it for simplicity.
        m_vertices[v1].emplace_back(v1, v2, weight);
        if (!m_isDirected)
        {
            m_vertices[v2].emplace_back(v2, v1, weight);
        }
    }

    void Print() const override
    {
        for (int v = 0; v < m_vertices.size(); ++v)
        {
            std::printf("%d: ", v);
            for (int e = 0; e < m_vertices[v].size(); ++e)
            {
                std::printf("(%d, %0.1f) ", m_vertices[v][e].m_vertex2, m_vertices[v][e].m_weight);
            }
            std::printf("\n");
        }
        std::printf("\n");
    }

private:
    std::vector<AdjecencyList> m_vertices;
};

void GraphsAsAdjacencyList()
{
    const int graphVertexCount = 6;
    GraphAdjecencyList graph(graphVertexCount);
    graph.SetEdge(0, 1, 7.0f);
    graph.SetEdge(1, 3, 5.0f);
    graph.SetEdge(2, 0, 2.0f);
    graph.SetEdge(2, 1, 1.0f);
    graph.SetEdge(2, 4, 6.0f);
    graph.SetEdge(3, 5, 7.0f);
    graph.SetEdge(4, 1, 3.0f);
    graph.SetEdge(4, 3, 9.0f);
    graph.SetEdge(4, 5, 4.0f);

    graph.Print();
}

// --------------------------------------------------------------------------------
// Traversing a graph: Depth First and Breadth First
// 
// The traverse algorithms are the same as in Trees (see Trees.cpp), but it's required
// to keep a vertex visited list to avoid cycles.
// 
// Applications of Depth First Search (DFS):
// - Detecting cycles in a graph by tracking visited edges.
// - Path finding from vertex A to B. use a stack to keep track of visited vertices,
//   once B is found, return the content of the stack.
// - Solving puzzles with only one solution, such as mazes.
// 
// Applications of Breadth First Search (BFS):
// - Shortest path in unweighted graphs (path with the least number of edges).
// - Find all neighbors. Peer-to-Peer networks, Social Networks, GPS System.
// - Detecting cycles in undirected graph.
// - Path finding, detect if there is a path between A and B.
// - AI: find best move
// - Find all connected components in an undirected graph.
// - Image processing: Flood-fill an image with a particular color.
// --------------------------------------------------------------------------------

void TraverseDepthFirst_Recursive(const Graph* graph, int v, std::vector<int>& visited)
{
    if (!graph)
    {
        return;
    }

    std::printf("%d ", v);
    visited.push_back(v);

    std::ranges::for_each(graph->GetEdges(v), 
        [graph, &visited](const Edge& edge)
        {
            if (std::ranges::find(visited, edge.m_vertex2) == visited.end())
            {
                TraverseDepthFirst_Recursive(graph, edge.m_vertex2, visited);
            }
        });
}

void TraverseDepthFirst_NonRecursive(const Graph* graph, int v)
{
    std::stack<int> stack;
    std::vector<int> visited;

    stack.push(v);
    visited.push_back(v);

    while (!stack.empty())
    {
        int vertex = stack.top();
        stack.pop();

        std::printf("%d ", vertex);

        // Notice the reverse order!
        std::ranges::for_each(graph->GetEdges(vertex) | std::views::reverse,
            [&stack, &visited](const Edge& edge)
            {
                if (std::ranges::find(visited, edge.m_vertex2) == visited.end())
                {
                    stack.push(edge.m_vertex2);
                    visited.push_back(edge.m_vertex2);
                }
            });
    }
}

void TraverseBreathFirst_NonRecursive(const Graph* graph, int v)
{
    std::queue<int> queue;
    std::vector<int> visited;

    queue.push(v);
    visited.push_back(v);

    while (!queue.empty())
    {
        int vertex = queue.front();
        queue.pop();

        std::printf("%d ", vertex);

        std::ranges::for_each(graph->GetEdges(vertex),
            [&queue, &visited](const Edge& edge)
            {
                if (std::ranges::find(visited, edge.m_vertex2) == visited.end())
                {
                    queue.push(edge.m_vertex2);
                    visited.push_back(edge.m_vertex2);
                }
            });
    }
}

void GraphsTraverse()
{
    const int graphVertexCount = 6;
    GraphAdjecencyList graph(graphVertexCount);
    graph.SetEdge(0, 1, 7.0f);
    graph.SetEdge(1, 3, 5.0f);
    graph.SetEdge(2, 0, 2.0f);
    graph.SetEdge(2, 1, 1.0f);
    graph.SetEdge(2, 4, 6.0f);
    graph.SetEdge(3, 5, 7.0f);
    graph.SetEdge(4, 1, 3.0f);
    graph.SetEdge(4, 3, 9.0f);
    graph.SetEdge(4, 5, 4.0f);

    std::printf("TraverseDepthFirst_Recursive vertex 2: ");
    std::vector<int> visited;
    TraverseDepthFirst_Recursive(&graph, 2, visited);
    std::printf("\n");

    std::printf("TraverseDepthFirst_NonRecursive vertex 2: ");
    TraverseDepthFirst_NonRecursive(&graph, 2);
    std::printf("\n");

    std::printf("TraverseBreathFirst_NonRecursive vertex 2: ");
    TraverseBreathFirst_NonRecursive(&graph, 2);
    std::printf("\n");
}

// --------------------------------------------------------------------------------
// Dijkstra algorithm
// 
// Given a weighted graph (only with positive values) and a source vertex, it finds
// the shortest paths from that source vertex to all the other vertices in the graph.
// 
// It can also be used to find the shortest path to a specific destination node,
// by terminating the algorithm once the shortest path to the destination node is known.
// 
// https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
// https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// A*
// 
// A* is a modification of Dijkstra’s Algorithm that is optimized for a single goal.
// A* achieves better performance by using heuristics to guide its search.
// A* cost function that tries to minimize is f(n) = g(n) + h(n), where g(n) is the cost from
// origin to the next node in the path (as in Dijkstra) and h(n) is a heuristic value
// (typically precomputed) that estimates the cost from the next node to the single goal.
// 
// https://theory.stanford.edu/~amitp/GameProgramming/AStarComparison.html
// https://www.geeksforgeeks.org/a-search-algorithm/
// https://www.redblobgames.com/pathfinding/a-star/introduction.html
// https://en.wikipedia.org/wiki/A*_search_algorithm
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Bellman-Ford algorithm
// 
// Given a weighted graph (with either positive or negative values) and a source
// vertex, it finds the shortest paths from that vertex to all the other vertices
// in the graph.
// 
// https://www.geeksforgeeks.org/bellman-ford-algorithm-dp-23/
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Floyd Warshall algorithm
// 
// Given a weighted graph (with either positive or negative values), it finds the
// shortest paths between all pairs of nodes.
// 
// https://www.geeksforgeeks.org/floyd-warshall-algorithm-dp-16/
// --------------------------------------------------------------------------------
