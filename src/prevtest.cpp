#include <cstdint>

#include "KDTree.h"


#include <vector>





int main() {

    vector<int> attr_set = {0,1};
    vector<int> attr_set2 = {1,2};


    KDTree tree({{2,2},{3,4},{4,4}}, 4, 2,attr_set );
    KDTree tree2({{1,2},{3,3},{4,4}}, 4, 2, attr_set2);

    cout<<"S: "<<tree.S<<endl;
    cout<<"attr: "<<tree.attr_set<<endl;

    tree.build_tree();
    tree2.build_tree();

    join({tree2.bitvector,tree.bitvector},{{1,2},{0,1}}, 6,3);

    return 0;
}



