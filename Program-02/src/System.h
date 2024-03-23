#ifndef SYSTEM_H
#define SYSTEM_H

#include "Master.h"

class System {
    // some functions might be useful such as read records to memory:
    private:
        ifstream inputFile;
        int currentMemorySize;
    public:
        // memory
        RecordStruct Mem[MEMSIZE];
        // Create a record from the input file
        RecordStruct createARecord(int, int);
        // Read records from input file to memory
        void readRecordIntoMemory();
        // Output Memory
        void outputMemory();
        // Flush fuction
        void flushToNextLevel();
        // Sort Memory
        void insertionSortOnMemory(RecordStruct);
        // Delete if repeated
        void replaceIfRepeated(RecordStruct);

        // Constructor
        System(string);
        //Destructor
        ~System();
};

System::System(string file) {
    currentMemorySize = 0;
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
            //TODO: at this point I beleive we need to sort and flush the memory to the next level
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

void System::outputMemory() {
    for (int i = 0; i < currentMemorySize; i++) {
        cout << "Record: " << i << " Key:" << Mem[i].key << " Value:" << Mem[i].value << endl;
    }
    currentMemorySize = 0;
}

void System::flushToNextLevel(){

}

void System::insertionSortOnMemory(RecordStruct record){
    int j = currentMemorySize - 1;
    while (j >= 0 && Mem[j].key > record.key) {
        if(record.key == Mem[j].key){
            replaceIfRepeated(record);
            return;
        }
        Mem[j + 1] = Mem[j];
        j--;
    }
    Mem[j + 1] = record;
    currentMemorySize++;
}

void System::replaceIfRepeated(RecordStruct record){

}



#endif