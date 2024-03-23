#ifndef MASTER_H
#define MASTER_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <vector>
#include <limits>

using std::cin;
using std::cout;
using std::endl;
using std::flush;
using std::ostream;
using std::ios;
using std::setiosflags;
using std::setw;
using std::ifstream;
using std::ofstream;
using std::string;
using std::runtime_error;
using std::ceil;
using std::sort;
using std::vector;
using std::numeric_limits;

const int BLOCKSIZE = 2;                            // records per block
const int THRESHOLD = 4;                            // size ratio between levels
const int MEMSIZE = (THRESHOLD + 1) * BLOCKSIZE;    // number of records in memory

struct RecStruct {
    int key;
    int value;
    // int status;     // 0: for normal, 1 for delete tombstone
};

#endif