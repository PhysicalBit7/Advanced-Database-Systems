# Tiered LSM-tree Simulation

This project simulates a Tiered LSM-tree. To simplify the assignment, no deletion is considered.

## System Setup

- _Complete_ - **BLOCKSIZE**: A constant used as the number of records that can be held in a block. (e.g., 2)
- _Complete_ - **THRESHOLD**: A constant used as the size ratio between levels. (e.g., 4)
- _Complete_ - - **Mem[MEMSIZE]**: An array of MEMSIZE records store data at level 0. MEMSIZE is a constant number for total records (integers) can be held in Mem[]. This constant MEMSIZE should be a number greater than or equal to (THRESHOLD+1)* BLOCKSIZE.
---
- Each layer of LSM has up to THRESHOLD files. The file name for tier t of layer i should be in the format: Li-t.txt. For example, L1-0.txt is the tier (chunk) 0 of data at level 1.
- Between records in a file, there is a newline (‘\n’).
- An array (or vector) to keep track of the number of chunks at each level.

## Record Structure

```cpp
struct RecStruct{ 
    int key; 
    int value;  
    // int status;     // 0: for normal, 1 for delete tombstone 
};
```


## Work
For each record read from the input file (prog02Data.txt): 
1. If end of file, write the contents (records) of Mem into L0.txt, write the number of 
chunks at each level to LevelInfo.txt, close all files and exit the program. 
2.  Maintain records in Mem sorted. 
3.  If repeated key, replace the old one with the newly read one.  
4.  If allocated Mem is full, flush to L1-i.txt where -i in the file name is a number to 
indicate the chunk id at level 1. 
5.  At level j, if the number of files is THRESHOLD, flush and merge-sort to next 
level, j+1. 
__Repeat this process if needed after flush and merge-sort to the next level__
 