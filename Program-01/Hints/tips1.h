
// you can consider implement it in a class

//system:
const int MEMSIZE = 8;      // number of records in memory
const int BLOCKSIZE = 2;    // number of records per block
const int HD_SIZE = 8192;   // max number of records in HD
int HD[HD_SIZE] = {0};      // hard disk
int Mem[MEMSIZE] = {0};     // memory

// 2PMMS
// initialization:
int dataSize;       // data size, number of records
int HDStarting;     // starting address in HD, it is 1 in our assignment

// some variables to track the addresses in HD for read and write
int HDnext2Read;    // offset where next record to pull (read).  0<= HDnext2Read < dataSize
int HDnext2Write;   // offset where to put data (write)

// 2 functions might be useful:
void readHD2Mem(int HDindex, int toMemIndex, int buffsize);     // read data (block by block) to mem from HD
void writeMem2HD(int memIndx, int toHDindex, int buffsize);     // write data from mem to HD by size (block)


// Phase 1
int nSublists;      // number of sublists
// linked list or array of sublists     // maintaining the sublists

// Phase 2
int outputBufferAddr;       // memory address (index) for the output buffer
int outputBufferSize;       // size of the output buffer reserved

// when working in phase 2, need to keep track of the next slot in the output buffer to put data
int outputBufferNextPtr;    // index for next smallest record to store
                            //    if full, need to output to HD


// each sublist:  
// if implemented by class (or by struct or by parallel arrays)
class Sublist
{
private:
    int size;           // sublist size, number of records
    int beginPtr;       // starting address when store in HD

    // variables used for phase II
    int memBegin;   // loading address in memory
    int memSize;    // memory size can be used (number of records) for this sublist
    int memCurr;    // offset of current index in the memory,  0<= memCurr <=memSize-1
    int memValid;   // number of records in the memory when brought in.  
                    //     Normally, it is same as memSize, but final block might be smaller
    int HDnext;     // offset of the next data to bring to mem, 0 <= HDcurr <=size-1 

public:

    // for phase II
    bool notCompleted() const;  // check if there is still records to bring in memory 
    int currData() const;       // the currently first one in this sublist to compare for the smallest value
                                //    return the value of HD[beginPtr+memCurr];
    int output1rec();           // return one record (to go to output buffer) and move ptr to next record
                                //          Then, if no record in memory, load one more block and reset memCurr
};


