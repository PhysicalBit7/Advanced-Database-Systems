// ***** Master.h *****

#ifndef MASTER_H

#define MASTER_H

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::flush;
using std::ostream;

#include <iomanip>
using std::ios;
using std::setiosflags;
using std::setw;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <string>
using std::string;

#include <stdexcept> // for std::runtime_error
using std::runtime_error;

#include <cmath>        // for ceil()
#include <algorithm>    // for sort
//#include <list>       // for list
//using std::list;
#include <vector>
using std::vector;

#include <limits>       // for std::numeric_limits<int>::max();

//system:

const int BLOCKSIZE = 2;        //records per block
const int THRESHOLD = 4;        //size ratio between levels
const int MEMSIZE = (THRESHOLD+1) * BLOCKSIZE;      //10      //number of records in memory

struct RecStruct{
    int key;
    int value; 
    // int status;     // 0: for normal, 1 for delete tombstone
};

#endif