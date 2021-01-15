//
// Created by Rainy Memory on 2021/1/12.
//

#include "BookStoreHeader.h"

//basicData: (in basicData.dat)
int bookNumber;
int totalTransaction;
double totalExpense;
double totalIncome;

//temporaryData:
vector<UserAccount> accountStack;
vector<int> selectedBookOffsetStack;//-1 represent not select

//UnrolledLinkedList:
//in userData.dat
UnrolledLinkedList indexUserID(INDEX_USERID_FILENAME);
//in bookData.dat
UnrolledLinkedList indexISBN(INDEX_ISBN_FILENAME);
UnrolledLinkedList indexAuthor(INDEX_AUTHOR_FILENAME);
UnrolledLinkedList indexName(INDEX_NAME_FILENAME);
UnrolledLinkedList indexKeyWord(INDEX_KEYWORD_FILENAME);

Book::Book() {}

Book::Book(double _price, int _quantity, string _ISBN, string _name, string _author, string _keyword) : price(_price), quantity(_quantity) {
    strcpy(ISBN, _ISBN.c_str());
    strcpy(name, _name.c_str());
    strcpy(author, _author.c_str());
    strcpy(keyword, _keyword.c_str());
}

void Book::show() const {
    cout << ISBN << "\t" << name << "\t" << author << "\t" << keyword << "\t" << std::setiosflags(ios::fixed) << std::setprecision(2) << price << "\t" << quantity << endl;
}

UserAccount::UserAccount() {}

UserAccount::UserAccount(int _authority, string _userID, string _name, string _password) : authority(_authority) {
    strcpy(userID, _userID.c_str());
    strcpy(name, _name.c_str());
    strcpy(password, _password.c_str());
}

void initialize() {
    fstream fs;
    fs.open(LOG_FILENAME, ios::in);
    if (!fs) {
        fs.clear();
        fs.close();
        
        //create file
        fs.open(LOG_FILENAME, ios::out);
        fs.close();
        fs.open(COMMAND_FILENAME, ios::out);
        fs.close();
        fs.open(STAFF_DATA_FILENAME, ios::out);
        fs.close();
        fs.open(STAFF_LOG_FILENAME, ios::out);
        fs.close();
        fs.open(BILL_FILENAME, ios::out);
        fs.close();
        fs.open(BASIC_DATA_FILENAME, ios::out);
        fs.close();
        fs.open(USER_DATA_FILENAME, ios::out);
        fs.close();
        fs.open(BOOK_DATA_FILENAME, ios::out);
        fs.close();
        fs.open(INDEX_USERID_FILENAME, ios::out);
        fs.close();
        fs.open(INDEX_ISBN_FILENAME, ios::out);
        fs.close();
        fs.open(INDEX_AUTHOR_FILENAME, ios::out);
        fs.close();
        fs.open(INDEX_NAME_FILENAME, ios::out);
        fs.close();
        fs.open(INDEX_KEYWORD_FILENAME, ios::out);
        fs.close();
        
        //create root account
        UserAccount root(7, "root", "root", "sjtu");
        int offset = writeData<UserAccount>(USER, root);
        Element temp(offset, "root");
        indexUserID.addElement(temp);
        
        //set basic data
        bookNumber = 0;
        totalTransaction = 0;
        totalExpense = 0;
        totalIncome = 0;
        writeBasicData<int>(BOOKNUMBER, bookNumber);
        writeBasicData<int>(TRANSACTION, totalTransaction);
        writeBasicData<double>(EXPENSE, totalExpense);
        writeBasicData<double>(INCOME, totalIncome);
    }
    else {
        //read basic data
        bookNumber = readBasicData<int>(BOOKNUMBER);
        totalTransaction = readBasicData<int>(TRANSACTION);
        totalExpense = readBasicData<double>(EXPENSE);
        totalIncome = readBasicData<double>(INCOME);
    }
}