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
BPlusTree indexUserID(INDEX_USERID_FILENAME);
//in bookData.dat
BPlusTree indexISBN(INDEX_ISBN_FILENAME);
BPlusTree indexAuthor(INDEX_AUTHOR_FILENAME);
BPlusTree indexName(INDEX_NAME_FILENAME);
BPlusTree indexKeyWord(INDEX_KEYWORD_FILENAME);

Book::Book() = default;

Book::Book(double _price, int _quantity, const string &ISBN_, const string &_name, const string &_author, const string &_keyword) : price(_price), quantity(_quantity) {
    strcpy(ISBN, ISBN_.c_str());
    strcpy(name, _name.c_str());
    strcpy(author, _author.c_str());
    strcpy(keyword, _keyword.c_str());
}

void Book::show() const {
    cout << ISBN << "\t" << name << "\t" << author << "\t" << keyword << "\t" << setiosflags(ios::fixed) << setprecision(2) << price << "\t" << quantity << "\n";
}

bool Book::operator<(const Book &o) const {
    return strcmp(ISBN, o.ISBN) < 0;
}

UserAccount::UserAccount() = default;

UserAccount::UserAccount(int _authority, const string &_userID, const string &_name, const string &_password) : authority(_authority) {
    strcpy(userID, _userID.c_str());
    strcpy(name, _name.c_str());
    strcpy(password, _password.c_str());
}

bool UserAccount::operator==(const UserAccount &o) const {
    if (authority != o.authority)return false;
    else if (strcmp(userID, o.userID) != 0)return false;
    else if (strcmp(name, o.name) != 0)return false;
    else if (strcmp(password, o.password) != 0)return false;
    else return true;
}

Entry::Entry() = default;

Entry::Entry(const string &ISBN_, const string &_userID, int _operatorAuthority, int _quantity, double _totalPrice) : operatorAuthority(_operatorAuthority), quantity(_quantity), totalPrice(_totalPrice) {
    strcpy(ISBN, ISBN_.c_str());
    strcpy(userID, _userID.c_str());
    time_t now = time(nullptr);
    string timeStr = ctime(&now);
    strcpy(dealTime, timeStr.c_str());
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