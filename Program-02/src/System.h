#ifndef SYSTEM_H
#define SYSTEM_H

#include "Master.h"

class System
{
    // some functions might be useful such as read records to memory:
private:
    ifstream inputFile;
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
    // Flush to next level
    void mergeToNextLevel(int);
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
    inputFile = ifstream(file);
    cout << "Opening file: " << file << endl;

    // Check if the file was opened successfully
    if (!inputFile)
    {
        std::cerr << "Unable to open file";
        exit(1); // terminate with error
    }
    cout << "File opened successfully" << endl;
}

System::~System()
{
    cout << "Closing file" << endl;
    inputFile.close();
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
        if (!inputFile.eof())
        {
            // std::cerr << "Memory is full but there are still records to read" << endl;
            // TODO: at this point I beleive we need to flush the memory to the next level
            flushMemory();
        }
    }
    // Still need to add the current record into memory
    int keyFromFile;
    int valueFromFile;
    inputFile >> keyFromFile >> valueFromFile;
    RecordStruct record = createARecord(keyFromFile, valueFromFile);
    // input record and call insertion sort
    insertionSortOnMemory(record);
}

void System::outputMemory(std::ostream &output)
{
    // output << " ---------- Outputting Memory: Size = " << currentMemorySize << " ----------" << endl;
    for (int i = 0; i < currentMemorySize; i++)
    {
        output << " Key:" << Mem[i].key << " Value:" << Mem[i].value << endl;
    }
    // currentMemorySize = 0;
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

    if(chunksAtLevel == THRESHOLD){
        //TODO: can make this a recursive function to handle multiple levels
        mergeToNextLevel(level1Flush); 
    }
    // Reset memory
    currentMemorySize = 0;

    // Check levels
    for (int i = 0; i < levelTracker.size(); i++)
    {
        cout << "Level: " << levelTracker[i].level + 1 << " Chunks: " << levelTracker[i].numberOfChunks << endl;
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


void System::mergeToNextLevel(int currentLevel)
{
    

}

#endif