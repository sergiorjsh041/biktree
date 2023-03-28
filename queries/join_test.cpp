
#include <fstream>
#include<bits/stdc++.h>
#include<ratio>

using namespace std::chrono;


#include "../src/joins.cpp"

high_resolution_clock::time_point start_select, stop_select;
double total_time_select = 0.0;
duration<double> time_span_select;

#define AT_X1 0
#define AT_X2 1
#define AT_X3 2
#define AT_X4 3


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


uint64_t maximum_in_table(std::vector<std::vector<uint64_t>> &table, uint16_t n_columns, uint64_t max_temp)
{
    uint64_t i, j;

    for (i = 0; i < table.size(); i++)
        for (j = 0; j < n_columns; j++)
            if (table[i][j] > max_temp)
                max_temp = table[i][j];


    return max_temp;
}



int main(int argc, char** argv)
{
    qdag::att_set att_R;
    qdag::att_set att_S;
    qdag::att_set att_T;

    att_R.push_back(AT_X1); att_R.push_back(AT_X2);
    att_S.push_back(AT_X2); att_S.push_back(AT_X3);
    att_T.push_back(AT_X3); att_T.push_back(AT_X4);

    std::string strRel_R(argv[1]), strRel_S(argv[2]), strRel_T(argv[3]);
    std::vector<std::vector<uint64_t>>* rel_R = read_relation(strRel_R, att_R.size());
    std::vector<std::vector<uint64_t>>* rel_S = read_relation(strRel_S, att_S.size());
    std::vector<std::vector<uint64_t>>* rel_T = read_relation(strRel_T, att_T.size());

    uint64_t grid_side = 32;

    grid_side = maximum_in_table(*rel_R, att_R.size(), grid_side);
    grid_side = maximum_in_table(*rel_S, att_S.size(), grid_side);
    grid_side = maximum_in_table(*rel_T, att_T.size(), grid_side);

    grid_side++;

    //cout << "Grid side: " << grid_side << endl;

    qdag qdag_rel_R(*rel_R, att_R, grid_side, 2, att_R.size());
    qdag qdag_rel_S(*rel_S, att_S, grid_side, 2, att_S.size());
    qdag qdag_rel_T(*rel_T, att_T, grid_side, 2, att_T.size());

    std::ostream &output_stream = cout;
    cout << "izquierda:\n";
    qdag_rel_R.print(output_stream);
    cout << "\nderecha:\n";
    qdag_rel_S.print(output_stream);
    //qdag_rel_T.print(output_stream);

//1011011 01100010 00000100 01100110
//0011011 11000001 01000100 01100110
//0001001 11100101 01110100 01100110
    vector<qdag> Q(2);//(3);

    Q[0] = qdag_rel_R;
    Q[1] = qdag_rel_S;
    //Q[2] = qdag_rel_T;

    qdag *Join_Result;
    Join_Result = multiJoin(Q, false, 1000);
    cout << "\n result:\n";
    Join_Result->print(output_stream);
    semiJoin(Q, false, 1000);
    cout << "\n active:\n";
    Q[0].print_active(output_stream);
    auto a = Q[0].Q->active[5];
    rank_bv_64 * pl = &a;
    pl->print_4_bits(0);

    //cout << endl;
    return 0;
}
