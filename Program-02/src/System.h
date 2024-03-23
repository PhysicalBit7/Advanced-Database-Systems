#ifndef SYSTEM_H
#define SYSTEM_H

#include "Master.h"

class System {
    // some functions might be useful such as read records to memory:
    private:
        ifstream inputFile;
        int currentMemorySize;
        int chunksAtCurrentLevel;
        int currentLevel;
        vector<Level> levelTracker;
        
    public:
        // memory
        RecordStruct Mem[MEMSIZE];
        // Create a record from the input file
        RecordStruct createARecord(int, int);
        // Read records from input file to memory
        void readRecordIntoMemory();
        // Output Memory
        void outputMemory(std::ostream& = std::cout);
        // Flush fuction
        void flushMemory();
        // Sort Memory
        void insertionSortOnMemory(RecordStruct);
        // Delete if repeated
        bool checkForDuplicatesAndReplace(RecordStruct);

        // Constructor
        System(string);
        //Destructor
        ~System();
};

System::System(string file) {
    currentMemorySize = 0;
    currentLevel = 1;
    chunksAtCurrentLevel = 0;
    Level level = {currentLevel, chunksAtCurrentLevel};
    levelTracker.push_back(level);

    file = "../Data/" + file;
    inputFile = ifstream(file);
    cout << "Opening file: " << file << endl;

    // Check if the file was opened successfully
    if (!inputFile) {
        std::cerr << "Unable to open file";
        exit(1); // terminate with error
    }
    cout << "File opened successfully" << endl;
}

System::~System() {
    cout << "Closing file" << endl;
    inputFile.close();
}

RecordStruct System::createARecord(int key, int value) {
    RecordStruct record;
    record.key = key;
    record.value = value;
    return record;
}

void System::readRecordIntoMemory() {
    //First we need to read in a line from the file
    //If memory is full we need to check if there are still records to read, sort, and flush
    if(currentMemorySize >= MEMSIZE) {
        std::cerr << "Memory is full" << endl;
        if(!inputFile.eof()){
            std::cerr << "Memory is full but there are still records to read" << endl;
            //TODO: at this point I beleive we need to flush the memory to the next level
            flushMemory();
        }
    }else{
        int keyFromFile;
        int valueFromFile;
        inputFile >> keyFromFile >> valueFromFile;
        RecordStruct record = createARecord(keyFromFile, valueFromFile);
        // input record and call insertion sort
        insertionSortOnMemory(record);   
    }
}

void System::outputMemory(std::ofstream& output) {
    output << " ---------- Outputting Memory: Size = " << currentMemorySize << " ----------" << endl;
    for (int i = 0; i < currentMemorySize; i++) {
        output << "Record: " << i << " Key:" << Mem[i].key << " Value:" << Mem[i].value << endl;
    }
    //currentMemorySize = 0;
}

void System::flushMemory(){
    if(chunksAtCurrentLevel == THRESHOLD){
        // TODO: Merge sort all chunks to the next level down
        chunksAtCurrentLevel = 0;
        Level newLevel = {currentLevel++, chunksAtCurrentLevel};
    }else{
        levelTracker[currentLevel].numberOfChunks++;
    }
    // Create appropriate file name under the level
    std::ostringstream filenameStream;
    filenameStream << "L" << currentLevel << "-" << chunksAtCurrentLevel <<".txt";
}

void System::insertionSortOnMemory(RecordStruct record){
    if(currentMemorySize == 0){
        //cout << "Inserting first record: " << record.key << " with value: " << record.value << endl;
        Mem[0] = record;
        currentMemorySize++;
    }else{
        //cout << "Inserting: " << record.key << " with value: " << record.value << endl;
        int j = currentMemorySize - 1;
        if(checkForDuplicatesAndReplace(record)){
            return;
        }
        while (j >= 0 && Mem[j].key > record.key) {
            Mem[j + 1] = Mem[j];
            j--;
        }
        Mem[j + 1] = record;
        currentMemorySize++;
    }
}

bool System::checkForDuplicatesAndReplace(RecordStruct record){
    for(int i = 0; i < currentMemorySize; i++){
        if(Mem[i].key == record.key){
            //cout << "------- Duplicate detected: " << Mem[i].key << " replacing value" << endl;
            Mem[i].value = record.value;
            return true;
        }
    }
    return false;
}





#endif