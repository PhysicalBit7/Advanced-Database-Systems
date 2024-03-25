#ifndef SYSTEM_H
#define SYSTEM_H

#include "Master.h"

class System
{
    // some functions might be useful such as read records to memory:
private:
    ifstream startingDataFile;
    int currentMemorySize;
    //int chunks at numberOfLevels;
    int numberOfLevels;
    // Current Level
    int level1Flush;
    // Number of Chunks
    int chunksAtLevel;
    vector<Level> levelTracker;

public:
    // memory
    RecordStruct Mem[MEMSIZE];
    // Create a record from the input file
    RecordStruct createARecord(int, int);
    // Read records from input file to memory
    void readRecordIntoMemory();
    // Output Memory
    void outputMemory(std::ostream & = std::cout);
    // Flush memory to Level 1
    void flushMemory();
    // Merge to next level with no merging to test
    void mergeToNextLevel(int, int);
    // Choose the smallest in memory
    void chooseSmallestInMem();
    // Fill memory
    void mergeSort(int, ofstream &, int);
    // Read in at specific memory location
    void readInAtSpecificMemory(int);
    // Sort Memory
    void insertionSortOnMemory(RecordStruct);
    // Delete if repeated
    bool checkForDuplicatesAndReplace(RecordStruct);

    // Constructor
    System(string);
    // Destructor
    ~System();
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

void System::mergeSort(int currentLevel, ofstream & outputFile, int numberOfChunks){
    std::ifstream* files[numberOfChunks];
    for(int i = 0; i < 4; i++){
        std::ostringstream filenameStream;
        filenameStream << "L" << currentLevel << "-" << i << ".txt";
        string filename = filenameStream.str();
        files[i] = new std::ifstream("../Data/" + filename);
        if (!*files[i]) {
            std::cerr << "Unable to open file " << filename;
            exit(1); // terminate with error
        }
    }

    int flag = 0;
    while(currentMemorySize < MEMSIZE - BLOCKSIZE && flag != numberOfChunks){
        int fileNum = 0;
        for(int i = 0; i < BLOCKSIZE; i++){
            string checkForEOF;
            std::getline(*files[fileNum], checkForEOF);
            std::istringstream iss(checkForEOF);
            if(!checkForEOF.empty()){
                int keyFromFile;
                int valueFromFile;
                iss >> keyFromFile >> valueFromFile;
                cout << "Adding Key: " << keyFromFile << " Value: " << valueFromFile << " from file: " << fileNum << endl;
                RecordStruct record = createARecord(keyFromFile, valueFromFile);
                Mem[currentMemorySize++] = record;
            }else{
                flag++;
                fileNum++;
            }
        }
        fileNum++;
        //cout << "______________Memory is full_______________" << endl;
        // TODO: need to check if memory is full. Reset filenum back to 0
        // if(currentMemorySize == 8){
        //     outputMemory(outputFile);
        //     fileNum = 0;
        //     currentMemorySize = 0;
        // }
    }
}


#endif