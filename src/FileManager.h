//
// Created by Rainy Memory on 2021/1/12.
//

//finished

#ifndef BOOKSTORE_FILEMANAGER_H
#define BOOKSTORE_FILEMANAGER_H

#include <fstream>

using std::fstream;
using std::ios;

// basicData.dat:
// int bookNumber;
// int totalTransaction;
// double totalExpense;
// double totalIncome;

enum basicDataType {
    BOOKNUMBER, TRANSACTION, EXPENSE, INCOME
};

template<class T>
void writeBasicData(basicDataType type, T *ptr);

template<class T>
void readBasicData(basicDataType type, T *ptr);

template<class T>
int writeData(const T &o, int offset = -1);

template<class T>
T readData(int offset);

#endif //BOOKSTORE_FILEMANAGER_H
