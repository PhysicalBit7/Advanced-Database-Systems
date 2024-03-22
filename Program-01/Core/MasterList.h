#ifndef MASTERLIST_H
#define MASTERLIST_H

#include <vector>
#include <iostream>
#include "Sublist.h"

class MasterList{
    
public:
    std::vector<Sublist> myList;
    MasterList() {}
    ~MasterList(){}
    void addElement(Sublist);
    void printList();

};

void MasterList::addElement(Sublist value) {
    
}

// void SublistLinkedList::printList(){
//     std::cout << "Linked List: ";
//     for (const auto& element : myList) {
//         element.print();
//     }
//     std::cout << std::endl;
// }

#endif
