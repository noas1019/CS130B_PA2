#include <iostream>
#include <list>
#include <vector>   
#include <math.h>
#include <sstream>
#include <unordered_map>
#include <iomanip>
#include "prog2.h"
using namespace std;

class Vertex {
public:
    double x;
    double y;
    int index;
    Vertex();
    Vertex(double x_cord, double y_cord, int index);
    double distance(Vertex other) const;
};

Vertex::Vertex() {
    this->x = 0.0;
    this->y = 0.0;
    this->index = 0;
}

Vertex::Vertex(double x_cord, double y_cord, int index) {
    this->x = x_cord;
    this->y = y_cord;
    this->index = index;
}

double Vertex::distance(Vertex other) const {
    double result = sqrt(pow((this->x - other.x), 2) + pow((this->y - other.y), 2));
    return result;
}

class Edge {
public:
    Vertex v0;
    Vertex v1;
    double weight;
    Edge(Vertex first, Vertex second);
};

Edge::Edge(Vertex first, Vertex second) {
    this->v0 = first;
    this->v1 = second;
    this->weight = first.distance(second);
}

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

unordered_map<int, int> makeSet(int n) {
    unordered_map<int, int> initial;
    for (int i = 0; i < n; i++) {
        initial[i] = -1;
    }
    return initial;
}

class UnionFind {
public:
    int* parent;
    int* rank;
    UnionFind(int n) {
        parent = new int[n];
        rank = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }
    int find(int n) {
        if (n != parent[n]) {
            parent[n] = find(parent[n]);
        }
        return parent[n];
    }
    void join(int a, int b) {
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
};

//int Find(int n, unordered_map<int, int>& parent) {
//    if (parent[n] == -1) {
//        return n;
//    }
//    else if (parent[n])
//    return Find(parent[n], parent);
//}

//void Union(int a, int b, unordered_map<int, int>& parent) {
//    int aParent = Find(a, parent);
//    int bParent = Find(b, parent);
//    parent[aParent] = bParent;
//}

//bool findCycle(unordered_map<int, int> parent, list<Edge> edges, Edge e) {
//    if (parent[e.v0.index] == parent[edges.front().v0.index] && parent[e.v1.index] == parent[edges.front().v0.index]) {
//        return true;
//    }
//    return false;
//}

int main() {
    
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



    /*unordered_map<int, int> parent = makeSet(n);
    cout << "Initial parent map:" << endl;
    for (auto const& pair : parent) {
        cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
    }
    for (Edge& e : edges) {
        cout << "Indexes: " << e.v0.index << " " << e.v1.index << ", Distance: " << e.v0.distance(e.v1) << endl;
        bool test = findCycle(parent, MCST, e);
        cout << "Is " << e.v0.index << " " << e.v1.index << " part of MCST already? " << test << endl;
        if (!findCycle(parent, MCST, e)) {
            MCST.push_back(e);
            cout << "Initial parent map:" << endl;
            for (auto const& pair : parent) {
                cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
            }
            Union(e.v0.index, e.v1.index, parent);
            cout << "Indexes sent to Union(): " << e.v0.index << ", " << e.v1.index << endl;
            cout << "New parents:" << endl;
            for (auto const& pair : parent) {
                cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
            }
        }
        bool finished = true;
        int temp = parent.begin()->second;
        for (auto const& pair : parent) {
            if (pair.second != temp) {
                finished = false;
            }
            temp = pair.second;
        }
        if (finished) {
            cout << "Finished!" << endl;
            break;
        }
    }
    cout << "Final parent map:" << endl;
    for (auto const& pair : parent) {
        cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
    }*/
    indexcompare icmp;
    MCST.sort(icmp);
    for (Edge& e : MCST) {
        cout << e.v0.index << " " << e.v1.index << endl;
    }

    //unordered_map<int, int> test = makeSet(n);
    //for (auto const& pair : test) {
    //    std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    //}

    //indexcompare cmp;
    //edges.sort(cmp);
    //int count = 0;
    //for (Edge& i : edges) {
    //    count++;
    //    cout << i.v0.index << " " << i.v1.index << endl;
    //}
    //cout << count << endl;

    return 0;
}