//
// Created by Rainy Memory on 2021/1/12.
//

#ifndef BOOKSTORE_BOOKSTOREHEADER_H
#define BOOKSTORE_BOOKSTOREHEADER_H

#include "UnrolledLinkedList.h"
#include "MyException.h"

#include <iomanip>
#include <sstream>
#include <ctime>

using std::stringstream;
using std::cin;

//file name
#define LOG_FILENAME "log.dat"
#define COMMAND_FILENAME "command.dat"
#define STAFF_LOG_FILENAME "staffLog.dat"
#define BILL_FILENAME "bill.dat"

#define BASIC_DATA_FILENAME "basicData.dat"
#define USER_DATA_FILENAME "userData.dat"
#define BOOK_DATA_FILENAME "bookData.dat"
#define STAFF_DATA_FILENAME "staffData.dat"

#define INDEX_USERID_FILENAME "indexUserID.dat"
#define INDEX_ISBN_FILENAME "indexISBN.dat"
#define INDEX_AUTHOR_FILENAME "indexAuthor.dat"
#define INDEX_NAME_FILENAME "indexName.dat"
#define INDEX_KEYWORD_FILENAME "indexKeyWord.dat"

//error message
#define REMAINS_ERROR_MESSAGE "redundant information"//all
#define INADEQUATE_AUTHORITY_MESSAGE "inadequate authority"//su
#define WRONG_PASSWORD_MESSAGE "password wrong"//su
#define NO_USER_LOGIN_NOW_MESSAGE "no user login now"//logout
#define DELETE_ROOT_ACCOUNT_MESSAGE "cannot delete root account"//delete
#define WRONG_OLD_PASSWORD_MESSAGE "old password wrong"//passwd
#define NO_BOOK_SELECTED_MESSAGE "no book was selected"//modify, import

#define UNKNOWN_ERROR_MESSAGE "unknown error"

//enum type:-----------\/

enum basicDataType {
    BOOKNUMBER, TRANSACTION, EXPENSE, INCOME
};

enum saveDataType {
    USER, BOOK
};

enum argumentType {
    ISBN_, NAME_, AUTHOR_, KEYWORD_, PRICE_, INVALID_
};

//enum type:-----------/\



//class:---------------\/

class Book {
public:
    double price = 0;
    int quantity = 0;
    char ISBN[20] = {0};
    char name[60] = {0};
    char author[60] = {0};
    char keyword[60] = {0};

public:
    Book();
    
    Book(double _price, int _quantity, string _ISBN, string _name, string _author, string _keyword);
    
    void show() const;
};

class UserAccount {
public:
    int authority = -1;
    char userID[30] = {0};
    char name[30] = {0};
    char password[30] = {0};

public:
    UserAccount();
    
    UserAccount(int _authority, string _userID, string _name, string _password);
};

class Entry {
public:
    time_t time;
    char userID[30] = {0};
    char ISBN[20] = {0};
    int quantity = 0;//positive represent buy, negative represent import
    double price = 0;
    double totalPrice = 0;
};

//class:---------------/\



//bookStore:-----------\/

void initialize();//finished

//bookStore:-----------/\



//commandFunction:-----\/

void splitKeyWord(string keyWordStr, vector<string> &keyWord);

argumentType getArgumentType(commandType type, string argument);

void deleteArgumentType(string &argument, argumentType type, commandType _type);

int nowSelected();

int nowAuthority();

void authorityCheck(int requirements, commandType type);

void argumentCheck(string argument, string argumentName, commandType type, int maxsize);

void runCommand(string cmd);

void staffRecord(const string &userID);

void getStaff(vector<string> &staff);

void logRecord(string logContent, const string &cmd);

void staffLogRecord(const string &type, const string &arguments);

//commandFunction:-----/\



//basicCommand:--------\/
void reportFinance();

void reportEmployee();

void reportMyself(string userID);

void showLog();

//basicCommand:--------/\



//bookCommand:---------\/

void selectBook(const string &ISBN);

void import(int quantity, double cost);

void showFinance(int times = -1);

void buy(string ISBN, int quantity);

//bookCommand:---------/\



//userCommand:---------\/

void login(const string &userID, const string &password = "");

UserAccount logout();//return logout account's user-id

void addAccount(const UserAccount &o, const string &userID);

void registerAccount(const UserAccount &o, const string &userID);

void deleteAccount(const string &userID);

void changePassword(const string &userID, const string &newPassword, const string &oldPassword = "");

//userCommand:---------/\



//FileManager:---------\/

// basicData.dat:
// int bookNumber;
// int totalTransaction;
// double totalExpense;
// double totalIncome;

template<class T>
void writeBasicData(basicDataType type, const T &o) {
    fstream fs;
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
    fs.write(reinterpret_cast<const char *>(&o), sizeof(T));
    fs.close();
}

template<class T>
T readBasicData(basicDataType type) {
    T save;
    fstream fs;
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
    fs.read(reinterpret_cast<char *>(&save), sizeof(T));
    fs.close();
    return save;
}

template<class T>
int writeData(saveDataType type, const T &o, int offset = -1) {
    fstream fs;
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
    fstream fs;
    T temp;
    fs.open((type == USER ? USER_DATA_FILENAME : BOOK_DATA_FILENAME), ios::in | ios::binary);
    fs.seekg(offset);
    fs.read(reinterpret_cast<char *>(&temp), sizeof(T));
    fs.close();
    return temp;
}

//FileManager:---------/\


#endif //BOOKSTORE_BOOKSTOREHEADER_H
