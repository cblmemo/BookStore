//
// Created by Rainy Memory on 2021/1/12.
//

#ifndef BOOKSTORE_BOOKSTOREHEADER_H
#define BOOKSTORE_BOOKSTOREHEADER_H

#include "UnrolledLinkedList.h"
#include "Exception.h"

#define BASIC_DATA_FILENAME "basicData.dat"
#define USER_DATA_FILENAME "userData.dat"
#define BOOK_DATA_FILENAME "bookData.dat"

//enum type:-----------\/

enum commandType {
    SU, LOGOUT, USERADD, REGISTER, DELETE, PASSWD, SELECT, MODIFY, IMPORT,
    SHOW, SHOWFINANCE, BUY, REPORTFINANCE, REPORTEMPLOYEE, REPORTMYSELF, LOG
};

enum basicDataType {
    BOOKNUMBER, TRANSACTION, EXPENSE, INCOME
};

enum saveDataType {
    USER, BOOK
};

//enum type:-----------/\



//class:---------------\/

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

class UserAccount {
public:
    int authority;
    char userID[30];
    char name[30];
    char password[30];
};

//class:---------------/\



//BookStore:-----------\/

void initialize();

//BookStore:-----------/\



//CommandFunction:-----\/

void stringCheck(commandType type, string cmd);

string splitString(string &cmd, bool keyWordFlag = false);//delete and return the first word of cmd

bool privilegeCheck(int requirements);

void runCommand(string cmd);

void logRecord();

//CommandFunction:-----/\



//BasicCommand:--------\/
void reportFinance();

void reportEmployee();

void reportMyself();

void log();

//BasicCommand:--------/\



//BookCommand:---------\/

void selectBook(string ISBN);

void modify(string arguments);

void import(int quantity, double cost);

void show(string argument = "");

void showFinance(int times = -1);

void buy(string ISBN, int quantity);

//BookCommand:---------/\



//UserCommand:---------\/

bool login(string userID, string password = "");

bool logout();

void addAccount(const UserAccount &o);

void registerAccount(const UserAccount &o);

void deleteAccount(string userID);

void changePassword(string userID, string newPassword, string oldPassword = "");

//UserCommand:---------/\



//FileManager:---------\/

template<class T>
void writeBasicData(basicDataType type, T *ptr);

template<class T>
void readBasicData(basicDataType type, T *ptr);

template<class T>
int writeData(saveDataType type, const T &o, int offset = -1);

template<class T>
T readData(saveDataType type, int offset);

//FileManager:---------/\


#endif //BOOKSTORE_BOOKSTOREHEADER_H
