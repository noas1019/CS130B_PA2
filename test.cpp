#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

// Vertex object will represent coordinates of starting position, cannons and destination
class Vertex {
private:
    double x;
    double y;

public:
    Vertex();
    Vertex(double x_cord, double y_cord);
    double distance(Vertex other);
};

// default constructor for Vertex object
Vertex::Vertex() {
    this->x = 0;
    this->y = 0;
}

// parameterized constructor for Vertex object
Vertex::Vertex(double x_cord, double y_cord) {
    this->x = x_cord;
    this->y = y_cord;
}

// memberfunction returns distance between one Vertex and another
double Vertex::distance(Vertex other) {
    double result = sqrt(pow((this->x - other.x), 2) + pow((this->y - other.y), 2));
    return result;
}

// Adjacency matrix using vectors
class Graph {
private:
    vector< vector<double> > adjacents;

public:
    Graph(vector<Vertex> vertices, int size);
    double minimum_path(int size);
};

// Parameterized constructor for adjacency matrix that takes a vector of Vertex's as input
Graph::Graph(vector<Vertex> vertices, int size) {
    // initialize a (size+2) by (size+2) adjacency matrix full of 0's
    this->adjacents.resize(size + 2, vector<double>(size + 2));
    for (int i = 0; i < size + 2; i++) {
        for (int j = 0; j < size + 2; j++) {
            this->adjacents[i][j] = 0;
        }
    }
    // fill first row with distances for running from start to i'th cannon at vertices[i], or from start to destination if i is last index of vertices[i]
    for (int i = 1; i < size + 2; i++) {
        this->adjacents[0][i] = vertices[0].distance(vertices[i]) / 5;
    }
    // fill rest of rows with distances for launching from i'th cannon at vertices[i] and running rest of way to j'th cannon at vertices[j] (or running rest of way to destination if j is last index of vertices[j]),
    // or just the distance for running from vertices[i] to vertices[j] if that's shorter (to account for when cannons may overshoot the other vertex)
    for (int i = 1; i < size + 2; i++) {
        for (int j = 1; j < size + 2; j++) {
            this->adjacents[i][j] = min(2 + abs(vertices[i].distance(vertices[j]) - 50) / 5, vertices[i].distance(vertices[j]) / 5);
        }
    }
    // Thus far, each entry in adjacents[i][j] represents the minimum distance for going directly from vertices[i] to vertices[j], 
    // so if we iteratively compare each entry to all possible sub-paths (i.e. all possible values of k so that one can go from vertices[i] to vertices[k] to vertices[j]) such that the minimum value becomes the new entry,
    // our new adjacency matrix will now be fully optimized in the sense that adjacents[i][j] will now hold the shortest possible distance between vertices[i] and vertices[j] including sub-paths
    // (e.g. if going from vertices[i] to vertices[k] then to vertices[j] is shorter than directly going from vertices[i] to vertices[j], then adjacents[i][j] will now hold the distance for going from vertices[i] to vertices[k] then to vertices[j])
    for (int k = 0; k < size + 2; k++) {
        for (int i = 0; i < size + 2; i++) {
            for (int j = 0; j < size + 2; j++) {
                this->adjacents[i][j] = min(this->adjacents[i][j], this->adjacents[i][k] + this->adjacents[k][j]);
            }
        }
    }
}

// since adjacents[i][j] contains the shortest path from vertices[i] to vertices[j], 
// we simply return adjacents[start][last] to find the shortest distance between our starting and destination vertices
double Graph::minimum_path(int size) {
    return this->adjacents[0][size + 1];
}

int main() {
    ios_base::sync_with_stdio(false);

    double x1;
    double y1;
    double x2;
    double y2;
    int size;

    cin >> x1 >> y1 >> x2 >> y2 >> size;

    // populate vertices such that vertices[0] = starting coordinates, vertices[last] = destination coordinates, and vertices[i] = coordinates of i'th cannon for 0 < i < last
    vector<Vertex> vertices(size + 2);
    vertices[0] = Vertex(x1, y1);
    vertices[size + 1] = Vertex(x2, y2);

    for (int i = 1; i < size + 1; i++) {
        double x;
        double y;
        cin >> x >> y;
        vertices[i] = Vertex(x, y);
    }

    Graph G = Graph(vertices, size + 2);

    cout << G.minimum_path(size) << endl;

    return 0;
}