#include "Mesh.h"
#include <climits>
#include <queue>
#include <cfloat>

Mesh::Mesh(const char* filename)
{
	int nVerts, nTris, n, k = 0;
	double x, y, z;
	minHeap.push_back(0);

	ifstream file(filename);

	string line;
	getline(file, line);
	getline(file, line);
	stringstream str(line);
	str >> nVerts >> nTris >> n;

	while (k++ < nVerts)
	{
		getline(file, line);
		stringstream str2(line);

		str2 >> x >> y >> z;
		addVertex(x, y, z); // Not Implemented.
	}
	k = 0;
	while (k++ < nTris)
	{
		getline(file, line);
		stringstream str2(line);

		str2 >> n >> x >> y >> z;
		addTriangle((size_t)x, (size_t)y, (size_t)z); // Not Implemented.
	}

	file.close();

	buildHeap(); // Not Implemented.
}


size_t Mesh::addVertex(double x, double y, double z) {

    size_t vert_ind = verts.size();

    verts.push_back( Vertex(vert_ind, x, y, z) );

	return vert_ind;
}


void Mesh::addEdge(size_t v1, size_t v2){

    size_t v1_edge_num = verts[v1].edgeList.size();

    for(size_t i=0; i<v1_edge_num; i++){

        size_t neigh_edge_ind = verts[v1].edgeList[i];
        Edge& neigh_edge = edges[neigh_edge_ind];

        if( neigh_edge.v1i == v2 || neigh_edge.v2i == v2 ){
            return;
        }

    }

    size_t new_edge_ind = edges.size();

    Edge newEdge( new_edge_ind, v1, v2 );
    newEdge.length = dist( verts[v1].coords, verts[v2].coords );

    edges.push_back(newEdge);

    verts[v1].vertList.push_back(v2);
    verts[v2].vertList.push_back(v1);

    verts[v1].edgeList.push_back(new_edge_ind);
    verts[v2].edgeList.push_back(new_edge_ind);

    minHeap.push_back(new_edge_ind);

}


size_t Mesh::addTriangle(size_t v1, size_t v2, size_t v3) {

    size_t tri_ind = tris.size();

    tris.push_back( Triangle(tri_ind, v1, v2, v3) );

    verts[v1].triList.push_back(tri_ind);
    verts[v2].triList.push_back(tri_ind);
    verts[v3].triList.push_back(tri_ind);

    addEdge(v1,v2);
    addEdge(v3,v2);         
    addEdge(v1,v3);

	return tri_ind;
}


void Mesh::bubbleDown(size_t hole, vector<size_t>& minHeap){

    size_t tmp_edge_ind = minHeap[hole];

    size_t heap_size = minHeap.size()-1;

    if( hole*2 > heap_size ) return;
    
    size_t child;

    for( ; hole*2 <= heap_size ; hole = child ){

        child = hole*2;

        if( child+1 <= heap_size && edges[ minHeap[child+1] ].length < edges[ minHeap[child]].length ){
            child++;
        }

        if( edges[tmp_edge_ind].length > edges[ minHeap[child] ].length ){

            minHeap[hole] = minHeap[child];

        }

        else{
            break;
        }

    }

    minHeap[hole] = tmp_edge_ind;

}


void Mesh::buildHeap() {

    for(size_t i = (minHeap.size()-1)/2 ; i>0 ; i--){

        bubbleDown(i, minHeap);

    }

}


Edge& Mesh::getEdge(size_t v1, size_t v2) {

    size_t edge_num = edges.size();

    for(size_t i=0; i<edge_num; i++){

        if( (edges[i].v1i == v1 && edges[i].v2i == v2) || (edges[i].v1i == v2 && edges[i].v2i == v1)){

            return edges[i];

        }

    }

	return edges[0];
}

void Mesh::printVertex(size_t v) {

    size_t vert_num = verts.size();

    if(v >= vert_num ){
        return;
    }

    std::cout << verts[v].coords[0] << " ";
    std::cout << verts[v].coords[1] << " ";
    std::cout << verts[v].coords[2] << " ";

}

void Mesh::printTriangle(size_t t) {

    size_t tri_num = tris.size();

    if(t >= tri_num ){
        return;
    }

    std::cout << "3 ";
    std::cout << tris[t].v1i << " ";
    std::cout << tris[t].v2i << " ";
    std::cout << tris[t].v3i << " ";

}

