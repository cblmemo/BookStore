//
// Created by Rainy Memory on 2021/1/12.
//

//finished

#include "BookStoreHeader.h"

// basicData.dat:
// int bookNumber;
// int totalTransaction;
// double totalExpense;
// double totalIncome;

fstream fs;

template<class T>
void writeBasicData(basicDataType type, T *ptr) {
    int pos;
    switch (type) {
        case BOOKNUMBER:
            pos = 0;
            break;
        case TRANSACTION:
            pos = sizeof(int);
            break;
        case EXPENSE:
            pos = 2 * sizeof(int);
            break;
        case INCOME:
            pos = 2 * sizeof(int) + sizeof(double);
            break;
    }
    fs.open(BASIC_DATA_FILENAME, ios::in | ios::out | ios::binary);
    fs.seekp(pos);
    fs.write(reinterpret_cast<const char *>(ptr), sizeof(T));
    fs.close();
}

template<class T>
void readBasicData(basicDataType type, T *ptr) {
    int pos;
    switch (type) {
        case BOOKNUMBER:
            pos = 0;
            break;
        case TRANSACTION:
            pos = sizeof(int);
            break;
        case EXPENSE:
            pos = 2 * sizeof(int);
            break;
        case INCOME:
            pos = 2 * sizeof(int) + sizeof(double);
            break;
    }
    fs.open(BASIC_DATA_FILENAME, ios::in | ios::binary);
    fs.seekg(pos);
    fs.read(reinterpret_cast<char *>(ptr), sizeof(T));
    fs.close();
}

template<class T>
int writeData(saveDataType type, const T &o, int offset) {
    fs.open((type == USER ? USER_DATA_FILENAME : BOOK_DATA_FILENAME), ios::in | ios::out | ios::binary);
    if (offset < 0) {
        fs.seekp(0, ios::end);
        offset = fs.tellp();
    }
    else fs.seekp(offset);
    fs.write(reinterpret_cast<const char *>(&o), sizeof(T));
    fs.close();
    return offset;
}

template<class T>
T readData(saveDataType type, int offset) {
    T temp;
    fs.open((type == USER ? USER_DATA_FILENAME : BOOK_DATA_FILENAME), ios::in | ios::binary);
    fs.seekg(offset);
    fs.read(reinterpret_cast<char *>(&temp), sizeof(T));
    fs.close();
    return temp;
}