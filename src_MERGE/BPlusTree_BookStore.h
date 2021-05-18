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
        std::pair<int, bool> res = bpt.find(temp);
        if (res.second)result.push_back(res.first);
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
