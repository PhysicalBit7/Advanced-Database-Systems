/**
 * @brief A class that creates a sublist for a sorted memory array
 *
 */

#ifndef SUBLIST_H
#define SUBLIST_H

// Need to do at memory address starting point
// HDNext is the next two elements to be added to main memory
// Memcurr is an offset in order to find the current element
// Create another variable to identify if a block of size 1 is loaded into memory memValid = 1
// There should be another variable identifying if the sublist is complete

class Sublist
{
private:
    int size;
    int beginningIndex;
    int currentMemory;    // offset of current index in the memory,  0<= memCurr <=memSize-1
    int memoryValid;   // number of records in the memory when brought in.  
                    //     Normally, it is same as memSize, but final block might be smaller
    int HDnext;     // offset of the next data to bring to mem, 0 <= HDcurr <=size-1 
    bool completed = false;

    

public:
    Sublist(int, int);

    // Phase 2
    // check if there is still records to bring in memory
    bool isComplete();
    void updateCurrent(int);

    int currentData();       // the current first one in this sublist to compare for the smallest value
                                //    return the value of HD[beginPtr+memCurr];
    int outputRecord();           // return one record (to go to output buffer) and move ptr to next record
                                    //          Then, if no record in memory, load one more block and reset memCurr

    int returnSize();
    int returnBeginning();
    void readElements(int);
    int checkHowManyCanRead();
    
};

/**
 * @brief Construct a new Sublist:: Sublist object
 * @param    {int}  The size of the sublist
 * @param    {int}   The beginning index of the sublist
 * @date 2024-02-19
 */
Sublist::Sublist(int size, int beginningIndex){
    this->size = size;
    this->beginningIndex = beginningIndex;
    currentMemory = beginningIndex;
}

/**
 * @brief Returns the size of the sublist
 * @return int 
 * @date 2024-02-19
 */
int Sublist::returnSize(){
    return size;
}

/**
 * @brief Returns the beginngin index of the sublist
 * @return int 
 * @date 2024-02-19
 */
int Sublist::returnBeginning(){
    return beginningIndex;
}

/**
 * @brief Returns the current hard drive index
 * @return int 
 * @date 2024-02-19
 */
int Sublist::currentData(){
    return currentMemory;
}


/**
 * @brief Updates the current memory variable to point to the next element
 * @param    {a}                 My Param doc
 * @date 2024-02-19
 */
void Sublist::updateCurrent(int increment){
    currentMemory += increment;
    if(currentMemory > (currentMemory + size)){
        completed = true;
    }
}

/**
 * @brief The sublist is complete
 * @return true 
 * @return false 
 * @date 2024-02-19
 */
bool Sublist::isComplete(){
    return completed;
}

/**
 * @brief Not complete
 * @return int 
 * @date 2024-02-19
 */
int Sublist::checkHowManyCanRead(){
    if(completed){
        return 0;
    }
    if(currentMemory < beginningIndex + 8){
        return 2;
    }else{
        return 1;
    }
}


#endif
