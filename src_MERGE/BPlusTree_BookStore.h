//
// Created by Rainy Memory on 2021/2/3.
//

#ifndef CODE_BPLUSTREE_BOOKSTORE_H
#define CODE_BPLUSTREE_BOOKSTORE_H

#include "BPlusTree.h"
#include "BPlusTreeString.h"
#include "UnrolledLinkedList.h"

class BPlusTree {
private:
    RainyMemory::BPlusTree<BPlusTreeString, int> bpt;

public:
    BPlusTree(const string &_filename) : bpt(_filename) {}
    
    void findElement(const string &key, vector<int> &result) {
        BPlusTreeString temp(key);
        int res = bpt.find(temp);
        if (res >= 0)result.push_back(res);
    }
    
    void addElement(const Element &o) {
        string tt = o.key;
        BPlusTreeString temp(tt);
        bpt.insert(temp, o.offset);
    }
    
    void deleteElement(const Element &o) {
        string tt = o.key;
        BPlusTreeString temp(tt);
        bpt.erase(temp);
    }
};

#endif //CODE_BPLUSTREE_BOOKSTORE_H
