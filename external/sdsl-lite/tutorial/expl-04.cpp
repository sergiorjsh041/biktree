#include <iostream>
#include <inttypes.h>

#include <sdsl/bit_vectors.hpp>
#include "../includes/rank.hpp"

using namespace std;
using namespace sdsl;

int main() {
    bit_vector b = {1, 1, 0, 1, 0, 0, 1};
    rank_bv_64 vec = rank_bv_64(b);
}
