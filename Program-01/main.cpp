/**
 * @brief Homework 1 
 * @class Advanced Database Systems
 *
 * Detailed description of the function and its parameters.
 *
 * @author Tanner Patrom
 * @date 2/19/2024
 */
#include "Core/HardDrive.h"
#include "Core/MainMemory.h"
#include "Core/Sublist.h"
//#include "Core/MasterList.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <list>

const int HD_SIZE = 8192;
//Main Memory Size
const int M = 8;
//Number of records per block
const int B = 2;
void fillFromSublistLL(std::list<Sublist>&, HardDrive&);


int main(){

    // Output file for analysis
    std::ofstream outputFile("checkOutput.txt");

    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }
    
    //Phase 1
    // 1. Initialize the Hard Drive
    HardDrive HD(HD_SIZE);
    HD.initializeHD("data.txt");
    outputFile << "BEGINNING PHASE 1\n";
    outputFile << "Initial Hard Drive Data: ";
    HD.print(outputFile);
    outputFile << "Hard Drive Initial Size: " << HD.initialSize() << std::endl;
    outputFile << std::endl;

    // More initialization
    // data size, number of records
    int numRecords = HD.initialSize();
    // starting address in HD, it is 1 in our assignment
    int whereToStart = 1;
    //Number of Sublists
    int numberOfSublists = ceil(static_cast<double>(numRecords)/static_cast<double>(M));
    //
    int howMuchWasWrote = 0;
    // For Sublist data
    int copyForSublistStart;
    int copyForSublistSize;


    // Initialize Main Memory
    MainMemory MM(M, B);

    //Initialize Master List of Sublists
    std::vector<Sublist> masterList;

    //2. Pass through creating sublists and 
    for(int i = 0; i < numberOfSublists; i++){
        // Pass to copy records and sort
        outputFile << "----------" << std::endl;
        outputFile << "Pass " << i << std::endl;
        outputFile << "Starting at index " << whereToStart << " of Hard Drive\n";
        
        whereToStart += MM.readHD2Mem(HD, whereToStart, howMuchWasWrote);
        outputFile << "Main Memory read " << howMuchWasWrote << " records from Hard Drive: ";
        MM.print(outputFile);
        outputFile << std::endl;
        MM.sortMM();
        outputFile << "After sorting main memory: ";
        MM.print(outputFile);
        outputFile << "\n\n\n";

        //Some sublist data
        copyForSublistStart = HD.currentSize();
        copyForSublistSize = howMuchWasWrote;



        // Write first chunk to HD at end of array and zero out memory
        MM.writeToHD(HD, howMuchWasWrote);
        outputFile << "Updated Hard Drive Data: ";
        HD.print(outputFile);
        outputFile << "New Hard Drive size: " << HD.currentSize() << std::endl;
        outputFile << "Memory reset to size: " << MM.size() << std::endl;
        outputFile << "Buffer size also reset to: " << howMuchWasWrote << std::endl;
        outputFile << "\n";

        // Create the sublist and add to SublistLinkedList as well as print
        Sublist SL(copyForSublistSize, copyForSublistStart + 1);
        masterList.push_back(SL);
        outputFile << "Sublist Beginning Index: " << SL.returnBeginning() << "\nSublist size: " << SL.returnSize() << std::endl;
        outputFile << "Sublist data: ";
        HD.printFrom(SL.returnBeginning(), (SL.returnSize() + SL.returnBeginning()), outputFile);
        outputFile << "\n\n\n";

    }



    //Phase 2
    outputFile << "-----\n";
    outputFile << "BEGINNING PHASE 2\n";
    outputFile << "Main Memory Size: " << MM.size() << "\n";
    MM.print(outputFile);
    outputFile << "\n";


    outputFile << "-----\n";
    outputFile << "First Round of adding to Main Memory\n";

    //Fill memory with first two elements from sublists to have 6 elements
    for(int i = 0; i < 3; i++){
        // Select sublist
        Sublist current = masterList[i];
        outputFile << "Sublist: " << i << std::endl;
        //Get list current data index
        int startingIndex = current.currentData();
        outputFile << "Starting Index: " << startingIndex << std::endl;
        //From that index try and add two records into main memory
        for(int i = 0; i < B; i++){
            if(!current.isComplete()){
                MM.addToMemory(HD, startingIndex + i);
                current.updateCurrent(1);
            }
            
        }
        outputFile << "Updated Index: " << current.currentData() << std::endl;
        outputFile << "\n\n";
        
    }


    int smallest;
    int comesFromBlock;
    outputFile << "Main Memory Size: " << MM.size() << "\n";
    MM.print(outputFile);
    outputFile << "\n";

    comesFromBlock = MM.chooseSmallestInMem(smallest);
    MM.push(smallest);

    outputFile << "Smallest: " << smallest << std::endl;
    outputFile << "Comes from block: " << comesFromBlock << std::endl;
    outputFile << "Main Memory Size: " << MM.size() << "\n";
    MM.print(outputFile);
    outputFile << "\n";

    outputFile << "Main Memory Size: " << MM.size() << "\n";
    MM.print(outputFile);
    outputFile << "\n";

    comesFromBlock = MM.chooseSmallestInMem(smallest);
    MM.push(smallest);

    outputFile << "Smallest: " << smallest << std::endl;
    outputFile << "Comes from block: " << comesFromBlock << std::endl;
    outputFile << "Main Memory Size: " << MM.size() << "\n";
    MM.print(outputFile);
    outputFile << "\n";

    // I am stuck here. I dont know how to properly increment to the next element in a block of main memory



    
    



    // Close the file
    outputFile.close();
    return 0;

}

