#include <iostream>
#include <list>
#include <vector>   
#include <math.h>
#include <sstream>
#include <unordered_map>
#include <iomanip>
#include "prog2.h"
using namespace std;

// custom Vertex class to hold x,y coordinates, index and Euclidean distance member function
class Vertex {
public:
    double x;
    double y;
    int index;
    Vertex();
    Vertex(double x_cord, double y_cord, int index);
    double distance(Vertex other) const;
};

// default constructor
Vertex::Vertex() {
    this->x = 0.0;
    this->y = 0.0;
    this->index = 0;
}

// parameterized constructor
Vertex::Vertex(double x_cord, double y_cord, int index) {
    this->x = x_cord;
    this->y = y_cord;
    this->index = index;
}

// Euclidean distance member function
double Vertex::distance(Vertex other) const {
    double result = sqrt(pow((this->x - other.x), 2) + pow((this->y - other.y), 2));
    return result;
}

// custom Edge class to hold vertices and weight (i.e. Euclidean distance) of each edge 
class Edge {
public:
    Vertex v0;
    Vertex v1;
    double weight;
    Edge(Vertex first, Vertex second);
};

// parameterized constructor
Edge::Edge(Vertex first, Vertex second) {
    this->v0 = first;
    this->v1 = second;
    this->weight = first.distance(second);
}

// custom comparator class for Edge objects, will sort by first vertex indices then second vertex indices in increasing order
class indexcompare {
public:
    bool operator()(const Edge& a, const Edge& b) const {
        if (a.v0.index < b.v0.index) {
            return true;
        }
        else if (a.v0.index == b.v0.index) {
            if (a.v1.index < b.v1.index) {
                return true;
            }
        }
        return false;
    }
};

// custom comparator class for Edge objects, will sort by weights in increasing order
class weightcompare {
public:
    bool operator()(const Edge& a, const Edge& b) const {
        double aDist = a.v0.distance(a.v1);
        double bDist = b.v0.distance(b.v1);
        if (aDist < bDist) {
            return true;
        }
        return false;
    }
};

// custom dijoint set class to hold vertices' parent information
class UnionFind {
public:
    int* parent;
    int* rank;
    UnionFind(int n);
    int find(int n);
    void join(int a, int b);
};

// parameterized constructor, initially all vertices are parents of themselves and have rank 0
UnionFind::UnionFind(int n) {
    parent = new int[n];
    rank = new int[n];
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
}

// find member function to find true parent of the given vertex
int UnionFind::find(int n) {
    if (n != parent[n]) {
        parent[n] = find(parent[n]);
    }
    return parent[n];
}

// union member function to join two sets according to rank
void UnionFind::join(int a, int b) {
    int parent1 = find(a);
    int parent2 = find(b);
    if (rank[parent1] > rank[parent2]) {
        parent[parent2] = parent1;
    }
    else {
        parent[parent1] = parent2;
    }
    if (rank[parent1] == rank[parent2]) {
        rank[parent2]++;
    }
}

int main() {
    
    // parse input for vertices
    vector<Vertex> points;
    string vertex;
    int n;
    getline(cin, vertex);
    n = stoi(vertex);
    for (int i = 0; i < n; i++) {
        getline(cin, vertex);
        double x, y;
        istringstream iss(vertex);
        iss >> x;
        iss >> y;
        Vertex point(x, y, i);
        points.push_back(point);
    }

    // parse input for edges
    list<Edge> edges;
    string edge;
    int m;
    getline(cin, edge);
    m = stoi(edge);
    if (m == 0) {
        for (int q = 0; q < points.size(); q++) {
            for (int p = q + 1; p < points.size(); p++) {
                edges.push_back(Edge(points[q],points[p]));
            }
        }
    }
    for (int j = 0; j < m; j++) {
        getline(cin, edge);
        int i, k;
        istringstream iss(edge);
        iss >> i;
        iss >> k;
        Edge segment(points[i], points[k]);
        edges.push_back(segment);
    } 

    // Kruskal's: traverse list of all edges sorted by weight in ascending order and add to MCST if doing so does not result in cycle
    list<Edge> MCST;
    weightcompare wcmp;
    edges.sort(wcmp);
    UnionFind labels(n);
    for (Edge& e : edges) {
        int u = labels.find(e.v0.index);
        int v = labels.find(e.v1.index);
        if (u != v) {
            labels.join(u, v);
            MCST.push_back(e);
        }
    }
    indexcompare icmp;
    MCST.sort(icmp);
    for (Edge& e : MCST) {
        cout << e.v0.index << " " << e.v1.index << endl;
    }

    return 0;
}