bool Mesh::isVertsNeighbour(size_t v1, size_t v2) {

    size_t vert_num = verts.size();

    if(v1 >= vert_num || v2 >= vert_num){
        return false;
    }

    if(v1 == v2){
        return false;
    }

    size_t v1_edge_num = verts[v1].edgeList.size();

    for(size_t i=0; i<v1_edge_num; i++){

        size_t neigh_edge_ind = verts[v1].edgeList[i];
        Edge& neigh_edge = edges[neigh_edge_ind];

        if( neigh_edge.v1i == v2 || neigh_edge.v2i == v2 ){

            return true;

        }

    }

	return false;
}

size_t Mesh::getDegree(size_t v) {

    size_t vert_num = verts.size();

    if(v >= vert_num ){
        return 0;
    }

	return verts[v].edgeList.size();
}


int Mesh::getJumpCount(size_t v1, size_t v2)
{
    size_t vert_num = verts.size();

    if(v1 >= vert_num || v2 >= vert_num){
        return -1;
    }

    if(v1 == v2){
        return 0;
    }

	std::queue<size_t> q;
    vector<bool> visited(vert_num, false);
    vector<int> distance(vert_num, -1);

    q.push(v1);
    visited[v1] = true;
    distance[v1] = 0;

    while( !q.empty() ){

        size_t curr_vert_ind = q.front();
        q.pop();

        size_t curr_vert_neigh_num = verts[curr_vert_ind].vertList.size();

        for(size_t i=0; i<curr_vert_neigh_num; i++){

            size_t neigh_ind = verts[curr_vert_ind].vertList[i];

            if( !visited[neigh_ind] ){

                visited[neigh_ind] = true;
                distance[neigh_ind] = distance[curr_vert_ind] + 1;
                q.push( neigh_ind );
                
                if( neigh_ind == v2 ){
                    return distance[v2];
                }

            }

        }

    }

    return -1;


}

double Mesh::getGeodesicDistance(size_t v1, size_t v2) {

	size_t vert_num = verts.size();

    if(v1 >= vert_num || v2 >= vert_num){
        return -1;
    }

    if(v1 == v2){
        return 0;
    }

    vector<bool> visited(vert_num, false);
    vector<double> distance(vert_num, DBL_MAX);

    distance[v1] = 0;

    while( true ){

        double curr_min_dist = DBL_MAX;
        size_t curr_min_vert_ind;
        for(size_t i=0; i<vert_num; i++){

            if( !visited[i] && distance[i] < curr_min_dist ){
                curr_min_dist = distance[i];
                curr_min_vert_ind = i;
            }

        }
        
        if (curr_min_dist == DBL_MAX || curr_min_vert_ind == vert_num) {
            // No path exists or all vertices visited
            break;
        }
        
        if(curr_min_vert_ind == v2 ){
            return distance[v2];
        }

        visited[ curr_min_vert_ind ] = true;

        size_t curr_min_vert_neigh_num = verts[curr_min_vert_ind].vertList.size();

        for(size_t i=0; i<curr_min_vert_neigh_num; i++){

            size_t neigh_ind = verts[curr_min_vert_ind].vertList[i];

            if( !visited[neigh_ind] ){

                double temp_dist = distance[curr_min_vert_ind] + dist( verts[curr_min_vert_ind].coords, verts[neigh_ind].coords );
                
                if( temp_dist < distance[neigh_ind]){
                    distance[neigh_ind] = temp_dist;
                }
                
            }

        }

    }

}


void Mesh::updateVertex(size_t v, double x, double y, double z) {

    verts[v].coords[0] = x;
    verts[v].coords[1] = y;
    verts[v].coords[2] = z;

    size_t neigh_edge_num = verts[v].edgeList.size();

    for(size_t i=0; i<neigh_edge_num; i++){

        size_t neigh_edge_ind = verts[v].edgeList[i];
        Edge& neigh_edge = edges[neigh_edge_ind];

        size_t neigh_vert_ind;

        if( neigh_edge.v1i == v ){
            neigh_vert_ind = neigh_edge.v2i;
        }
        else{
            neigh_vert_ind = neigh_edge.v1i;
        }
        neigh_edge.length = dist( verts[v].coords, verts[neigh_vert_ind].coords );


    }

    buildHeap();

}


void Mesh::removeMin( vector<size_t>& copy_heap ){

    size_t edge_num = copy_heap.size()-1;

    if( edge_num <= 0 ){
        return;
    }

    copy_heap[1] = copy_heap[edge_num];
    copy_heap.pop_back();
    bubbleDown(1, copy_heap);

}


Edge& Mesh::getKthShortestEdge(size_t k) {

    size_t edge_num = minHeap.size()-1;

    if( k>edge_num ){
        return edges[0];
    }

    vector<size_t> copy_heap = minHeap;

	for(size_t i=1; i<=k; i++){
        removeMin(copy_heap);
    }

    return edges[ copy_heap[1] ];
    
}

// Implement your helper functions here.