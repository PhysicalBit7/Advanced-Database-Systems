#ifndef SYSTEM_H
#define SYSTEM_H

#include "Master.h"

#include <fstream>
#include <vector>
#include <iostream>



class System {
private:
    ifstream startingDataFile;  // Input file stream for starting data
    int currentMemorySize;  // Current size of memory
    int numberOfLevels;  // Number of levels in the system
    int level1Flush;  // Current level 1 flush status
    int chunksAtLevel;  // Number of chunks at each level
    vector<Level> levelTracker;  // Vector to track levels

public:
    // Memory size
    RecordStruct Mem[MEMSIZE];

    // Constructor
    System(string filePath);

    // Destructor
    ~System();

    /**
     * Creates a record from the input file.
     *
     * @param property1 The value of property 1.
     * @param property2 The value of property 2.
     * @return The created record.
     */
    RecordStruct createARecord(int property1, int property2);

    /**
     * Reads records from the input file into memory.
     */
    void readRecordIntoMemory();

    /**
     * Outputs the contents of the memory.
     *
     * @param os The output stream to write the memory contents to. Default is std::cout.
     */
    void outputMemory(std::ostream& os = std::cout);

    /**
     * Flushes the memory to Level 1.
     */
    void flushMemory();

    /**
     * Merges the memory to the next level without any merging to test.
     *
     * @param level The level to merge.
     * @param chunks The number of chunks to merge.
     */
    void mergeToNextLevel(int level, int chunks);

    /**
     * Chooses the smallest record in memory.
     */
    void chooseSmallestInMem();

    /**
     * Fills the memory using merge sort.
     *
     * @param level The level to fill.
     * @param outputFile The output file stream to write the sorted records to.
     * @param chunks The number of chunks to merge.
     */
    void mergeSort(int level, ofstream& outputFile, int chunks);

    /**
     * Reads records into a specific memory location.
     *
     * @param location The memory location to read into.
     */
    void readInAtSpecificMemory(int location);

    /**
     * Sorts the memory using insertion sort.
     *
     * @param record The record to insert into memory.
     */
    void insertionSortOnMemory(RecordStruct record);

    /**
     * Checks for duplicates in memory and replaces them.
     *
     * @param record The record to check for duplicates.
     * @return True if a duplicate was found and replaced, false otherwise.
     */
    bool checkForDuplicatesAndReplace(RecordStruct record);
};

System::System(string file)
{
    currentMemorySize = 0;
    level1Flush = 0;
    numberOfLevels = 0;
    chunksAtLevel = 0;
    Level level = {level1Flush, chunksAtLevel};
    levelTracker.push_back(level);

    file = "../Data/" + file;
    startingDataFile = ifstream(file);
    cout << "Opening file: " << file << endl;

    // Check if the file was opened successfully
    if (!startingDataFile)
    {
        std::cerr << "Unable to open file";
        exit(1); // terminate with error
    }
    cout << "File opened successfully" << endl;
}

System::~System()
{
    cout << "Closing file" << endl;
    startingDataFile.close();
}

RecordStruct System::createARecord(int key, int value)
{
    RecordStruct record;
    record.key = key;
    record.value = value;
    return record;
}

void System::readRecordIntoMemory()
{
    // First we need to read in a line from the file
    // If memory is full we need to check if there are still records to read, sort, and flush
    if (currentMemorySize == MEMSIZE)
    {
        // std::cerr << "Memory is full" << endl;
        if (!startingDataFile.eof())
        {
            // std::cerr << "Memory is full but there are still records to read" << endl;
            // TODO: at this point I beleive we need to flush the memory to the next level
            flushMemory();
        }
    }
    // Still need to add the current record into memory
    int keyFromFile;
    int valueFromFile;
    startingDataFile >> keyFromFile >> valueFromFile;
    RecordStruct record = createARecord(keyFromFile, valueFromFile);
    // input record and call insertion sort
    insertionSortOnMemory(record);
}

void System::outputMemory(std::ostream &output)
{
    //output << " ---------- Outputting Memory: Size = " << currentMemorySize << " ----------" << endl;
    for (int i = 0; i < currentMemorySize; i++)
    {
        output << Mem[i].key << "  " << Mem[i].value << endl;
    }
}

