#ifndef SYSTEM_H
#define SYSTEM_H

#include "Master.h"

class System {
    // some functions might be useful such as read records to memory:
    private:
        ifstream inputFile;
        int memoryLocation;
    public:
        // memory
        RecordStruct Mem[MEMSIZE];
        // Create a record from the input file
        RecordStruct createARecord(int, int);
        // Read records from input file to memory
        void readRecordIntoMemory();
        // Output Memory
        void outputMemory();

        // Constructor
        System(string);
        //Destructor
        ~System();
};

System::System(string file) {
    memoryLocation = 0;
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
    int keyFromFile;
    int valueFromFile;
    inputFile >> keyFromFile >> valueFromFile;
    RecordStruct record = createARecord(keyFromFile, valueFromFile);
    Mem[memoryLocation++] = record;
}

void System::outputMemory() {
    for (int i = 0; i < 2; i++) {
        cout << "Record: " << i << " Key:" << Mem[i].key << " Value:" << Mem[i].value << endl;
    }
}


#endif