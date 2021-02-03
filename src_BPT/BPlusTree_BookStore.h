//
// Created by Rainy Memory on 2021/2/3.
//

#ifndef CODE_BPLUSTREE_BOOKSTORE_H
#define CODE_BPLUSTREE_BOOKSTORE_H

#include "BPlusTree.h"
#include "BPlusTreeString.h"

class Element {
public:
    int offset;
    string key;
    
    explicit Element(int _offset = -1, const string &_key = "") : offset(_offset), key(_key) {}
    
    Element(const Element &o) : offset(o.offset), key(o.key) {}
    
    Element &operator=(const Element &o) {
        if (this == &o)return *this;
        offset = o.offset;
        key = o.key;
    }
};

class BPlusTree {
private:
    RainyMemory::BPlusTree<BPlusTreeString, int> bpt;

public:
    BPlusTree(const string &_filename) : bpt(_filename) {}
    
    void findElement(const string &key, vector<int> &result) {
        BPlusTreeString temp(key);
        bpt.find(temp, result);
    }
    
    void addElement(const Element &o) {
        BPlusTreeString temp(o.key);
        bpt.insert(temp, o.offset);
    }
    
    void deleteElement(const Element &o) {
        BPlusTreeString temp(o.key);
        bpt.erase(temp, o.offset);
    }
};

#endif //CODE_BPLUSTREE_BOOKSTORE_H
