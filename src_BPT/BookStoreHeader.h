//
// Created by Rainy Memory on 2021/1/12.
//

#ifndef BOOKSTORE_BOOKSTOREHEADER_H
#define BOOKSTORE_BOOKSTOREHEADER_H

#include "HashBPlusTree.h"
#include "MyException.h"
#include "TokenScanner.h"

#include <iomanip>

#include <algorithm>
#include <ctime>

//#define log

using std::cin;
using std::sort;
using std::find;
using std::setiosflags;
using std::setprecision;
using RainyMemory::TokenScanner;

//file name
#define LOG_FILENAME "log.dat"
#define COMMAND_FILENAME "command.dat"
#define STAFF_LOG_FILENAME "staffLog.dat"
#define BILL_FILENAME "bill.dat"

#define BASIC_DATA_FILENAME "basicData.dat"
#define USER_DATA_FILENAME "userData.dat"
#define BOOK_DATA_FILENAME "bookData.dat"
#define STAFF_DATA_FILENAME "staffData.dat"

#define INDEX_USERID_FILENAME "indexUserID"
#define INDEX_ISBN_FILENAME "indexISBN"
#define INDEX_AUTHOR_FILENAME "indexAuthor"
#define INDEX_NAME_FILENAME "indexName"
#define INDEX_KEYWORD_FILENAME "indexKeyWord"

//error message
#define REMAINS_ERROR_MESSAGE "redundant information"//all
#define INADEQUATE_AUTHORITY_MESSAGE "inadequate authority"//all
#define WRONG_PASSWORD_MESSAGE "password wrong"//su, passwd
#define NO_USER_LOGIN_NOW_MESSAGE "no user login now"//logout
#define DELETE_ROOT_ACCOUNT_MESSAGE "cannot delete root account"//delete
#define DELETE_ALREADY_LOGIN_ACCOUNT_MESSAGE "cannot delete account that is already login"//delete
#define WRONG_OLD_PASSWORD_MESSAGE "old password wrong"//passwd
#define NO_BOOK_SELECTED_MESSAGE "no book was selected"//modify, import
#define REPEATED_KEYWORDS_MESSAGE "repeated keywords"//modify
#define INEXIST_BOOK_MESSAGE "this book doesn't exist"//buy
#define NO_ENOUGH_INVENTORY_MESSAGE "no enough inventory"//buy
#define BOSS_REPORT_ITSELF_MESSAGE "boss trying to report itself"//report myself
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

class BasicData {
public:
    int bookNumber = 0;
    int totalTransaction = 0;
    double totalExpense = 0;
    double totalIncome = 0;
};

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
    
    Book(double _price, int _quantity, const string &ISBN_, const string &_name, const string &_author, const string &_keyword);
    
    void show() const;
    
    bool operator<(const Book &o) const;
};

class UserAccount {
public:
    int authority = -1;
    char userID[30] = {0};
    char name[30] = {0};
    char password[30] = {0};

public:
    UserAccount();
    
    UserAccount(int _authority, const string &_userID, const string &_name, const string &_password);
    
    bool operator==(const UserAccount &o) const;
};

class Entry {
public:
    char dealTime[40] = {0};
    char ISBN[20] = {0};
    char userID[30] = {0};
    int operatorAuthority = -1;
    int quantity = 0;//positive represent buy(income), negative represent import(expense)
    double totalPrice = 0;//positive represent buy(income), negative represent import(expense)

public:
    Entry();
    
    Entry(const string &ISBN_, const string &_userID, int _operatorAuthority, int _quantity, double _totalPrice);
};

//class:---------------/\



//commandFunction:-----\/

void initialize();

void splitKeyWord(const string &keyWordStr, vector<string> &keyWord);

argumentType getArgumentType(commandType type, const string &argument);

void deleteArgumentType(string &argument, argumentType type, commandType _type);

int nowSelected();

int nowAuthority();

void authorityCheck(int requirements, commandType type);

void argumentCheck(const string &argument, const string &argumentNameStr, commandType type, int maxsize);

void runCommand(const string &cmd);

void entryRecord(const Entry &o);

void calculateEntry(int start, int end, double &income, double &expense);

//commandFunction:-----/\



//bookCommand:---------\/

void selectBook(const string &ISBN);

void import(int quantity, double cost_price);

double buy(const string &ISBN, int quantity);

//bookCommand:---------/\



//userCommand:---------\/

void login(const string &userID, const string &password = "");

UserAccount logout();//return logout account's user-id

void addAccount(const UserAccount &o, const string &userID);

void registerAccount(const UserAccount &o, const string &userID);

void deleteAccount(const string &userID);

void changePassword(const string &userID, const string &newPassword, const string &oldPassword = "");

//userCommand:---------/\


#endif //BOOKSTORE_BOOKSTOREHEADER_H
