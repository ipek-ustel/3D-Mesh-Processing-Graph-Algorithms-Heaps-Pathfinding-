#ifndef MESH_H
#define HESH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "DataStructures.h"

using std::string;
using std::ifstream;
using std::stringstream;

class Mesh {
public:
	vector<Vertex> verts;
	vector<Edge> edges;
	vector<Triangle> tris;
	vector<size_t> minHeap;

	Mesh() {
		minHeap.push_back(0);
	};
	Mesh(const char* filename);
	~Mesh() {
		verts.clear();
		edges.clear();
		tris.clear();
		minHeap.clear();
	};

	inline Vertex& getVertex(size_t v) {
		if (v < verts.size()) {
			return verts[v];
		}
		else {
			return verts[0];
		}
	}

	inline Edge& getEdge(size_t e) {
		if (e < edges.size()) {
			return edges[e];
		}
		else {
			return edges[0];
		}
	}

	Edge& getEdge(size_t v1, size_t v2);

	inline Triangle& getTriangle(size_t t) {
		if (t < tris.size()) {
			return tris[t];
		}
		else {
			return tris[0];
		}
	}

	void printVertex(size_t v);
	void printTriangle(size_t t);

	bool isVertsNeighbour(size_t v1, size_t v2);
	size_t getDegree(size_t v);
	int getJumpCount(size_t v1, size_t v2);
	double getGeodesicDistance(size_t v1, size_t v2);
	void updateVertex(size_t v, double x, double y, double z);
	Edge& getKthShortestEdge(size_t k);

private:	
	size_t addVertex(double x, double y, double z);
	size_t addTriangle(size_t v1, size_t v2, size_t v3);
	void buildHeap();

	// Declare your helper functions here.
	
	void addEdge( size_t v1, size_t v2 );
	void bubbleDown(size_t hole, vector<size_t>& minHeap );
	void removeMin( vector<size_t>& copy_minHeap );
	
};

#endif // !MESH_H

