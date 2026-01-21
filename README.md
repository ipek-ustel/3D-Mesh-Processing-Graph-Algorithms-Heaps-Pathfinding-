This project implements a high-performance system for parsing, navigating, and analyzing 3D Meshes, which are specialized undirected graphs representing objects in 3D space. Developed as part of the CENG 213 Data Structures course at Middle East Technical University (METU).

## Key Features

**OFF File Parser:** Efficiently reads and parses Object File Format (.off) files, constructing a manifold mesh structure consisting of vertices, edges, and triangular surfaces.

**Graph Navigation (BFS):** Implements Breadth-First Search (BFS) to compute the "Jump Count" (minimum number of edges) between any two vertices in the mesh.

**Geodesic Distance (Dijkstra):** Calculates the shortest path between vertices on the mesh surface using Dijkstra’s Algorithm, where edge weights are defined by Euclidean distances in 3D space.

**Efficient Edge Management (Min-Heap):** Manages thousands of mesh edges using a Min-Heap, allowing for $O(1)$ access to the shortest edge and efficient retrieval of the $k$-th shortest edge.

## Technical Implementation

- Core Algorithms

**Floyd's Heap Construction:** Instead of inserting edges one-by-one, the system utilizes Floyd's algorithm (buildHeap) to construct the initial min-heap in $O(n)$ time.

**Dynamic Updates:** When a vertex coordinate is updated, the system re-calculates all connected edge lengths and restores the heap property using bubbleDown operations.

**Adjacency Management:** Each vertex maintains localized lists (vertList, triList, edgeList) to ensure constant-time access to its immediate neighborhood.

- Complexity Analysis

**Heap Construction:** $O(n)$ where $n$ is the number of edges.

**Pathfinding (Dijkstra):** $O(E \log V)$ using the optimized vertex traversal.

**K-th Shortest Edge:** $O(k \log n)$ by performing $k$ removals on a temporary copy of the heap.

## Project Structure

**DataStructures.h:** Defines Vertex, Edge, and Triangle structs with integrated 3D coordinates.

**Mesh.h / Mesh.cpp:** The core engine handling graph traversals, heap management, and mesh modifications.

**Arithmetic.h:** Inline utility functions for 3D vector math (dot product, cross product, Euclidean distance).
