#include "Master.h"
#include "System.h" 

int main()
{
    
    // some variables to keep track of status could be useful
    //     for example, the current number of chunk at every level
    
    // Create an opening to the input file
    System system("prog02Data.txt");

    // Practice reading in a line and creating a record
    for(int i = 0; i < 22; i++)
    {
        system.readRecordIntoMemory();
    }
    system.outputMemory();
    /*
    while(!)
    {
        //1. add to memory in a sorted order
        //      if repeated, replace the old one. 
        //      To make it simple, just do as insertion sort does

        //2. if full, flush Mem to level 1 as a new chunk, let i=1

        //3.    if level i reaches the max number of chunks, 
        //          mergesort all chunks to the next level (i+1)

        //4.    i++; and go to step 3
    }
    
    // flush all the data in Mem to L0.txt file

    // keep all the information of the number of chunks at each level in a single file
    */

}
