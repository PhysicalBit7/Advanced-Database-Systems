/**
 * @brief A class whose purpose is to mimick a hard drive
 *
 * This class mimicks a hard drive. It stores data from a file in an array of a specified size.
 */

#ifndef HardDrive_h
#define HardDrive_h

#include <iostream>
#include <string>
#include <fstream>



class HardDrive {
//Hard Drive Size
private:
    int* array;
    int currentSizeOfHD;
    int initialSizeOfHD;

//HD[0] == number of records stored
public:
    HardDrive(int);
    ~HardDrive();
    void initializeHD(std::string);
    int initialSize();
    int currentSize();
    void print(std::ofstream&);
    int& operator[](int index);
    void updateSize(int);
    void printFrom(int, int, std::ofstream&);
    //int HDnextToRead
    //int HDnextToWrite

};

/**
 * @brief Class Constructor
 *
 * Constructor builds the hard drive from a specified size and initializes all elements to 0
 *
 * @param size Size of the hard drive
 */
HardDrive::HardDrive(int size){
    array = new int[size];

    for (int i = 0; i < size; ++i) {
        array[i] = 0;  // Initialize array elements to 0
    }
}

/**
 * @brief Destroy the Hard Drive:: Hard Drive object
 * @date 2024-02-19
 */
HardDrive::~HardDrive() {
    delete[] array; 
}

/**
 * @brief Initilizes the hard drive from the data in a specified file
 * @param    {std::string}      Takes in a file name to open
 * @date 2024-02-19
 */
void HardDrive::initializeHD(std::string file){
    std::ifstream inputFile(file);

    // Check if the file is successfully opened
    if (inputFile.is_open()) {
        // File is open, you can read from it
    } else {
        std::cerr << "Error opening the file for reading." << std::endl;
    }

    //Initialize HD
    int num;
    int i = 1;
    while (inputFile >> num) {
        // Process each number as needed
        array[i++] = num;
    }
    array[0] = i - 1;
    initialSizeOfHD = i - 1;
    currentSizeOfHD = i - 1;
}

/**
 * @brief Print the hard drive contents
 * @param    {std::ofstream}    The open file to output results to
 * @date 2024-02-19
 */
void HardDrive::print(std::ofstream& outputFile){
    for(int i = 0; i <= this->currentSize(); i++){
        outputFile << array[i] << " ";
    }
    outputFile << std::endl;
}


/**
 * @brief The initial size of the hard drive
 * @return The initial size 
 * @date 2024-02-19
 */
int HardDrive::initialSize(){
    return initialSizeOfHD;
}


/**
 * @brief Updates the size of the hard drive
 * @param    {int}  How many to increment the hard drive size by
 * @date 2024-02-19
 */
void HardDrive::updateSize(int add){
    currentSizeOfHD += add;
    array[0] = currentSizeOfHD;
}


/**
 * @brief The current size of the hard drive
 * @return Returns an int of the current size 
 * @date 2024-02-19
 */
int HardDrive::currentSize(){
    return currentSizeOfHD;
}


/**
 * @brief Operator overload
 * @param    {int}  Index by which to access
 * @return int& 
 * @date 2024-02-19
 */
int& HardDrive::operator[](int index) {
    return array[index];
}


/**
 * @brief Prints from a specified point in the hard drive
 * @param    {int} The starting index
 * @param    {int} The ending index
 * @param    {std::ofstream} The output file to print to
 * @date 2024-02-19
 */
void HardDrive::printFrom(int startingIndex, int endingIndex, std::ofstream& outputFile){
    for(int i = startingIndex; i < endingIndex; i++){
        outputFile << array[i] << " ";
    }
}




#endif
