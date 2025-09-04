#include "KDTree.h"
#include <iostream>
#include <cmath>
#include <map>

using namespace std;

// Funciones sacadas del libro del profesor Gonzalo Navarro
bool childrenlabeled(bit_vector bv, long long v,long long l, long long sigma){ 
    return bv[sigma*(v-1)+l-1]==1;
}

long long labeledchild(rank_support_v<1> rank, long long v, long long l, long long sigma){
    return rank(sigma*(v-1)+l)+1;
}

// Constructur nulo para correctitud del codigo
Grid::Grid() {
    next=0;
}


// Constructor de la clase Grid, se setean los d rangos en [1,S]
Grid::Grid(long long d, long long S, int level): level(level) {
    ranges.resize(d, {1, S});
}

// Constructor de la clase Response, depende del tipo de arbol utilizado
Response::Response(long long d, int level): level(level){
    bv.resize(2);
    bv[0]=0;
    bv[1]=0;

}

// Constructor de la clase KDTree
KDTree::KDTree(const vector<vector<long long>>& pts, long long S, long long d, const vector<int> &attr_set) : points(pts), S(S), d(d), attr_set(attr_set) {
    Grid root(d, S, 0);
    for (auto& p : points) {
        root.points.insert(p);
    }
    root.next=0;
    grids.push(root); // Se inserta la raiz al conjunto de grillas para inicializar la construccion
    get_response = &KDTree::get_resp;
    bitvector.resize((int) d*std::log2(S));
    cout << this->attr_set[0] <<endl;
}

// Funcion principal de construccion: genera bitvector y divide las grillas
void KDTree::build_tree() {
    while (!grids.empty()) {
        
        Grid g = grids.front();
        grids.pop();

        Response resp = (this->*get_response)(g);

        int level = resp.level;
        bit_vector bv = resp.bv;
        bitvector[level].resize(bitvector[level].size()+resp.bv.size());

        for (long long i = 0; i < resp.bv.size();i++){
            bitvector[level][i+bitvector[level].size()-resp.bv.size()]=resp.bv[i];
            
        }
     
        for (auto& subgrid : resp.subgrids) {
            bool is_trivial = true;
            for (const auto& range : subgrid.ranges) {
                if (range.first != range.second) {
                    is_trivial = false;
                    break;
                }
            }

            if (!is_trivial) {
                grids.push(subgrid);
                
            } 
        }

    }
    for (int i=0;i<bitvector.size();i++) {
        cout << bitvector[i] << endl;
    }


    cout << attr_set << endl;
}


// Algoritmo get_resp que genera el bitvector y las subgrillas
Response KDTree::get_resp(Grid& g) {
    Response resp(d,g.level);

    vector<vector<pair<long long, long long>>> subranges(2);
        
    long long axis = g.next;
       
    // se generan los subrangos
    for (long long i = 0; i < 2; ++i) {
        for (long long j = 0; j < d; ++j) {
            if (j==axis){
                long long mid = (g.ranges[j].second - g.ranges[j].first + 1) / 2;  // Para asegurar divisiones en mitades exactas
                if (i ==1) {
                    subranges[i].push_back({g.ranges[j].first + mid, g.ranges[j].second});
                } else {
                    subranges[i].push_back({g.ranges[j].first, g.ranges[j].first + mid - 1});
                }
            } else {
                subranges[i].push_back(g.ranges[j]);
            
            }
        }
    }



    // Dividir los puntos en subgrillas segun los rangos
    vector<Grid> subgrids(2, Grid(d, S,g.level+1));
    for (auto& p : g.points) {
        for (long long i = 0; i < 2; ++i) {
            bool in_subgrid = true;
            for (long long j = 0; j < d; ++j) {
                if (p[j] < subranges[i][j].first || p[j] > subranges[i][j].second) {
                    in_subgrid = false;
                    break;
                }
            }

            if (in_subgrid) {
                subgrids[i].points.insert(p);
                subgrids[i].next=(g.next+1)%d;
                resp.bv[i] = 1;  // Marcar el bit correspondiente
                
                // Asignar los rangos de subranges a las subgrillas
                subgrids[i].ranges = subranges[i];
                subgrids[i].next=(axis+1)%d;
            }
        }
    }

    
    // se añaden las grillas no vacias a la respuesta    
    for (long long i = 0; i < subgrids.size(); ++i) {
        if (!subgrids[i].points.empty()) {
         
            resp.subgrids.push_back(subgrids[i]);
        }
    }

    return resp;
}

