
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
void readHD2Mem(int HDindex, int toMemIndex, int buffsize);     // read data (# of records) to mem from HD
{
    for (int i=0; i<fuffsize; i++)
    {
        Mem[toMemIndex+i] = HD[HDindex+i];
    }
}
void writeMem2HD(int memIndx, int toHDindex, int buffsize)     // write data from mem to HD by size (records)
{
    for (int i=0; i<buffsize; i++)
    {
        HD[toHDIndex+i] = HD[memIndex+i];
    }    
}


// Phase 1
int nSublists=ceil(dataSize/MEMSIZE);      // number of sublists
// choose array or linked list to manage all the sublists
// Sublist *sublistArray[100];
// Sublist *sublistArray[nSublists];
// Sublist **sublistArray = new *Sublist[nSublists+1];
// std::list<*Sublist>; 


{
    HDnext2Write = 201;     // some location in HD
    for ( ... )  // to create for each sublist
    {
        int bufsize;  // the number of records to load to memory
        if (HDStarting+dataSize < HDnext2Read+MEMSIZE)  // if < MEMSIZE left
            bufsize = HDStarting+dataSize - HDnext2Read;   // last sublist
        else
            bufsize = MEMSIZE;

        //1. loaded a chunk
        readHD2Mem(HDnext2Read, 0, bufsize); 
        HDnext2Read += bufsize;   // if dataSize > MEMSIZE
        //2. sort
        sort(Mem);  // any sorting function in c++ or your implementation
        //3. write back to HD
        writeMem2HD(0, HDnext2Write, bufsize); 


        Sublist *sl = new subList(HDnext2Write /*startingIndex*/, 
                                bufsize /*sublistSizee*/);

        // need to put s1 to array or linked list

        HDnext2Write += bufsiz;
    }
}




// Phase 2
int outputBufferAddr;       // memory address (index) for the output buffer
int outputBufferSize;       // size of the output buffer reserved

// when working in phase 2, need to keep track of the next slot in the output buffer to put data
int outputBufferNextPtr;    // index for next smallest record to store
                            //    if full, need to output to HD
{
    // first block
    for (int i=0; i<nSublists; i++)
    {
        sublistArray[i].beginPtr = 0+ i*BLOCKSIZE;
        sublistArray[i].loadNextBlock();
    }

    for (...)
    {
        // compare the current value of each sublist
        int min = infinity; 
        for (int i=0; i<nSublists; i++)
        {
            if (sublistArray[i].notCompleted && sublistArray[i].currData() < min)
            {
                min = sublistArray[i].currData();
                minSublist = i; 
            }
        }
        // after min found
        sublistArray[minSublist].output1rec();
    }
}


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
    { return HDnext < startingpoint + size}; 

    int currData() const;       // the currently first one in this sublist to compare for the smallest value
                                //    return the value of HD[beginPtr+memCurr];
    int output1rec();           // return one record (to go to output buffer) and move ptr to next record
                                //          Then, if no record in memory, load one more block and reset memCurr
    {
        // copy to output buffer
        // move output buffer ptr
        //      if full
        //          write to HD
        //          output fuffer ptr = initial one
        // memCurr ++;
        //      if go beyond the boundary
        //          memCurr = intial one
        //          loadNextBlock();
        //          if (completed?)
    }
    loadNextBlock() 
    {
        for (int i=0; i<BLOCKSIZE; i++)
            Mem[memBegin+i] = HD[HDnext++]; 

    }
};


