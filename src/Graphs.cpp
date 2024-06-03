#include <cstdio>

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
// 
// - Edge List
// - Adjacency Matrix
// - Adjacency List
// --------------------------------------------------------------------------------

void Graphs()
{
}

// --------------------------------------------------------------------------------
// Traverse: Depth First and Breadth First
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

// --------------------------------------------------------------------------------
// Dijkstra algorithm
// 
// Given a weighted graph (only with positive values) and a source vertex, find the
// shortest paths from that source vertex to all the other vertices in the graph.
// 
// https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Bellman-Ford algorithm
// 
// Given a weighted graph (with either positive or negative values) and a source
// vertex, find the shortest paths from that vertex to all the other vertices in
// the graph.
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

// --------------------------------------------------------------------------------
// A*
// --------------------------------------------------------------------------------