int intersect(vector<bit_vector>* bitvectors, vector<vector<bit_vector>> bitrees,vector<pair<int,int>> posiciones, vector<vector<int>> attr_set,int att, int p, int h) {
    cout<<"intersecting"<<endl;
    cout<<"positions"<<endl;
    for (int i=0;i<posiciones.size();i++) {
        cout<<posiciones[i].first<<", "<<posiciones[i].second<<endl;
    }

    cout<<p <<" >= "<< h <<endl;
    if (p >= h){
        return 1;
    }
    cout<<"Creating initial c"<<endl;
    bit_vector c = bit_vector(2,1);

    cout<<"Intersecting with the correspondents bitrees"<<endl;
    for (auto& j: attr_set[att]) {
        cout<<"Tree " << j <<endl;
        cout<<"c[0]"<<endl;
        c[0]= c[0] && bitrees[j][posiciones[j].first][posiciones[j].second];
        cout<<"c[1]"<<endl;
        c[1]= c[1] && bitrees[j][posiciones[j].first][posiciones[j].second+1];
    }
    // Línea 4 de intersect para abajo...
    cout<< "Updating the positions"<<endl;
    vector<pair<int,int>> newposiciones;
    for (int i=0;i<posiciones.size();i++) {
        newposiciones.push_back(posiciones[i]);
    }
    if (c[0]== 1){
        cout<<"c[0]=1, working"<<endl;
        for (auto& j: attr_set[att]) {
            rank_support_v<1> rank(&bitrees[j][posiciones[j].first]);
            newposiciones[j].first= posiciones[j].first + 1;
            newposiciones[j].second= 2*(rank(posiciones[j].second+1)-1);
        }
        c[0] = intersect(bitvectors,bitrees,newposiciones,attr_set,(att+1)%attr_set.size(),p+1,h);
    }
    if (c[1]==1) {
        cout<<"C[1]=1, working"<<endl;
        for (auto& j: attr_set[att]) {
            cout<<"rank"<<endl;
            rank_support_v<1> rank(&bitrees[j][posiciones[j].first]);
            cout<<"calculating first coord"<<endl;
            newposiciones[j].first= posiciones[j].first + 1;
            cout<<"calculating second coord"<<endl;
            cout<<bitrees[j][posiciones[j].first].size()<<endl;
            cout<<posiciones[j].second+1<<endl;
            newposiciones[j].second= 2*(rank(posiciones[j].second+2)-1);

        }
        c[1] = intersect(bitvectors,bitrees,newposiciones,attr_set,(att+1)%attr_set.size(),p+1,h);
    }
    if (c[0]==0 and c[1]==0) {
        cout<<"c=00, returning"<<endl;
        return 0;
    }

    cout<<"Updating bitvectors"<<endl;
    ((*bitvectors)[p]).resize((*bitvectors)[p].size()+2);
    cout<<"changing first"<<endl;
    (*bitvectors)[p][(*bitvectors)[p].size()-2]=c[0];
    cout<<"changing second"<<endl;
    (*bitvectors)[p][(*bitvectors)[p].size()-1]=c[1];

    return 1;


}

vector<bit_vector> join(vector<vector<bit_vector>> bitrees,vector<vector<int>> attr, int h, int d) {
    cout<<"joining"<<endl;
    vector<bit_vector> bitvectors(h);
    vector<vector<int>> attr_set(d);
    for (int j=0;j<attr.size();j++) {
        for (int k=0;k<attr[j].size();k++) {
            attr_set[attr[j][k]].push_back(j);
        }
    }
    cout<<"attr_set"<<endl;
    for (int i=0;i< attr_set.size();i++ ) {
        cout<<attr_set[i]<<endl;
    }
    vector<pair<int,int>> posiciones(bitrees.size());
    for (int i=0; i<bitrees.size();i++) {
        posiciones[i]={0,0};
    }
    intersect(&bitvectors,bitrees, posiciones,attr_set,0,0,h);
    for (int i=0;i<bitvectors.size();i++) {
        cout << bitvectors[i] << endl;
    }
    return bitvectors;
}




