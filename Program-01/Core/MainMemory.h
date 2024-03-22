/**
 * @brief A class whose purpose is to mimick main memory
 *
 * This class mimicks main memory.
 */

#ifndef MAINMEMORY_h
#define MAINMEMORY_h

#include "HardDrive.h"
#include <algorithm>
#include <fstream>

class MainMemory {
private:
    int* mainMemory;
    //Main Memory Size
    int M;
    //Number of records per block
    int B;
    
    int currentSize;
    
    int* memoryPointerArray[4];

public:
    MainMemory(int, int);
    ~MainMemory();
    int readHD2Mem(HardDrive&, int, int&);
    void writeToHD(HardDrive&, int&);
    void print(std::ofstream&);
    void sortMM();
    int size();
    void addToMemory(HardDrive&, int);
    int chooseSmallestInMem(int&);
    int& operator[](int index);
    void push(int);
};


/**
 * @brief Construct a new Main Memory:: Main Memory object
 * @param    {int} The size of main memory
 * @param    {int} The number of records per block
 * @date 2024-02-19
 */
MainMemory::MainMemory(int size, int recordsPerBlock){
    mainMemory = new int[size];
    M = size;
    B = recordsPerBlock;

    for (int i = 0; i < M; ++i) {
        mainMemory[i] = 0;  // Initialize mainMemory elements to 0
    }

    for(int i = 0; i < 4; i++){
        memoryPointerArray[i] = &mainMemory[i * B];
    }
}


/**
 * @brief Destroy the Main Memory:: Main Memory object
 * @date 2024-02-19
 */
MainMemory::~MainMemory() {
    delete[] mainMemory; 
}


/**
 * @brief Reads from the hard drive to main memory
 * @param    {HardDrive}   The hard drive to read from
 * @param    {int} The starting index to start reading from
 * @param    {int&} The buffersize
 * @return int 
 * @date 2024-02-19
 */
int MainMemory::readHD2Mem(HardDrive& hd, int startingIndex, int& buffsize){
    int updateIndex = 0;
    for(int i = startingIndex, j = 0; i <= hd.initialSize() && j < M; i++, j++){
        mainMemory[j] = hd[i];
        updateIndex++;
    }
    
    //Update the current size of Main Memory
    currentSize = updateIndex;

    //Update how much was read into Main Memory
    buffsize = updateIndex;

    //Update the next startingIndex
    return updateIndex;
}


/**
 * @brief Print function
 * @param    {std::ofstream}  The output file to print to
 * @date 2024-02-19
 */
void MainMemory::print(std::ofstream& outputFile){
    for(int i = 0; i < currentSize; i++){
        outputFile << mainMemory[i] << " ";
    }
}


/**
 * @brief The call to sort main memory
 * @date 2024-02-19
 */
void MainMemory::sortMM(){
    std::sort(mainMemory, mainMemory + currentSize);
}


/**
 * @brief Function to write what is in main memory to the Hard Drive
 * @param    {HardDrive}   The hard drive to write to
 * @param    {int&}  The buffsize
 * @date 2024-02-19
 */
void MainMemory::writeToHD(HardDrive& hd, int& buffsize){
    int hdSize = hd[0] + 1;
    hd.updateSize(buffsize);
    for(int i = 0; i < buffsize; i++){
        hd[hdSize + i] = mainMemory[i];
    }
    currentSize = 0;
    buffsize = 0;
}

/**
 * @brief [] Operator overload
 * @param    {int}  The index by which to access
 * @return int& 
 * @date 2024-02-19
 */
int& MainMemory::operator[](int index) {
    return mainMemory[index];
}

/**
 * @brief The size of main memory
 * @return int 
 * @date 2024-02-19
 */
int MainMemory::size(){
    return currentSize;
}

/**
 * @brief Adds an element to the end of main memory
 * @param    {HardDrive}   The hard drive to read from
 * @param    {int}  The index to access
 * @date 2024-02-19
 */
void MainMemory::addToMemory(HardDrive& hd, int index){;
    mainMemory[currentSize] = hd[index]; 
    currentSize++;
}

/**
 * @brief Adds to main memory without needing the hard drive
 * @param    {int}   The value to write
 * @date 2024-02-19
 */
void MainMemory::push(int value){
    mainMemory[currentSize++] = value;

}

/**
 * @brief The function to choose the smallest element in main memory
 * @param    {int&} The smallest value in memory
 * @return int
 * @date 2024-02-19
 */
int MainMemory::chooseSmallestInMem(int& smallest){
    // Choose the smallest in Memory and add at end of memory
    smallest = mainMemory[0];
    int comesFromSublist = 0;


    for (int i = 1; i < 3; i++) {
        if (mainMemory[i * B] < smallest) {
            smallest = mainMemory[i * B]; // Update smallest if a smaller element is found
            comesFromSublist = i;
            memoryPointerArray[i] = &mainMemory[i * B + 1];
        }
    }

    if(comesFromSublist == 0){
        memoryPointerArray[0] = &mainMemory[0 * B + 1];
    }

    return comesFromSublist;

}

#endif