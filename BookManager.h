//
// Created by Rainy Memory on 2021/1/10.
//

#ifndef BOOKSTORE_BOOKMANAGER_H
#define BOOKSTORE_BOOKMANAGER_H

#include <iostream>

using std::cout;
using std::endl;

class Book {
private:
    char ISBN[20] = {0};
    char name[60] = {0};
    char author[60] = {0};
    char keyword[60] = {0};
    double price = 0;
    int quantity = 0;

public:
    void show() const {
        cout << ISBN << "\t" << name << "\t" << author << "\t" << keyword << "\t" << price << "\t" << quantity << endl;
    }
};

class Administrator{
public:
    void runProgramme();
};



#endif //BOOKSTORE_BOOKMANAGER_H
