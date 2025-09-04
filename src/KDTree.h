#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <set>
#include <queue>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

// Estructura que representa una grilla
struct Grid {
    vector<pair<long long, long long>> ranges;  // Rangos en cada dimension [start, end]
    set<vector<long long>> points;        // Conjunto de puntos en esta grilla
    long long next;                       // Eje en el cual se realiza el proceso de construcción (para kdtree bi)
    int level;
    Grid(long long d, long long S, int level);
    Grid(); // Declaracion del constructor
};

// Estructura que representa la respuesta de la grilla
struct Response {
    bit_vector bv;                 // Bitvector con la respuesta de la grilla
    vector<Grid> subgrids;
    int level;// Subgrillas resultantes con puntos

    Response(long long d, int level);               // Declaracion del constructor
};

class KDTree {
public:
    vector<vector<long long>> points;  // Lista de puntos
    long long S;                       // Tamaño de la grilla
    long long d;                       // Dimensiones del espacio
    queue<Grid> grids;          // Arreglo de grillas
    vector<int> attr_set;
    vector<bit_vector> bitvector;


    Response (KDTree::*get_response)(Grid&);  // Mtodo de creacion de las subgrillas
    
    KDTree(const vector<vector<long long>>& pts, long long S, long long d, const vector<int> &attr_set); // Declaración del constructor
    
    bit_vector representation; // Vector de bits que representa al arbol
    
    void build_tree(); // Constructor del vector de bits

    Response get_resp(Grid& g);

};

vector<bit_vector> join(vector<vector<bit_vector>> bitrees,vector<vector<int>> attr, int h, int d);
int intersect(vector<bit_vector>* bitvectors,vector<vector<bit_vector>> bitrees, vector<pair<int,int>> posiciones, vector<vector<int>> attr_set,int att, int p, int h);

#endif

