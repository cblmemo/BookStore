//
// Created by Rainy Memory on 2021/3/5.
//

#ifndef CODE_HASHBPLUSTREE_H
#define CODE_HASHBPLUSTREE_H

#include <iostream>
#include <cstring>
#include <string>
#include "BPlusTree.h"

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
public:
    class hashType {
        enum prime {
            PRIME = 1000000007
        };
        
        static int getHash1(const string &o) {
            int h = o.length();
            for (char i : o)h = (h << 4) ^ (h >> 28) ^ i;
            return h % PRIME;
        }
        
        static int getHash2(const string &o) {
            int h = o.length();
            for (char i : o)h = (h << 7) ^ (h >> 25) ^ i;
            return h % PRIME;
        }
    
    public:
        int hash1 = 0;
        int hash2 = 0;
        
        explicit hashType(const string &o) : hash1(getHash1(o)), hash2(getHash2(o)) {}
        
        hashType() = default;
        
        hashType &operator=(const hashType &o) = default;
        
        bool operator==(const hashType &o) const {
            return hash1 == o.hash1 && hash2 == o.hash2;
        }
        
        bool operator!=(const hashType &o) const {
            return !(*this == o);
        }
        
        bool operator<(const hashType &o) const {
            if (hash1 < o.hash1)return true;
            else if (hash1 > o.hash1)return false;
            else return hash2 < o.hash2;
        }
        
        bool operator>(const hashType &o) const {
            return !(*this == o) && !(*this < o);
        }
        
        bool operator<=(const hashType &o) const {
            return *this == o || *this < o;
        }
        
        bool operator>=(const hashType &o) const {
            return *this == o || *this > o;
        }
    };

private:
    RainyMemory::BPlusTree<hashType, int, 500, 500> bpt;

public:
    explicit BPlusTree(const string &_filename) : bpt(_filename) {}
    
    void findElement(const string &key, vector<int> &result) {
        hashType temp(key);
        bpt.find(temp, result);
    }
    
    void addElement(const Element &o) {
        hashType temp(o.key);
        bpt.insert(temp, o.offset);
    }
    
    void deleteElement(const Element &o) {
        hashType temp(o.key);
        bpt.erase(temp, o.offset);
    }
};

#endif //CODE_HASHBPLUSTREE_H