void System::flushMemory(){
    //Here we are only worrying about flushing to level 1

    // Create appropriate file name under the level
    std::ostringstream filenameStream;
    filenameStream << "L" << levelTracker[level1Flush].level + 1 << "-" << levelTracker[level1Flush].numberOfChunks << ".txt";
    string filename = filenameStream.str();

    // Actually output file
    ofstream outputFile("../Data/" + filename);
    outputMemory(outputFile);
    outputFile.close();

    // Update level tracker
    chunksAtLevel++;
    levelTracker[level1Flush].numberOfChunks = chunksAtLevel;

    // Reset memory
    currentMemorySize = 0;

    if(chunksAtLevel == THRESHOLD){
        mergeToNextLevel(level1Flush, levelTracker[level1Flush].numberOfChunks);
        levelTracker[level1Flush].numberOfChunks = 0;
        //TODO: swap to merge to next level with merging 
    }

}

void System::insertionSortOnMemory(RecordStruct record)
{
    if (currentMemorySize == 0)
    {
        // cout << "Inserting first record: " << record.key << " with value: " << record.value << endl;
        Mem[0] = record;
        currentMemorySize++;
    }
    else
    {
        // cout << "Inserting: " << record.key << " with value: " << record.value << endl;
        int j = currentMemorySize - 1;
        if (checkForDuplicatesAndReplace(record))
        {
            return;
        }
        while (j >= 0 && Mem[j].key > record.key)
        {
            Mem[j + 1] = Mem[j];
            j--;
        }
        Mem[j + 1] = record;
        currentMemorySize++;
    }
}

bool System::checkForDuplicatesAndReplace(RecordStruct record)
{
    for (int i = 0; i < currentMemorySize; i++)
    {
        if (Mem[i].key == record.key)
        {
            // cout << "------- Duplicate detected: " << Mem[i].key << " replacing value" << endl;
            Mem[i].value = record.value;
            return true;
        }
    }
    return false;
}


void System::mergeToNextLevel(int currentLevel, int numberOfChunks){
    // To account for us starting at 0
    currentLevel++;

    cout << "Merging to next level. Currently at " << currentLevel << endl;

    //if there does not exist a level above this create it
    if(levelTracker.size() < currentLevel + 1){
        numberOfLevels++;
        Level level = {numberOfLevels, 0};
        levelTracker.push_back(level);
        //cout << "Level Tracker Size = " << levelTracker.size() << endl;
    // else there does exist a level above this
    }

    // Create appropriate file name under the level
    std::ostringstream filenameStream;
    filenameStream << "L" << levelTracker[currentLevel].level + 1 << "-" << levelTracker[currentLevel].numberOfChunks << ".txt";
    cout << currentLevel << endl;
    string filename = filenameStream.str();
    ofstream outputFile("../Data/" + filename);

    // Merge sort the records
    mergeSort(currentLevel, outputFile, numberOfChunks);
    // Close the file
    outputFile.close();
    // TODO: Need to make this recursive in order to cascade down the current levels to make sure they dont also need to merge
}

void System::readInAtSpecificMemory(int starting){

}

void System::mergeSort(int numberOfLevels, ofstream & outputFile, int numberOfChunks){
    std::ifstream* files[numberOfChunks];
    for(int i = 0; i < 4; i++){
        std::ostringstream filenameStream;
        filenameStream << "L" << numberOfLevels << "-" << i << ".txt";
        string filename = filenameStream.str();
        files[i] = new std::ifstream("../Data/" + filename);
        if (!*files[i]) {
            std::cerr << "Unable to open file " << filename;
            exit(1); // terminate with error
        }
    }

    int fileNum = 0;
    int flag = 0;
    while(flag != THRESHOLD){
        for(int i = 0; i < BLOCKSIZE; i++){
            string checkForEOF;
            std::getline(*files[fileNum], checkForEOF);
            std::istringstream iss(checkForEOF);
            if(!checkForEOF.empty()){
                int keyFromFile;
                int valueFromFile;
                iss >> keyFromFile >> valueFromFile;
                //cout << "Adding Key: " << keyFromFile << " Value: " << valueFromFile << " from file: " << fileNum << endl;
                RecordStruct record = createARecord(keyFromFile, valueFromFile);
                Mem[currentMemorySize++] = record;
            }else{
                flag++;
                fileNum++;
                if(flag == THRESHOLD){
                    break;
                }
            }
        }
        fileNum++;
        // TODO: need to check if memory is full. Reset filenum back to 0
        if(currentMemorySize == MEMSIZE - BLOCKSIZE){
            //cout << "______________Memory is full_______________ : " << currentMemorySize << endl;
            outputMemory(outputFile);
            fileNum = 0;
            currentMemorySize = 0;
        }
    }
}


#endif