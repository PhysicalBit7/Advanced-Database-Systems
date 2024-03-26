#include "Master.h"
#include "System.h" 

int main()
{
    
    // some variables to keep track of status could be useful
    //     for example, the current number of chunk at every level
    
    // Create an opening to the input file
    System system("startingData.txt");

    // Practice reading in a line and creating a record
    for(int i = 0; i < 62; i++)
    {
        system.readRecordIntoMemory();
    }


}
