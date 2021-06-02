//
// Created by anouk on 24-04-21.
//

#include <fstream>
#include<ratio>
#include<chrono>
#include<ctime>

using namespace std::chrono;


#include "joins.cpp"

#define AT_X 0
#define AT_Y 1

std::vector<std::vector<uint64_t>>* read_relation(const std::string filename, uint16_t n_Atts)
{
    std::ifstream input_stream(filename);
    uint64_t x;
    uint16_t i, j=0;

    std::vector<std::vector<uint64_t>>* relation;
    std::vector<uint64_t> tuple;

    relation = new std::vector<std::vector<uint64_t>>();

    input_stream >> x;
    while (!input_stream.eof()) {
        tuple.clear();
        for (i = 0; i < n_Atts; i++) {
            tuple.push_back(x);
            input_stream >> x;
        }

        relation->push_back(tuple);
    }

    return relation;
}
int main(int argc, char** argv)
{
    qdag::att_set att_R;
    att_R.push_back(AT_Y); att_R.push_back(AT_X);
    std::string strRel_R(argv[1]);
    std::vector<std::vector<uint64_t>>* rel_R = read_relation(strRel_R, att_R.size());
    uint64_t grid_side = 36;


    se_quadtree* Q;
    Q = new se_quadtree(*rel_R, grid_side, 2, att_R.size());
    //printf("holi %" PRIu8 "\n", Q->bv[3].get_4_bits(8));


    std::ostream &output_stream = cout;
    Q->print(output_stream);
    return 0; //0101 0110 0010 0010 1000

}
