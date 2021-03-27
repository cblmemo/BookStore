//
// Created by Rainy Memory on 2021/1/12.
//

#include "BookStoreHeader.h"

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