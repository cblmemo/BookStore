//
// Created by Rainy Memory on 2021/1/12.
//

#ifndef BOOKSTORE_BOOKSTOREHEADER_H
#define BOOKSTORE_BOOKSTOREHEADER_H

#include "UnrolledLinkedList.h"
#include "MyException.hpp"
#include <sstream>

using std::stringstream;
using std::cin;

//file name
#define LOG_FILENAME "log.dat"
#define BILL_FILENAME "bill.dat"

#define BASIC_DATA_FILENAME "basicData.dat"
#define USER_DATA_FILENAME "userData.dat"
#define BOOK_DATA_FILENAME "bookData.dat"

#define INDEX_USERID_FILENAME "indexUserID.dat"
#define INDEX_ISBN_FILENAME "indexISBN.dat"
#define INDEX_AUTHOR_FILENAME "indexAuthor.dat"
#define INDEX_NAME_FILENAME "indexName.dat"
#define INDEX_KEYWORD_FILENAME "indexKeyWord.dat"

//error message
#define REMAINS_ERROR_MESSAGE "redundant information"//all
#define MISSING_ERROR_MESSAGE "missing "//all
#define INEXISTENT_ACCOUNT_MESSAGE "account doesn't exist"//su
#define INADEQUATE_AUTHORITY_MESSAGE "inadequate authority"//su
#define WRONG_PASSWORD_MESSAGE "password wrong"//su
#define NO_USER_LOGIN_NOW_MESSAGE "no user login now"//logout
#define WRONG_OLD_PASSWORD_MESSAGE "old password wrong"//passwd

#define UNKNOWN_ERROR_MESSAGE "unknown error"

//enum type:-----------\/

enum basicDataType {
    BOOKNUMBER, TRANSACTION, EXPENSE, INCOME
};

enum saveDataType {
    USER, BOOK
};

//enum type:-----------/\



//class:---------------\/

class Book {
public:
    double price = -1;
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
    long long time = 0;
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

string splitKeyWord(string keyWord, vector<string> &result);

int nowAuthority();//finished

bool authorityCheck(int requirements);

void runCommand(string cmd);

void logRecord(string logContent, string cmd);

//commandFunction:-----/\



//basicCommand:--------\/
void reportFinance();

void reportEmployee();

void reportMyself();

void showLog();

//basicCommand:--------/\



//bookCommand:---------\/

void selectBook(string ISBN);

void modify(string arguments);

void import(int quantity, double cost);

void show(string argument = "");

void showFinance(int times = -1);

void buy(string ISBN, int quantity);

//bookCommand:---------/\



//userCommand:---------\/

void login(string userID, string password = "");

UserAccount logout();//return logout account's user-id

void addAccount(const UserAccount &o,string userID);

void registerAccount(const UserAccount &o,string userID);

void deleteAccount(string userID);

void changePassword(string userID, string newPassword, string oldPassword = "");

//userCommand:---------/\



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
